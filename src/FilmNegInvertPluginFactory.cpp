/*
══════════════════════════════════════════════════════════════════════════════
 FILM NEG INVERT 1.0 - FILMNEGINVERTPLUGINFACTORY.CPP
 Implementação da factory para criação de instâncias do plugin Film Neg Invert
 C++ Source
══════════════════════════════════════════════════════════════════════════════
Taller Dev - 2026
VAI CORINTHIANS!!
══════════════════════════════════════════════════════════════════════════════
*/

// ============================================
// 1. INCLUDES # Bibliotecas necessárias
// ============================================
#include "FilmNegInvertPluginFactory.h"
#include "FilmNegInvertPlugin.h"
#include "ofxsImageEffect.h"
#include "ofxsMultiThread.h"

// ============================================
// 2. NAMESPACE # Namespace do projeto
// ============================================
namespace FilmNegInvert {

// ============================================
// 3. CONSTRUCTOR IMPLEMENTATION # Implementação do construtor
// ============================================
FilmNegInvertPluginFactory::FilmNegInvertPluginFactory()
    : OFX::PluginFactoryHelper<FilmNegInvertPluginFactory>("com.tallerneg.FilmNegInvert", 1, 0) {
}

// ============================================
// 4. DESCRIBE IMPLEMENTATION # Implementação da descrição do plugin
// ============================================
void FilmNegInvertPluginFactory::describe(OFX::ImageEffectDescriptor& desc) {
    // ============================================
    // 4.1. BASIC PROPERTIES # Propriedades básicas
    // ============================================
    desc.setLabel("Film Neg Invert");
    desc.setPluginGrouping("Color");
    desc.setPluginDescription("Remove colored mask from image");

    // ============================================
    // 4.2. CONTEXTS SUPPORT # Suporte a contextos
    // ============================================
    desc.addSupportedContext(OFX::eContextFilter);
    desc.addSupportedContext(OFX::eContextGeneral);

    // ============================================
    // 4.3. PIXEL DEPTHS # Profundidades de pixel suportadas
    // ============================================
    desc.addSupportedBitDepth(OFX::eBitDepthUByte);
    desc.addSupportedBitDepth(OFX::eBitDepthUShort);
    desc.addSupportedBitDepth(OFX::eBitDepthFloat);

    // ============================================
    // 4.4. TILES SUPPORT # Suporte a tiles
    // ============================================
    desc.setSingleInstance(false);
    desc.setHostFrameThreading(false);
    desc.setSupportsMultiResolution(true);
    desc.setSupportsTiles(false);
    desc.setTemporalClipAccess(false);

    // ============================================
    // 4.5. RENDER THREAD SAFETY # Segurança de threads de renderização
    // ============================================
    desc.setRenderTwiceAlways(false);
    desc.setSupportsMultipleClipPARs(false);
    desc.setRenderThreadSafety(OFX::eRenderFullySafe);
}

// ============================================
// 5. DESCRIBE IN CONTEXT IMPLEMENTATION # Implementação da descrição no contexto
// ============================================
void FilmNegInvertPluginFactory::describeInContext(OFX::ImageEffectDescriptor& desc, OFX::ContextEnum context) {
    // ============================================
    // 5.1. SOURCE CLIP # Clip de entrada (Source)
    // ============================================
    OFX::ClipDescriptor* srcClip = desc.defineClip("Source");
    srcClip->addSupportedComponent(OFX::ePixelComponentRGB);
    srcClip->addSupportedComponent(OFX::ePixelComponentRGBA);
    srcClip->setTemporalClipAccess(false);
    srcClip->setSupportsTiles(false);
    srcClip->setIsMask(false);

    // ============================================
    // 5.2. OUTPUT CLIP # Clip de saída (Output)
    // ============================================
    OFX::ClipDescriptor* dstClip = desc.defineClip("Output");
    dstClip->addSupportedComponent(OFX::ePixelComponentRGB);
    dstClip->addSupportedComponent(OFX::ePixelComponentRGBA);
    dstClip->setSupportsTiles(false);

    // ============================================
    // 5.3. PAGE SETUP # Configuração de página da UI
    // ============================================
    OFX::PageParamDescriptor* page = desc.definePageParam("Main");

    // ============================================
    // 5.4. ALGORITHM PARAMETER # Parâmetro de algoritmo
    // ============================================
    {
        OFX::ChoiceParamDescriptor* param = desc.defineChoiceParam(kParamAlgorithm);
        param->setLabel("Algorithm");
        param->setHint("Colored mask removal algorithm");
        param->appendOption(kParamAlgorithmRGBReduction);
        param->setDefault(0); // RGB Reduction
        param->setAnimates(false);
        page->addChild(*param);
    }

    // ============================================
    // 5.5. MASK COLOR PARAMETER # Parâmetro de cor da máscara
    // ============================================
    {
        OFX::RGBParamDescriptor* param = desc.defineRGBParam(kParamMaskColor);
        param->setLabel("Mask Color");
        param->setHint("Pick film base color (between perforations)");
        param->setDefault(0.8, 0.5, 0.3); // Máscara laranja típica
        param->setRange(0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
        param->setDisplayRange(0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
        param->setAnimates(true);
        page->addChild(*param);
    }

    // ============================================
    // 5.6. GAMMA PARAMETER # Parâmetro de gama
    // ============================================
    {
        OFX::DoubleParamDescriptor* param = desc.defineDoubleParam(kParamGamma);
        param->setLabel("Film Gamma");
        param->setHint("Film response curve");
        param->setDefault(0.6); // Vision3
        param->setRange(0.1, 2.0);
        param->setDisplayRange(0.4, 1.0);
        param->setAnimates(true);
        page->addChild(*param);
    }

    // ============================================
    // 5.7. EXPOSURE PARAMETER # Parâmetro de exposição
    // ============================================
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

    // ============================================
    // 5.8. INPUT TYPE PARAMETER # Parâmetro de tipo de entrada
    // ============================================
    {
        OFX::ChoiceParamDescriptor* param = desc.defineChoiceParam(kParamInputType);
        param->setLabel("Input Type");
        param->setHint("Linear (RAW) or Log (Cineon/DPX)");
        param->appendOption(kParamInputTypeLinear);
        param->appendOption(kParamInputTypeLog);
        param->setDefault(0); // Linear (RAW)
        param->setAnimates(false);
        page->addChild(*param);
    }

    // ============================================
    // 5.9. FILM TYPE PARAMETER # Parâmetro de tipo de filme
    // ============================================
    {
        OFX::ChoiceParamDescriptor* param = desc.defineChoiceParam(kParamFilmType);
        param->setLabel("Film Type");
        param->setHint("Film stock preset");
        param->appendOption(kParamFilmTypeVision3);
        param->appendOption(kParamFilmTypeDoubleX);
        param->setDefault(0); // Vision3 (Color)
        param->setAnimates(false);
        page->addChild(*param);
    }

    // ============================================
    // 5.10. CONTRAST FACTORS PARAMETERS # Parâmetros de fatores de contraste
    // ============================================
    {
        // Red Factor
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
        // Green Factor
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
        // Blue Factor
        OFX::DoubleParamDescriptor* param = desc.defineDoubleParam(kParamBlueFactor);
        param->setLabel("Blue Factor");
        param->setHint("Blue channel contrast factor");
        param->setDefault(1.0);
        param->setRange(0.0, 300.0);
        param->setDisplayRange(0.0, 10.0);
        param->setAnimates(true);
        page->addChild(*param);
    }

    // ============================================
    // 5.11. COLOR INVERT PARAMETER # Parâmetro de inversão de cor
    // ============================================
    {
        OFX::BooleanParamDescriptor* param = desc.defineBooleanParam(kParamColorInvert);
        param->setLabel("Color Invert");
        param->setHint("Invert colors after filter");
        param->setDefault(false);
        param->setAnimates(true);
        page->addChild(*param);
    }

    // ============================================
    // 5.12. ANALYZE BUTTON PARAMETER # Botão de análise
    // ============================================
    {
        OFX::PushButtonParamDescriptor* param = desc.definePushButtonParam(kParamAnalyze);
        param->setLabel("Analyze Mask");
        param->setHint("Analyze image to find mask color");
        page->addChild(*param);
    }

    // ============================================
    // 5.13. HELP BUTTON PARAMETER # Botão de ajuda
    // ============================================
    {
        OFX::PushButtonParamDescriptor* param = desc.definePushButtonParam(kParamHelp);
        param->setLabel("Help");
        param->setHint("Show help information");
        page->addChild(*param);
    }
}

// ============================================
// 6. CREATE INSTANCE IMPLEMENTATION # Implementação da criação de instância
// ============================================
OFX::ImageEffect* FilmNegInvertPluginFactory::createInstance(OfxImageEffectHandle handle, OFX::ContextEnum context) {
    // Cria uma nova instância do plugin
    return new FilmNegInvertPlugin(handle);
}

} // namespace FilmNegInvert
