/*
══════════════════════════════════════════════════════════════════════════════
 [FILM NEG INVERT] [1.1.0] - FILMNEGINVERTPLUGIN.CPP
 [Plugin implementation based on the OpenFX C++ support library.]
 [C++]
══════════════════════════════════════════════════════════════════════════════
Taller Lab - 2026
https://tallerlab.com/
VAI CORINTHIANS!!
══════════════════════════════════════════════════════════════════════════════
*/

// ===========================================
// 1. INCLUDES
// ===========================================

#include "FilmNegInvertPlugin.h"
#include "FilmNegInvertPluginFactory.h"
#include "ofxsImageEffect.h"
#include "ofxsProcessing.h"
#include <algorithm>
#include <cstddef>
#include <cmath>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <vector>

// ===========================================
// 2. INTERNAL HELPERS
// ===========================================

namespace {

const float kMinMaskValue = 1.0e-4f;
const float kFloatEpsilon = 1.0e-4f;
const float kMonochromeWeights[3] = { 0.2126f, 0.7152f, 0.0722f };
const int kFloatLUTResolution = 16384;
const float kFloatLUTMaxInput = 4.0f;

inline float clamp01(float value)
{
    return std::max(0.0f, std::min(1.0f, value));
}

inline bool nearlyEqual(double a, double b, double epsilon = 1.0e-4)
{
    return std::fabs(a - b) <= epsilon;
}

inline float cineonToLinear(float value)
{
    const float codeValue = clamp01(value) * 1023.0f;
    return std::max(0.0f, std::pow(10.0f, (codeValue - 685.0f) / 300.0f));
}

struct RenderSettings {
    float invMask[3];
    float gammaExponent;
    float exposure;
    float channelFactor[3];
    bool colorInvert;
    bool inputIsLog;
    bool monochromeOutput;
};

inline float transformChannel(float value, const RenderSettings& settings, int channel)
{
    if (settings.inputIsLog) {
        value = cineonToLinear(value);
    }

    value = std::max(0.0f, value * settings.invMask[channel]);
    return std::pow(value, settings.gammaExponent) * settings.channelFactor[channel] * settings.exposure;
}

inline void processChannels(float& r, float& g, float& b, const RenderSettings& settings)
{
    r = transformChannel(r, settings, 0);
    g = transformChannel(g, settings, 1);
    b = transformChannel(b, settings, 2);

    if (settings.monochromeOutput) {
        const float mono = (kMonochromeWeights[0] * r) +
                           (kMonochromeWeights[1] * g) +
                           (kMonochromeWeights[2] * b);
        r = mono;
        g = mono;
        b = mono;
    }

    if (settings.colorInvert) {
        r = 1.0f - r;
        g = 1.0f - g;
        b = 1.0f - b;
    }

    r = clamp01(r);
    g = clamp01(g);
    b = clamp01(b);
}

template <class PIX, int maxValue>
struct PixelConverter {
    static float toUnit(PIX value)
    {
        return static_cast<float>(value) / static_cast<float>(maxValue);
    }

    static PIX fromUnit(float value)
    {
        const float scaled = clamp01(value) * static_cast<float>(maxValue);
        return static_cast<PIX>(scaled + 0.5f);
    }
};

template <>
struct PixelConverter<float, 1> {
    static float toUnit(float value)
    {
        return value;
    }

    static float fromUnit(float value)
    {
        return clamp01(value);
    }
};

template <class PIX, int maxValue>
struct ChannelLUT {
    std::vector<PIX> colorOutput[3];
    std::vector<float> monochromeContribution[3];

    void build(const RenderSettings& settings)
    {
        const int tableSize = maxValue + 1;
        for (int channel = 0; channel < 3; ++channel) {
            colorOutput[channel].resize(tableSize);
            monochromeContribution[channel].resize(tableSize);
        }

        for (int i = 0; i < tableSize; ++i) {
            const PIX inputValue = static_cast<PIX>(i);
            const float unitValue = PixelConverter<PIX, maxValue>::toUnit(inputValue);

            for (int channel = 0; channel < 3; ++channel) {
                const float transformed = transformChannel(unitValue, settings, channel);
                monochromeContribution[channel][i] = transformed * kMonochromeWeights[channel];

                float output = transformed;
                if (settings.colorInvert) {
                    output = 1.0f - output;
                }
                colorOutput[channel][i] = PixelConverter<PIX, maxValue>::fromUnit(output);
            }
        }
    }
};

struct FloatChannelLUT {
    std::vector<float> transformed[3];
    float step;
    float inverseStep;
    float maxInput;

