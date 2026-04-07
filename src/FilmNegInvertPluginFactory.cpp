/*
══════════════════════════════════════════════════════════════════════════════
 [FILM NEG INVERT] [1.1.0] - FILMNEGINVERTPLUGINFACTORY.CPP
 [Factory definition and UI descriptor setup.]
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

#include "FilmNegInvertPluginFactory.h"
#include "FilmNegInvertPlugin.h"
#include "ofxsImageEffect.h"

// ===========================================
// 2. NAMESPACE
// ===========================================

namespace FilmNegInvert {

// ===========================================
// 3. FACTORY LIFECYCLE
// ===========================================

FilmNegInvertPluginFactory::FilmNegInvertPluginFactory()
    : OFX::PluginFactoryHelper<FilmNegInvertPluginFactory>("com.tallerneg.FilmNegInvert", 1, 0)
{
}

// ===========================================
// 4. PLUGIN DESCRIPTOR
// ===========================================

void FilmNegInvertPluginFactory::describe(OFX::ImageEffectDescriptor& desc)
{
    desc.setLabel("Film Neg Invert");
    desc.setPluginGrouping("Color");
    desc.setPluginDescription("Remove colored mask from image");
    desc.getPropertySet().propSetString(kOfxPropIcon, "com.tallerneg.FilmNegInvert.png", 1, false);

    desc.addSupportedContext(OFX::eContextFilter);
    desc.addSupportedContext(OFX::eContextGeneral);

    desc.addSupportedBitDepth(OFX::eBitDepthUByte);
    desc.addSupportedBitDepth(OFX::eBitDepthUShort);
    desc.addSupportedBitDepth(OFX::eBitDepthFloat);

    desc.setSingleInstance(false);
    desc.setHostFrameThreading(false);
    desc.setSupportsMultiResolution(true);
    desc.setSupportsTiles(true);
    desc.setTemporalClipAccess(false);

    desc.setRenderTwiceAlways(false);
    desc.setSupportsMultipleClipPARs(false);
    desc.setRenderThreadSafety(OFX::eRenderFullySafe);
}

// ===========================================
// 5. PARAMETER DESCRIPTOR
// ===========================================

void FilmNegInvertPluginFactory::describeInContext(OFX::ImageEffectDescriptor& desc, OFX::ContextEnum context)
{
    OFX::ClipDescriptor* srcClip = desc.defineClip("Source");
    srcClip->addSupportedComponent(OFX::ePixelComponentRGB);
    srcClip->addSupportedComponent(OFX::ePixelComponentRGBA);
    srcClip->setTemporalClipAccess(false);
    srcClip->setSupportsTiles(true);
    srcClip->setIsMask(false);

    OFX::ClipDescriptor* dstClip = desc.defineClip("Output");
    dstClip->addSupportedComponent(OFX::ePixelComponentRGB);
    dstClip->addSupportedComponent(OFX::ePixelComponentRGBA);
    dstClip->setSupportsTiles(true);

    OFX::PageParamDescriptor* page = desc.definePageParam("Main");

    {
        OFX::RGBParamDescriptor* param = desc.defineRGBParam(kParamMaskColor);
        param->setLabel("Mask Color");
        param->setHint("Film base color.");
        param->setDefault(0.8, 0.5, 0.3);
        param->setRange(0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
        param->setDisplayRange(0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
        param->setAnimates(true);
        page->addChild(*param);
    }

    {
        OFX::DoubleParamDescriptor* param = desc.defineDoubleParam(kParamGamma);
        param->setLabel("Film Gamma");
        param->setHint("Film response curve");
        param->setDefault(0.6);
        param->setRange(0.1, 2.0);
        param->setDisplayRange(0.4, 1.0);
        param->setAnimates(true);
        page->addChild(*param);
    }

    {
        OFX::DoubleParamDescriptor* param = desc.defineDoubleParam(kParamExposure);
        param->setLabel("Exposure");
        param->setHint("Linear exposure adjustment");
        param->setDefault(1.0);
        param->setRange(0.0, 10.0);
        param->setDisplayRange(0.0, 5.0);
        param->setAnimates(true);
        page->addChild(*param);
    }

    {
        OFX::ChoiceParamDescriptor* param = desc.defineChoiceParam(kParamInputType);
        param->setLabel("Input Type");
        param->setHint("Select Linear for scene-linear scans or Log for Cineon/DPX style scans.");
        param->appendOption(kParamInputTypeLinear);
        param->appendOption(kParamInputTypeLog);
        param->setDefault(0);
        param->setAnimates(false);
        page->addChild(*param);
    }

    {
        OFX::ChoiceParamDescriptor* param = desc.defineChoiceParam(kParamFilmType);
        param->setLabel("Film Type");
        param->setHint("Applies a practical preset for the selected stock family.");
        param->appendOption(kParamFilmTypeVision3);
        param->appendOption(kParamFilmTypeDoubleX);
        param->setDefault(0);
        param->setAnimates(false);
        page->addChild(*param);
    }

    {
        OFX::DoubleParamDescriptor* param = desc.defineDoubleParam(kParamRedFactor);
        param->setLabel("Red Factor");
        param->setHint("Red channel contrast factor");
        param->setDefault(1.0);
        param->setRange(0.0, 300.0);
        param->setDisplayRange(0.0, 10.0);
        param->setAnimates(true);
        page->addChild(*param);
    }
    {
        OFX::DoubleParamDescriptor* param = desc.defineDoubleParam(kParamGreenFactor);
        param->setLabel("Green Factor");
        param->setHint("Green channel contrast factor");
        param->setDefault(1.0);
        param->setRange(0.0, 300.0);
        param->setDisplayRange(0.0, 10.0);
        param->setAnimates(true);
        page->addChild(*param);
    }
    {
        OFX::DoubleParamDescriptor* param = desc.defineDoubleParam(kParamBlueFactor);
        param->setLabel("Blue Factor");
        param->setHint("Blue channel contrast factor");
        param->setDefault(1.0);
        param->setRange(0.0, 300.0);
        param->setDisplayRange(0.0, 10.0);
        param->setAnimates(true);
        page->addChild(*param);
    }

    {
        OFX::BooleanParamDescriptor* param = desc.defineBooleanParam(kParamColorInvert);
        param->setLabel("Color Invert");
        param->setHint("Invert colors after filter");
        param->setDefault(false);
        param->setAnimates(true);
        page->addChild(*param);
    }

    {
        OFX::PushButtonParamDescriptor* param = desc.definePushButtonParam(kParamReset);
        param->setLabel("Reset");
        param->setHint("Restore the plugin controls to the startup defaults.");
        page->addChild(*param);
    }
}

// ===========================================
// 6. INSTANCE CREATION
// ===========================================

OFX::ImageEffect* FilmNegInvertPluginFactory::createInstance(OfxImageEffectHandle handle, OFX::ContextEnum context)
{
    return new FilmNegInvertPlugin(handle);
}

} // namespace FilmNegInvert
