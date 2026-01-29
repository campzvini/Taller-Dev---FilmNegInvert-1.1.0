/*
══════════════════════════════════════════════════════════════════════════════
 FILM NEG INVERT 1.0 - FILMNEGINVERTPLUGIN.CPP
 Implementação do plugin Film Neg Invert usando Support Library C++ do OpenFX
 C++ Source
══════════════════════════════════════════════════════════════════════════════
Taller Dev - 2026
VAI CORINTHIANS!!
══════════════════════════════════════════════════════════════════════════════
*/

// ============================================
// 1. INCLUDES # Bibliotecas necessárias
// ============================================
#include "FilmNegInvertPlugin.h"
#include "FilmNegInvertPluginFactory.h"
#include <cstring>
#include <cmath>
#include <algorithm>
#include <stdexcept>

// ============================================
// 2. NAMESPACE # Namespace do projeto
// ============================================
namespace FilmNegInvert {

// ============================================
// 3. CONSTRUCTOR IMPLEMENTATION # Implementação do construtor
// ============================================
FilmNegInvertPlugin::FilmNegInvertPlugin(OfxImageEffectHandle handle)
    : OFX::ImageEffect(handle) {
    
    // ============================================
    // 3.1. FETCH CLIPS # Obter clips
    // ============================================
    _srcClip = fetchClip(kOfxImageEffectSimpleSourceClipName);
    _dstClip = fetchClip(kOfxImageEffectOutputClipName);

    // ============================================
    // 3.2. FETCH PARAMETERS # Obter parâmetros
    // ============================================
    _paramAlgorithm = fetchChoiceParam(kParamAlgorithm);
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

// ============================================
// 4. RENDER IMPLEMENTATION # Implementação da renderização
// ============================================
void FilmNegInvertPlugin::render(const OFX::RenderArguments& args) {
    // ============================================
    // 4.1. VALIDATE CLIPS # Validar clips
    // ============================================
    if (!_srcClip || !_dstClip) {
        throw std::runtime_error("Invalid clips");
    }

    // ============================================
    // 4.2. FETCH IMAGES # Obter imagens
    // ============================================
    std::unique_ptr<OFX::Image> srcImg(_srcClip->fetchImage(args.time));
    std::unique_ptr<OFX::Image> dstImg(_dstClip->fetchImage(args.time));

    if (!srcImg || !dstImg) {
        throw std::runtime_error("Failed to fetch images");
    }

    // ============================================
    // 4.3. VALIDATE IMAGE PROPERTIES # Validar propriedades das imagens
    // ============================================
    OFX::BitDepthEnum srcBitDepth = srcImg->getPixelDepth();
    OFX::BitDepthEnum dstBitDepth = dstImg->getPixelDepth();
    OFX::PixelComponentEnum srcComponents = srcImg->getPixelComponents();
    OFX::PixelComponentEnum dstComponents = dstImg->getPixelComponents();

    if (srcBitDepth != dstBitDepth || srcComponents != dstComponents) {
        throw std::runtime_error("Source and destination images must have the same bit depth and components");
    }

    // ============================================
    // 4.4. PROCESS IMAGE # Processar imagem
    // ============================================
    const void* srcPtr = srcImg->getPixelData();
    void* dstPtr = dstImg->getPixelData();
    int srcRowBytes = srcImg->getRowBytes();
    int dstRowBytes = dstImg->getRowBytes();
    int nComponents = srcImg->getPixelComponentCount();

    processImage(srcPtr, dstPtr, srcRowBytes, dstRowBytes, args.renderWindow, srcBitDepth, nComponents);
}

// ============================================
// 5. CHANGED PARAM IMPLEMENTATION # Implementação de mudança de parâmetro
// ============================================
void FilmNegInvertPlugin::changedParam(const OFX::InstanceChangedArgs& args, const std::string& paramName) {
    // ============================================
    // 5.1. HANDLE ANALYZE BUTTON # Tratar botão de análise
    // ============================================
    if (paramName == kParamAnalyze && args.reason == OFX::eChangeUserEdit) {
        // Criar argumentos de renderização para análise
        OFX::RenderArguments renderArgs;
        renderArgs.time = args.time;
        renderArgs.renderScale = args.renderScale;
        renderArgs.fieldToRender = OFX::eFieldNone;
        renderArgs.renderQualityDraft = false;
        
        // Obter bounds da imagem de origem
        OfxRectD bounds = _srcClip->getRegionOfDefinition(args.time);
        renderArgs.renderWindow.x1 = static_cast<int>(bounds.x1);
        renderArgs.renderWindow.y1 = static_cast<int>(bounds.y1);
        renderArgs.renderWindow.x2 = static_cast<int>(bounds.x2);
        renderArgs.renderWindow.y2 = static_cast<int>(bounds.y2);
        
        // Analisar cor da máscara
        analyzeMaskColor(renderArgs);
    }
    // ============================================
    // 5.2. HANDLE HELP BUTTON # Tratar botão de ajuda
    // ============================================
    else if (paramName == kParamHelp && args.reason == OFX::eChangeUserEdit) {
        sendMessage(OFX::Message::eMessageMessage, "", 
            "Film Neg Invert - Help\n\n"
            "This plugin removes the colored mask from film negatives and inverts the image.\n\n"
            "Steps:\n"
            "1. Load your negative scan\n"
            "2. Click 'Analyze Mask' to automatically detect the mask color\n"
            "3. Adjust Mask Color if needed (pick from between perforations)\n"
            "4. Adjust Film Gamma and Exposure as needed\n"
            "5. Enable Color Invert to see the positive image\n\n"
            "For best results, use high-quality scans with good exposure.");
    }
}

// ============================================
// 6. IS IDENTITY IMPLEMENTATION # Implementação de verificação de identidade
// ============================================
bool FilmNegInvertPlugin::isIdentity(const OFX::IsIdentityArguments& args, OFX::Clip*& identityClip, double& identityTime) {
    // ============================================
    // 6.1. CHECK IF PASS-THROUGH # Verificar se é pass-through
    // ============================================
    // Se a exposição for 1.0 e não houver inversão de cor, pode ser pass-through
    double exposure;
    _paramExposure->getValue(exposure);
    
    bool colorInvert;
    _paramColorInvert->getValue(colorInvert);
    
    if (exposure == 1.0 && !colorInvert) {
        identityClip = _srcClip;
        identityTime = args.time;
        return true;
    }
    
    return false;
}

// ============================================
// 7. PROCESS IMAGE IMPLEMENTATION # Implementação do processamento de imagem
// ============================================
void FilmNegInvertPlugin::processImage(const void* srcPtr, void* dstPtr, int srcRowBytes, int dstRowBytes,
                                        const OfxRectI& renderWindow, OFX::BitDepthEnum pixelDepth, int nComponents) {
    // ============================================
    // 7.1. GET PARAMETER VALUES # Obter valores dos parâmetros
    // ============================================
    double maskR, maskG, maskB;
    _paramMaskColor->getValue(maskR, maskG, maskB);
    
    double gamma;
    _paramGamma->getValue(gamma);
    
    double exposure;
    _paramExposure->getValue(exposure);
    
    int inputType;
    _paramInputType->getValue(inputType);
    
    int filmType;
    _paramFilmType->getValue(filmType);
    
    double redFactor, greenFactor, blueFactor;
    _paramRedFactor->getValue(redFactor);
    _paramGreenFactor->getValue(greenFactor);
    _paramBlueFactor->getValue(blueFactor);
    
    bool colorInvert;
    _paramColorInvert->getValue(colorInvert);
    
    // ============================================
    // 7.2. PROCESS BY BIT DEPTH # Processar por profundidade de bit
    // ============================================
    if (pixelDepth == OFX::eBitDepthFloat) {
        // ============================================
        // 7.2.1. FLOAT PROCESSING # Processamento em ponto flutuante
        // ============================================
        const float* srcRow = static_cast<const float*>(srcPtr);
        float* dstRow = static_cast<float*>(dstPtr);
        
        int height = renderWindow.y2 - renderWindow.y1;
        int width = renderWindow.x2 - renderWindow.x1;
        
        // Calcular offset inicial
        int srcOffset = renderWindow.y1 * srcRowBytes + renderWindow.x1 * nComponents * sizeof(float);
        int dstOffset = renderWindow.y1 * dstRowBytes + renderWindow.x1 * nComponents * sizeof(float);
        
        srcRow = reinterpret_cast<const float*>(reinterpret_cast<const char*>(srcPtr) + srcOffset);
        dstRow = reinterpret_cast<float*>(reinterpret_cast<char*>(dstPtr) + dstOffset);
        
        // Processar linha por linha
        for (int y = 0; y < height; ++y) {
            const float* srcPixel = srcRow;
            float* dstPixel = dstRow;
            
            for (int x = 0; x < width; ++x) {
                float r = srcPixel[0];
                float g = srcPixel[1];
                float b = srcPixel[2];
                float a = (nComponents == 4) ? srcPixel[3] : 1.0f;
                
                // ============================================
                // 7.2.1.1. DIVIDE BY MASK # Dividir pela máscara (normalizar)
                // ============================================
                // A divisão pela máscara normaliza a imagem e realiza a inversão implicitamente
                // A base do filme (máscara) se torna 1.0 (branco) após divisão
                r = std::max(0.0f, r / std::max(0.0001f, static_cast<float>(maskR)));
                g = std::max(0.0f, g / std::max(0.0001f, static_cast<float>(maskG)));
                b = std::max(0.0f, b / std::max(0.0001f, static_cast<float>(maskB)));
                
                // ============================================
                // 7.2.1.2. APPLY CONTRAST # Aplicar contraste
                // ============================================
                r = std::pow(r, static_cast<float>(1.0 / gamma)) * static_cast<float>(redFactor);
                g = std::pow(g, static_cast<float>(1.0 / gamma)) * static_cast<float>(greenFactor);
                b = std::pow(b, static_cast<float>(1.0 / gamma)) * static_cast<float>(blueFactor);
                
                // ============================================
                // 7.2.1.3. APPLY EXPOSURE # Aplicar exposição
                // ============================================
                r *= static_cast<float>(exposure);
                g *= static_cast<float>(exposure);
                b *= static_cast<float>(exposure);
                
                // ============================================
                // 7.2.1.4. INVERT COLORS (OPTIONAL) # Inverter cores (opcional)
                // ============================================
                // Inversão simples de curva (igual ao "Invert" do DaVinci Resolve)
                // Isso aplica a fórmula: Saída = 1.0 - Entrada
                // Cada canal RGB é invertido individualmente
                if (colorInvert) {
                    r = 1.0f - r;
                    g = 1.0f - g;
                    b = 1.0f - b;
                }
                
                // ============================================
                // 7.2.1.5. CLAMP VALUES # Limitar valores
                // ============================================
                r = std::max(0.0f, std::min(1.0f, r));
                g = std::max(0.0f, std::min(1.0f, g));
                b = std::max(0.0f, std::min(1.0f, b));
                
                // ============================================
                // 7.2.1.6. WRITE OUTPUT # Escrever saída
                // ============================================
                dstPixel[0] = r;
                dstPixel[1] = g;
                dstPixel[2] = b;
                if (nComponents == 4) {
                    dstPixel[3] = a;
                }
                
                srcPixel += nComponents;
                dstPixel += nComponents;
            }
            
            srcRow = reinterpret_cast<const float*>(reinterpret_cast<const char*>(srcRow) + srcRowBytes);
            dstRow = reinterpret_cast<float*>(reinterpret_cast<char*>(dstRow) + dstRowBytes);
        }
    }
    else if (pixelDepth == OFX::eBitDepthUByte) {
        // ============================================
        // 7.2.2. BYTE PROCESSING # Processamento em byte
        // ============================================
        const unsigned char* srcRow = static_cast<const unsigned char*>(srcPtr);
        unsigned char* dstRow = static_cast<unsigned char*>(dstPtr);
        
        int height = renderWindow.y2 - renderWindow.y1;
        int width = renderWindow.x2 - renderWindow.x1;
        
        // Calcular offset inicial
        int srcOffset = renderWindow.y1 * srcRowBytes + renderWindow.x1 * nComponents;
        int dstOffset = renderWindow.y1 * dstRowBytes + renderWindow.x1 * nComponents;
        
        srcRow += srcOffset;
        dstRow += dstOffset;
        
        // Processar linha por linha
        for (int y = 0; y < height; ++y) {
            const unsigned char* srcPixel = srcRow;
            unsigned char* dstPixel = dstRow;
            
            for (int x = 0; x < width; ++x) {
                // Converter para float
                float r = srcPixel[0] / 255.0f;
                float g = srcPixel[1] / 255.0f;
                float b = srcPixel[2] / 255.0f;
                unsigned char a = (nComponents == 4) ? srcPixel[3] : 255;
                
                // Dividir pela máscara (normalizar)
                r = std::max(0.0f, r / std::max(0.0001f, static_cast<float>(maskR)));
                g = std::max(0.0f, g / std::max(0.0001f, static_cast<float>(maskG)));
                b = std::max(0.0f, b / std::max(0.0001f, static_cast<float>(maskB)));
                
                // Aplicar contraste
                r = std::pow(r, static_cast<float>(1.0 / gamma)) * static_cast<float>(redFactor);
                g = std::pow(g, static_cast<float>(1.0 / gamma)) * static_cast<float>(greenFactor);
                b = std::pow(b, static_cast<float>(1.0 / gamma)) * static_cast<float>(blueFactor);
                
                // Aplicar exposição
                r *= static_cast<float>(exposure);
                g *= static_cast<float>(exposure);
                b *= static_cast<float>(exposure);
                
                // ============================================
                // 7.2.2.4. INVERT COLORS (OPTIONAL) # Inverter cores (opcional)
                // ============================================
                // Inversão simples de curva (igual ao "Invert" do DaVinci Resolve)
                // Isso aplica a fórmula: Saída = 1.0 - Entrada
                // Cada canal RGB é invertido individualmente
                if (colorInvert) {
                    r = 1.0f - r;
                    g = 1.0f - g;
                    b = 1.0f - b;
                }
                
                // Limitar valores
                r = std::max(0.0f, std::min(1.0f, r));
                g = std::max(0.0f, std::min(1.0f, g));
                b = std::max(0.0f, std::min(1.0f, b));
                
                // Converter de volta para byte
                dstPixel[0] = static_cast<unsigned char>(r * 255.0f);
                dstPixel[1] = static_cast<unsigned char>(g * 255.0f);
                dstPixel[2] = static_cast<unsigned char>(b * 255.0f);
                if (nComponents == 4) {
                    dstPixel[3] = a;
                }
                
                srcPixel += nComponents;
                dstPixel += nComponents;
            }
            
            srcRow += srcRowBytes;
            dstRow += dstRowBytes;
        }
    }
    else if (pixelDepth == OFX::eBitDepthUShort) {
        // ============================================
        // 7.2.3. SHORT PROCESSING # Processamento em short
        // ============================================
        const unsigned short* srcRow = static_cast<const unsigned short*>(srcPtr);
        unsigned short* dstRow = static_cast<unsigned short*>(dstPtr);
        
        int height = renderWindow.y2 - renderWindow.y1;
        int width = renderWindow.x2 - renderWindow.x1;
        
        // Calcular offset inicial
        int srcOffset = renderWindow.y1 * srcRowBytes + renderWindow.x1 * nComponents * sizeof(unsigned short);
        int dstOffset = renderWindow.y1 * dstRowBytes + renderWindow.x1 * nComponents * sizeof(unsigned short);
        
        srcRow = reinterpret_cast<const unsigned short*>(reinterpret_cast<const char*>(srcPtr) + srcOffset);
        dstRow = reinterpret_cast<unsigned short*>(reinterpret_cast<char*>(dstPtr) + dstOffset);
        
        // Processar linha por linha
        for (int y = 0; y < height; ++y) {
            const unsigned short* srcPixel = srcRow;
            unsigned short* dstPixel = dstRow;
            
            for (int x = 0; x < width; ++x) {
                // Converter para float
                float r = srcPixel[0] / 65535.0f;
                float g = srcPixel[1] / 65535.0f;
                float b = srcPixel[2] / 65535.0f;
                unsigned short a = (nComponents == 4) ? srcPixel[3] : 65535;
                
                // Dividir pela máscara (normalizar)
                r = std::max(0.0f, r / std::max(0.0001f, static_cast<float>(maskR)));
                g = std::max(0.0f, g / std::max(0.0001f, static_cast<float>(maskG)));
                b = std::max(0.0f, b / std::max(0.0001f, static_cast<float>(maskB)));
                
                // Aplicar contraste
                r = std::pow(r, static_cast<float>(1.0 / gamma)) * static_cast<float>(redFactor);
                g = std::pow(g, static_cast<float>(1.0 / gamma)) * static_cast<float>(greenFactor);
                b = std::pow(b, static_cast<float>(1.0 / gamma)) * static_cast<float>(blueFactor);
                
                // Aplicar exposição
                r *= static_cast<float>(exposure);
                g *= static_cast<float>(exposure);
                b *= static_cast<float>(exposure);
                
                // ============================================
                // 7.2.2.4. INVERT COLORS (OPTIONAL) # Inverter cores (opcional)
                // ============================================
                // Inversão simples de curva (igual ao "Invert" do DaVinci Resolve)
                // Isso aplica a fórmula: Saída = 1.0 - Entrada
                // Cada canal RGB é invertido individualmente
                if (colorInvert) {
                    r = 1.0f - r;
                    g = 1.0f - g;
                    b = 1.0f - b;
                }
                
                // Limitar valores
                r = std::max(0.0f, std::min(1.0f, r));
                g = std::max(0.0f, std::min(1.0f, g));
                b = std::max(0.0f, std::min(1.0f, b));
                
                // Converter de volta para short
                dstPixel[0] = static_cast<unsigned short>(r * 65535.0f);
                dstPixel[1] = static_cast<unsigned short>(g * 65535.0f);
                dstPixel[2] = static_cast<unsigned short>(b * 65535.0f);
                if (nComponents == 4) {
                    dstPixel[3] = a;
                }
                
                srcPixel += nComponents;
                dstPixel += nComponents;
            }
            
            srcRow = reinterpret_cast<const unsigned short*>(reinterpret_cast<const char*>(srcRow) + srcRowBytes);
            dstRow = reinterpret_cast<unsigned short*>(reinterpret_cast<char*>(dstRow) + dstRowBytes);
        }
    }
}

// ============================================
// 8. ANALYZE MASK COLOR IMPLEMENTATION # Implementação da análise de cor da máscara
// ============================================
void FilmNegInvertPlugin::analyzeMaskColor(const OFX::RenderArguments& args) {
    // ============================================
    // 8.1. FETCH SOURCE IMAGE # Obter imagem de origem
    // ============================================
    std::unique_ptr<OFX::Image> srcImg(_srcClip->fetchImage(args.time));
    
    if (!srcImg) {
        throw std::runtime_error("Failed to fetch source image for analysis");
    }
    
    // ============================================
    // 8.2. GET IMAGE PROPERTIES # Obter propriedades da imagem
    // ============================================
    OFX::BitDepthEnum pixelDepth = srcImg->getPixelDepth();
    int nComponents = srcImg->getPixelComponentCount();
    int rowBytes = srcImg->getRowBytes();
    const void* srcPtr = srcImg->getPixelData();
    
    const OfxRectI& bounds = args.renderWindow;
    int width = bounds.x2 - bounds.x1;
    int height = bounds.y2 - bounds.y1;
    
    // ============================================
    // 8.3. ANALYZE BY BIT DEPTH # Analisar por profundidade de bit
    // ============================================
    if (pixelDepth == OFX::eBitDepthFloat) {
        // ============================================
        // 8.3.1. FLOAT ANALYSIS # Análise em ponto flutuante
        // ============================================
        const float* srcRow = static_cast<const float*>(srcPtr);
        
        double maxY = 0.0;
        double sumR = 0.0, sumG = 0.0, sumB = 0.0;
        int count = 0;
        
        for (int y = 0; y < height; ++y) {
            const float* srcPixel = reinterpret_cast<const float*>(reinterpret_cast<const char*>(srcPtr) + y * rowBytes);
            
            for (int x = 0; x < width; ++x) {
                float r = srcPixel[0];
                float g = srcPixel[1];
                float b = srcPixel[2];
                
                // Converter RGB para YUV
                double yVal, uVal, vVal;
                rgbToYuv(r, g, b, yVal, uVal, vVal);
                
                // Encontrar Y máximo
                if (yVal > maxY) {
                    maxY = yVal;
                    sumR = r;
                    sumG = g;
                    sumB = b;
                    count = 1;
                }
                else if (std::abs(yVal - maxY) < 0.01) {
                    // Acumular valores próximos ao Y máximo
                    sumR += r;
                    sumG += g;
                    sumB += b;
                    count++;
                }
                
                srcPixel += nComponents;
            }
        }
        
        // ============================================
        // 8.3.2. SET MASK COLOR # Definir cor da máscara
        // ============================================
        if (count > 0) {
            _paramMaskColor->setValue(sumR / count, sumG / count, sumB / count);
        }
    }
    else if (pixelDepth == OFX::eBitDepthUByte) {
        // ============================================
        // 8.3.2. BYTE ANALYSIS # Análise em byte
        // ============================================
        const unsigned char* srcRow = static_cast<const unsigned char*>(srcPtr);
        
        double maxY = 0.0;
        double sumR = 0.0, sumG = 0.0, sumB = 0.0;
        int count = 0;
        
        for (int y = 0; y < height; ++y) {
            const unsigned char* srcPixel = srcRow + y * rowBytes;
            
            for (int x = 0; x < width; ++x) {
                float r = srcPixel[0] / 255.0f;
                float g = srcPixel[1] / 255.0f;
                float b = srcPixel[2] / 255.0f;
                
                // Converter RGB para YUV
                double yVal, uVal, vVal;
                rgbToYuv(r, g, b, yVal, uVal, vVal);
                
                // Encontrar Y máximo
                if (yVal > maxY) {
                    maxY = yVal;
                    sumR = r;
                    sumG = g;
                    sumB = b;
                    count = 1;
                }
                else if (std::abs(yVal - maxY) < 0.01) {
                    sumR += r;
                    sumG += g;
                    sumB += b;
                    count++;
                }
                
                srcPixel += nComponents;
            }
        }
        
        if (count > 0) {
            _paramMaskColor->setValue(sumR / count, sumG / count, sumB / count);
        }
    }
    else if (pixelDepth == OFX::eBitDepthUShort) {
        // ============================================
        // 8.3.3. SHORT ANALYSIS # Análise em short
        // ============================================
        const unsigned short* srcRow = static_cast<const unsigned short*>(srcPtr);
        
        double maxY = 0.0;
        double sumR = 0.0, sumG = 0.0, sumB = 0.0;
        int count = 0;
        
        for (int y = 0; y < height; ++y) {
            const unsigned short* srcPixel = reinterpret_cast<const unsigned short*>(reinterpret_cast<const char*>(srcPtr) + y * rowBytes);
            
            for (int x = 0; x < width; ++x) {
                float r = srcPixel[0] / 65535.0f;
                float g = srcPixel[1] / 65535.0f;
                float b = srcPixel[2] / 65535.0f;
                
                // Converter RGB para YUV
                double yVal, uVal, vVal;
                rgbToYuv(r, g, b, yVal, uVal, vVal);
                
                // Encontrar Y máximo
                if (yVal > maxY) {
                    maxY = yVal;
                    sumR = r;
                    sumG = g;
                    sumB = b;
                    count = 1;
                }
                else if (std::abs(yVal - maxY) < 0.01) {
                    sumR += r;
                    sumG += g;
                    sumB += b;
                    count++;
                }
                
                srcPixel += nComponents;
            }
        }
        
        if (count > 0) {
            _paramMaskColor->setValue(sumR / count, sumG / count, sumB / count);
        }
    }
}

// ============================================
// 9. RGB TO YUV IMPLEMENTATION # Implementação da conversão RGB para YUV
// ============================================
void FilmNegInvertPlugin::rgbToYuv(double r, double g, double b, double& y, double& u, double& v) {
    // Conversão RGB para YUV (Rec. 601)
    y = 0.299 * r + 0.587 * g + 0.114 * b;
    u = -0.14713 * r - 0.28886 * g + 0.436 * b;
    v = 0.615 * r - 0.51499 * g - 0.10001 * b;
}

// ============================================
// 10. YUV TO RGB IMPLEMENTATION # Implementação da conversão YUV para RGB
// ============================================
void FilmNegInvertPlugin::yuvToRgb(double y, double u, double v, double& r, double& g, double& b) {
    // Conversão YUV para RGB (Rec. 601)
    r = y + 1.13983 * v;
    g = y - 0.39465 * u - 0.58060 * v;
    b = y + 2.03211 * u;
}

} // namespace FilmNegInvert