    FloatChannelLUT()
        : step(0.0f)
        , inverseStep(0.0f)
        , maxInput(0.0f)
    {
    }

    void build(const RenderSettings& settings)
    {
        maxInput = settings.inputIsLog ? 1.0f : kFloatLUTMaxInput;
        const int sampleCount = kFloatLUTResolution + 1;
        step = maxInput / static_cast<float>(kFloatLUTResolution);
        inverseStep = (step > 0.0f) ? (1.0f / step) : 0.0f;

        for (int channel = 0; channel < 3; ++channel) {
            transformed[channel].resize(sampleCount);
        }

        for (int i = 0; i < sampleCount; ++i) {
            const float inputValue = step * static_cast<float>(i);
            for (int channel = 0; channel < 3; ++channel) {
                transformed[channel][i] = transformChannel(inputValue, settings, channel);
            }
        }
    }

    float sample(float value, const RenderSettings& settings, int channel) const
    {
        if (value <= 0.0f) {
            return transformed[channel][0];
        }
        if (value >= maxInput) {
            return transformChannel(value, settings, channel);
        }

        const float position = value * inverseStep;
        int index = static_cast<int>(position);
        if (index >= kFloatLUTResolution) {
            index = kFloatLUTResolution - 1;
        }

        const float fraction = position - static_cast<float>(index);
        const float a = transformed[channel][index];
        const float b = transformed[channel][index + 1];
        return a + ((b - a) * fraction);
    }
};

template <class PIX>
inline PIX* getRowPointer(unsigned char* base,
                          std::ptrdiff_t rowBytes,
                          const OfxRectI& bounds,
                          int x,
                          int y,
                          int nComponents)
{
    return reinterpret_cast<PIX*>(base +
                                  (static_cast<std::ptrdiff_t>(y - bounds.y1) * rowBytes) +
                                  (static_cast<std::ptrdiff_t>(x - bounds.x1) * nComponents * sizeof(PIX)));
}

template <class PIX>
inline const PIX* getRowPointer(const unsigned char* base,
                                std::ptrdiff_t rowBytes,
                                const OfxRectI& bounds,
                                int x,
                                int y,
                                int nComponents)
{
    return reinterpret_cast<const PIX*>(base +
                                        (static_cast<std::ptrdiff_t>(y - bounds.y1) * rowBytes) +
                                        (static_cast<std::ptrdiff_t>(x - bounds.x1) * nComponents * sizeof(PIX)));
}

template <class PIX, int nComponents>
inline void clearPixels(PIX*& dstPix, int pixelCount)
{
    for (int i = 0; i < pixelCount; ++i) {
        for (int channel = 0; channel < nComponents; ++channel) {
            dstPix[channel] = 0;
        }
        dstPix += nComponents;
    }
}

class NegativeProcessorBase : public OFX::ImageProcessor {
protected:
    OFX::Image* _srcImg;
    RenderSettings _settings;

public:
    explicit NegativeProcessorBase(OFX::ImageEffect& effect)
        : OFX::ImageProcessor(effect)
        , _srcImg(0)
    {
    }

    void setSrcImg(OFX::Image* srcImg)
    {
        _srcImg = srcImg;
    }

    void setSettings(const RenderSettings& settings)
    {
        _settings = settings;
    }
};

template <class PIX, int nComponents, int maxValue>
class NegativeProcessor : public NegativeProcessorBase {
private:
    ChannelLUT<PIX, maxValue> _lut;

public:
    explicit NegativeProcessor(OFX::ImageEffect& effect)
        : NegativeProcessorBase(effect)
    {
    }

    void setSettings(const RenderSettings& settings)
    {
        NegativeProcessorBase::setSettings(settings);
        _lut.build(settings);
    }

