#include "TallerNegInvertPluginFactory.h"
#include "TallerNegInvertPlugin.h"

#define kPluginIdentifier "com.tallerneg.TallerNegInvert"
#define kPluginName "TallerNegInvert"
#define kPluginVersionMajor 1
#define kPluginVersionMinor 0

TallerNegInvertPluginFactory::TallerNegInvertPluginFactory()
    : OFX::PluginFactoryHelper<TallerNegInvertPluginFactory>(
          kPluginIdentifier, kPluginVersionMajor, kPluginVersionMinor) {}

void TallerNegInvertPluginFactory::describe(OFX::ImageEffectDescriptor &desc) {
  desc.setLabel(kPluginName);
  desc.setPluginGrouping("Color");
  desc.setPluginDescription(
      "Inverts film negative (Vision3 50D) removing orange mask.");

  // Support for contexts
  desc.addSupportedContext(OFX::eContextFilter);
  desc.addSupportedContext(OFX::eContextGeneral);

  // Support for bit depths (Float is critical for color work)
  desc.addSupportedBitDepth(OFX::eBitDepthFloat);

  // CUDA Support
  // Note: kOfxImageEffectPropCudaRenderSupported is defined in ofxRendering.h
  // or similar extensions. Since we might not have the extension header, we use
  // the string property directly if needed, but usually the wrapper handles it
  // or we set it via property set. Using the raw property string
  // "OfxImageEffectPropCudaRenderSupported"
  desc.getPropertySet().propSetInt("OfxImageEffectPropCudaRenderSupported", 1);

  desc.setSupportsTiles(true);
}

void TallerNegInvertPluginFactory::describeInContext(
    OFX::ImageEffectDescriptor &desc, OFX::ContextEnum context) {
  // Input Clip (Source)
  OFX::ClipDescriptor *srcClip =
      desc.defineClip(kOfxImageEffectSimpleSourceClipName);
  srcClip->addSupportedComponent(OFX::ePixelComponentRGBA);
  srcClip->addSupportedComponent(OFX::ePixelComponentRGB);
  srcClip->setSupportsTiles(true);

  // Output Clip
  OFX::ClipDescriptor *dstClip = desc.defineClip(kOfxImageEffectOutputClipName);
  dstClip->addSupportedComponent(OFX::ePixelComponentRGBA);
  dstClip->addSupportedComponent(OFX::ePixelComponentRGB);
  dstClip->setSupportsTiles(true);

  // Parameters

  // Mask Color (RGB)
  OFX::RGBParamDescriptor *maskColor = desc.defineRGBParam("MaskColor");
  maskColor->setLabel("Mask Color");
  maskColor->setDefault(0.8, 0.5, 0.3); // Typical orange mask
  maskColor->setHint("Pick the film base color (between perforations).");

  // Gamma
  OFX::DoubleParamDescriptor *gamma = desc.defineDoubleParam("Gamma");
  gamma->setLabel("Film Gamma");
  gamma->setDefault(0.6);
  gamma->setRange(0.1, 2.0);
  gamma->setDisplayRange(0.4, 1.0);

  // Exposure
  OFX::DoubleParamDescriptor *exposure = desc.defineDoubleParam("Exposure");
  exposure->setLabel("Exposure");
  exposure->setDefault(1.0);
  exposure->setRange(0.0, 10.0);
  exposure->setDisplayRange(0.0, 5.0);

  // Input Type
  OFX::ChoiceParamDescriptor *inputType = desc.defineChoiceParam("InputType");
  inputType->setLabel("Input Type");
  inputType->appendOption("Linear (RAW)");
  inputType->appendOption("Log (Cineon/DPX)");
  inputType->setDefault(0); // Linear

  // Film Type
  OFX::ChoiceParamDescriptor *filmType = desc.defineChoiceParam("FilmType");
  filmType->setLabel("Film Type");
  filmType->appendOption("Vision3 (Color)");
  filmType->appendOption("Double-X (B&W)");
  filmType->setDefault(0);
}

OFX::ImageEffect *
TallerNegInvertPluginFactory::createInstance(OfxImageEffectHandle handle,
                                             OFX::ContextEnum context) {
  return new TallerNegInvertPlugin(handle);
}

namespace OFX {
namespace Plugin {
void getPluginIDs(OFX::PluginFactoryArray &p_Array) {
  static TallerNegInvertPluginFactory p_Factory;
  p_Array.push_back(&p_Factory);
}
} // namespace Plugin
} // namespace OFX