    void multiThreadProcessImages(OfxRectI procWindow) override
    {
        const OfxRectI dstBounds = _dstImg->getBounds();
        unsigned char* dstBase = static_cast<unsigned char*>(_dstImg->getPixelData());
        const std::ptrdiff_t dstRowBytes = _dstImg->getRowBytes();
        const unsigned char* srcBase = _srcImg ? static_cast<const unsigned char*>(_srcImg->getPixelData()) : 0;
        const OfxRectI srcBounds = _srcImg ? _srcImg->getBounds() : OfxRectI();
        const std::ptrdiff_t srcRowBytes = _srcImg ? _srcImg->getRowBytes() : 0;

        for (int y = procWindow.y1; y < procWindow.y2; ++y) {
            if (_effect.abort()) {
                break;
            }

            PIX* dstPix = getRowPointer<PIX>(dstBase, dstRowBytes, dstBounds, procWindow.x1, y, nComponents);

            if (!srcBase || y < srcBounds.y1 || y >= srcBounds.y2) {
                clearPixels<PIX, nComponents>(dstPix, procWindow.x2 - procWindow.x1);
                continue;
            }

            const int validX1 = std::max(procWindow.x1, srcBounds.x1);
            const int validX2 = std::min(procWindow.x2, srcBounds.x2);

            if (validX1 > procWindow.x1) {
                clearPixels<PIX, nComponents>(dstPix, validX1 - procWindow.x1);
            }

            if (validX1 < validX2) {
                const PIX* srcPix = getRowPointer<PIX>(srcBase, srcRowBytes, srcBounds, validX1, y, nComponents);

                if (_settings.monochromeOutput) {
                    for (int x = validX1; x < validX2; ++x) {
                        float mono = _lut.monochromeContribution[0][srcPix[0]] +
                                     _lut.monochromeContribution[1][srcPix[1]] +
                                     _lut.monochromeContribution[2][srcPix[2]];
                        if (_settings.colorInvert) {
                            mono = 1.0f - mono;
                        }
                        const PIX output = PixelConverter<PIX, maxValue>::fromUnit(mono);

                        dstPix[0] = output;
                        dstPix[1] = output;
                        dstPix[2] = output;
                        if (nComponents == 4) {
                            dstPix[3] = srcPix[3];
                        }

                        srcPix += nComponents;
                        dstPix += nComponents;
                    }
                } else {
                    for (int x = validX1; x < validX2; ++x) {
                        dstPix[0] = _lut.colorOutput[0][srcPix[0]];
                        dstPix[1] = _lut.colorOutput[1][srcPix[1]];
                        dstPix[2] = _lut.colorOutput[2][srcPix[2]];
                        if (nComponents == 4) {
                            dstPix[3] = srcPix[3];
                        }

                        srcPix += nComponents;
                        dstPix += nComponents;
                    }
                }
            }

            if (validX2 < procWindow.x2) {
                clearPixels<PIX, nComponents>(dstPix, procWindow.x2 - validX2);
            }
        }
    }
};

template <int nComponents>
class NegativeProcessor<float, nComponents, 1> : public NegativeProcessorBase {
private:
    FloatChannelLUT _lut;

public:
    explicit NegativeProcessor(OFX::ImageEffect& effect)
        : NegativeProcessorBase(effect)
    {
    }

    void setSettings(const RenderSettings& settings)
    {
        NegativeProcessorBase::setSettings(settings);
        _lut.build(settings);
    }

    void multiThreadProcessImages(OfxRectI procWindow) override
    {
        const OfxRectI dstBounds = _dstImg->getBounds();
        unsigned char* dstBase = static_cast<unsigned char*>(_dstImg->getPixelData());
        const std::ptrdiff_t dstRowBytes = _dstImg->getRowBytes();
        const unsigned char* srcBase = _srcImg ? static_cast<const unsigned char*>(_srcImg->getPixelData()) : 0;
        const OfxRectI srcBounds = _srcImg ? _srcImg->getBounds() : OfxRectI();
        const std::ptrdiff_t srcRowBytes = _srcImg ? _srcImg->getRowBytes() : 0;

        for (int y = procWindow.y1; y < procWindow.y2; ++y) {
            if (_effect.abort()) {
                break;
            }

            float* dstPix = getRowPointer<float>(dstBase, dstRowBytes, dstBounds, procWindow.x1, y, nComponents);

            if (!srcBase || y < srcBounds.y1 || y >= srcBounds.y2) {
                clearPixels<float, nComponents>(dstPix, procWindow.x2 - procWindow.x1);
                continue;
            }

            const int validX1 = std::max(procWindow.x1, srcBounds.x1);
            const int validX2 = std::min(procWindow.x2, srcBounds.x2);

            if (validX1 > procWindow.x1) {
                clearPixels<float, nComponents>(dstPix, validX1 - procWindow.x1);
            }

            if (validX1 < validX2) {
                const float* srcPix = getRowPointer<float>(srcBase, srcRowBytes, srcBounds, validX1, y, nComponents);

                if (_settings.monochromeOutput) {
                    for (int x = validX1; x < validX2; ++x) {
                        float mono =
                            (_lut.sample(srcPix[0], _settings, 0) * kMonochromeWeights[0]) +
                            (_lut.sample(srcPix[1], _settings, 1) * kMonochromeWeights[1]) +
                            (_lut.sample(srcPix[2], _settings, 2) * kMonochromeWeights[2]);
                        const float alpha = (nComponents == 4) ? srcPix[3] : 0.0f;

                        if (_settings.colorInvert) {
                            mono = 1.0f - mono;
                        }

                        mono = clamp01(mono);
                        dstPix[0] = mono;
                        dstPix[1] = mono;
                        dstPix[2] = mono;
                        if (nComponents == 4) {
                            dstPix[3] = alpha;
                        }

                        srcPix += nComponents;
                        dstPix += nComponents;
                    }
                } else {
                    for (int x = validX1; x < validX2; ++x) {
                        float r = _lut.sample(srcPix[0], _settings, 0);
                        float g = _lut.sample(srcPix[1], _settings, 1);
                        float b = _lut.sample(srcPix[2], _settings, 2);
                        const float alpha = (nComponents == 4) ? srcPix[3] : 0.0f;

                        if (_settings.colorInvert) {
                            r = 1.0f - r;
                            g = 1.0f - g;
                            b = 1.0f - b;
                        }

                        dstPix[0] = clamp01(r);
                        dstPix[1] = clamp01(g);
                        dstPix[2] = clamp01(b);
                        if (nComponents == 4) {
                            dstPix[3] = alpha;
                        }

                        srcPix += nComponents;
                        dstPix += nComponents;
                    }
                }
            }

            if (validX2 < procWindow.x2) {
                clearPixels<float, nComponents>(dstPix, procWindow.x2 - validX2);
            }
        }
    }
};

template <class ProcessorType>
void setupAndProcessProcessor(OFX::ImageEffect& effect,
                              OFX::Clip* srcClip,
                              OFX::Clip* dstClip,
                              ProcessorType& processor,
                              const RenderSettings& settings,
                              const OFX::RenderArguments& args)
{
    std::unique_ptr<OFX::Image> dstImg(dstClip->fetchImage(args.time));
    std::unique_ptr<OFX::Image> srcImg(srcClip->fetchImage(args.time));

    if (!srcImg || !dstImg) {
        throw std::runtime_error("Failed to fetch images");
    }

    const OFX::BitDepthEnum srcBitDepth = srcImg->getPixelDepth();
    const OFX::BitDepthEnum dstBitDepth = dstImg->getPixelDepth();
    const OFX::PixelComponentEnum srcComponents = srcImg->getPixelComponents();
    const OFX::PixelComponentEnum dstComponents = dstImg->getPixelComponents();

    if (srcBitDepth != dstBitDepth || srcComponents != dstComponents) {
        throw std::runtime_error("Source and destination images must have the same bit depth and components");
    }

    processor.setDstImg(dstImg.get());
    processor.setSrcImg(srcImg.get());
    processor.setSettings(settings);
    processor.setRenderWindow(args.renderWindow);
    processor.setGPURenderArgs(args);
    processor.process();
}

} // namespace

// ===========================================
// 3. PLUGIN IMPLEMENTATION
// ===========================================

namespace FilmNegInvert {

// ===========================================
// 4. LIFECYCLE
// ===========================================

FilmNegInvertPlugin::FilmNegInvertPlugin(OfxImageEffectHandle handle)
    : OFX::ImageEffect(handle)
{
    _srcClip = fetchClip(kOfxImageEffectSimpleSourceClipName);
    _dstClip = fetchClip(kOfxImageEffectOutputClipName);

    _paramMaskColor = fetchRGBParam(kParamMaskColor);
    _paramGamma = fetchDoubleParam(kParamGamma);
    _paramExposure = fetchDoubleParam(kParamExposure);
    _paramInputType = fetchChoiceParam(kParamInputType);
    _paramFilmType = fetchChoiceParam(kParamFilmType);
    _paramRedFactor = fetchDoubleParam(kParamRedFactor);
    _paramGreenFactor = fetchDoubleParam(kParamGreenFactor);
    _paramBlueFactor = fetchDoubleParam(kParamBlueFactor);
    _paramColorInvert = fetchBooleanParam(kParamColorInvert);
}

// ===========================================
// 5. RENDERING
// ===========================================

void FilmNegInvertPlugin::render(const OFX::RenderArguments& args)
{
    if (!_srcClip || !_dstClip) {
        throw std::runtime_error("Invalid clips");
    }

    double maskR = 1.0;
    double maskG = 1.0;
    double maskB = 1.0;
    _paramMaskColor->getValue(maskR, maskG, maskB);

    double gamma = 1.0;
    _paramGamma->getValue(gamma);

    double exposure = 1.0;
    _paramExposure->getValue(exposure);

    int inputType = 0;
    _paramInputType->getValue(inputType);

    int filmType = 0;
    _paramFilmType->getValue(filmType);

    double redFactor = 1.0;
    double greenFactor = 1.0;
    double blueFactor = 1.0;
    _paramRedFactor->getValue(redFactor);
    _paramGreenFactor->getValue(greenFactor);
    _paramBlueFactor->getValue(blueFactor);

    bool colorInvert = false;
    _paramColorInvert->getValue(colorInvert);

    RenderSettings settings;
    settings.invMask[0] = 1.0f / std::max(kMinMaskValue, static_cast<float>(maskR));
    settings.invMask[1] = 1.0f / std::max(kMinMaskValue, static_cast<float>(maskG));
    settings.invMask[2] = 1.0f / std::max(kMinMaskValue, static_cast<float>(maskB));
    settings.gammaExponent = 1.0f / std::max(0.1f, static_cast<float>(gamma));
    settings.exposure = std::max(0.0f, static_cast<float>(exposure));
    settings.channelFactor[0] = std::max(0.0f, static_cast<float>(redFactor));
    settings.channelFactor[1] = std::max(0.0f, static_cast<float>(greenFactor));
    settings.channelFactor[2] = std::max(0.0f, static_cast<float>(blueFactor));
    settings.colorInvert = colorInvert;
    settings.inputIsLog = (inputType == 1);
    settings.monochromeOutput = (filmType == 1);

    const OFX::PixelComponentEnum components = _dstClip->getPixelComponents();
    const OFX::BitDepthEnum bitDepth = _dstClip->getPixelDepth();

    if (components == OFX::ePixelComponentRGBA) {
        switch (bitDepth) {
        case OFX::eBitDepthUByte: {
            NegativeProcessor<unsigned char, 4, 255> processor(*this);
            setupAndProcessProcessor(*this, _srcClip, _dstClip, processor, settings, args);
            break;
        }
        case OFX::eBitDepthUShort: {
            NegativeProcessor<unsigned short, 4, 65535> processor(*this);
            setupAndProcessProcessor(*this, _srcClip, _dstClip, processor, settings, args);
            break;
        }
        case OFX::eBitDepthFloat: {
            NegativeProcessor<float, 4, 1> processor(*this);
            setupAndProcessProcessor(*this, _srcClip, _dstClip, processor, settings, args);
            break;
        }
        default:
            OFX::throwSuiteStatusException(kOfxStatErrUnsupported);
        }
    } else if (components == OFX::ePixelComponentRGB) {
        switch (bitDepth) {
        case OFX::eBitDepthUByte: {
            NegativeProcessor<unsigned char, 3, 255> processor(*this);
            setupAndProcessProcessor(*this, _srcClip, _dstClip, processor, settings, args);
            break;
        }
        case OFX::eBitDepthUShort: {
            NegativeProcessor<unsigned short, 3, 65535> processor(*this);
            setupAndProcessProcessor(*this, _srcClip, _dstClip, processor, settings, args);
            break;
        }
        case OFX::eBitDepthFloat: {
            NegativeProcessor<float, 3, 1> processor(*this);
            setupAndProcessProcessor(*this, _srcClip, _dstClip, processor, settings, args);
            break;
        }
        default:
            OFX::throwSuiteStatusException(kOfxStatErrUnsupported);
        }
    } else {
        OFX::throwSuiteStatusException(kOfxStatErrUnsupported);
    }
}

// ===========================================
// 6. PARAMETER EVENTS
// ===========================================

void FilmNegInvertPlugin::changedParam(const OFX::InstanceChangedArgs& args, const std::string& paramName)
{
    if (args.reason != OFX::eChangeUserEdit) {
        return;
    }

    if (paramName == kParamFilmType) {
        int filmType = 0;
        _paramFilmType->getValue(filmType);
        applyFilmTypePreset(filmType);
    } else if (paramName == kParamReset) {
        resetToDefaults();
    }
}

// ===========================================
// 7. IDENTITY CHECK
// ===========================================

bool FilmNegInvertPlugin::isIdentity(const OFX::IsIdentityArguments& args, OFX::Clip*& identityClip, double& identityTime)
{
    double maskR = 1.0;
    double maskG = 1.0;
    double maskB = 1.0;
    _paramMaskColor->getValue(maskR, maskG, maskB);

    double gamma = 1.0;
    _paramGamma->getValue(gamma);

    double exposure = 1.0;
    _paramExposure->getValue(exposure);

    int inputType = 0;
    _paramInputType->getValue(inputType);

    int filmType = 0;
    _paramFilmType->getValue(filmType);

    double redFactor = 1.0;
    double greenFactor = 1.0;
    double blueFactor = 1.0;
    _paramRedFactor->getValue(redFactor);
    _paramGreenFactor->getValue(greenFactor);
    _paramBlueFactor->getValue(blueFactor);

    bool colorInvert = false;
    _paramColorInvert->getValue(colorInvert);

    const bool isIdentityState =
        nearlyEqual(maskR, 1.0) &&
        nearlyEqual(maskG, 1.0) &&
        nearlyEqual(maskB, 1.0) &&
        nearlyEqual(gamma, 1.0) &&
        nearlyEqual(exposure, 1.0) &&
        nearlyEqual(redFactor, 1.0) &&
        nearlyEqual(greenFactor, 1.0) &&
        nearlyEqual(blueFactor, 1.0) &&
        (inputType == 0) &&
        (filmType == 0) &&
        !colorInvert;

    if (isIdentityState) {
        identityClip = _srcClip;
        identityTime = args.time;
        return true;
    }

    return false;
}

// ===========================================
// 8. PRESETS
// ===========================================

void FilmNegInvertPlugin::applyFilmTypePreset(int filmType)
{
    if (filmType == 1) {
        _paramMaskColor->setValue(0.76, 0.72, 0.68);
        _paramGamma->setValue(0.55);
        _paramRedFactor->setValue(1.0);
        _paramGreenFactor->setValue(1.0);
        _paramBlueFactor->setValue(1.0);
    } else {
        _paramMaskColor->setValue(0.82, 0.60, 0.38);
        _paramGamma->setValue(0.60);
        _paramRedFactor->setValue(1.0);
        _paramGreenFactor->setValue(1.0);
        _paramBlueFactor->setValue(1.0);
    }
}

// ===========================================
// 9. DEFAULTS
// ===========================================

void FilmNegInvertPlugin::resetToDefaults()
{
    _paramInputType->setValue(0);
    _paramFilmType->setValue(0);
    _paramMaskColor->setValue(0.8, 0.5, 0.3);
    _paramGamma->setValue(0.6);
    _paramExposure->setValue(1.0);
    _paramRedFactor->setValue(1.0);
    _paramGreenFactor->setValue(1.0);
    _paramBlueFactor->setValue(1.0);
    _paramColorInvert->setValue(false);
}

} // namespace FilmNegInvert
