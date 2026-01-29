/*
══════════════════════════════════════════════════════════════════════════════
 FILM NEG INVERT 1.0 - FILMNEGINVERTPLUGINFACTORY.H
 Factory class para criação de instâncias do plugin Film Neg Invert
 C++ Header
══════════════════════════════════════════════════════════════════════════════
Taller Dev - 2026
VAI CORINTHIANS!!
══════════════════════════════════════════════════════════════════════════════
*/

#ifndef FILMNEGINVERTPLUGINFACTORY_H
#define FILMNEGINVERTPLUGINFACTORY_H

// ============================================
// 1. INCLUDES # Bibliotecas necessárias
// ============================================
#include "ofxsImageEffect.h"

// ============================================
// 2. FORWARD DECLARATIONS # Declarações antecipadas
// ============================================
namespace FilmNegInvert {
    class FilmNegInvertPlugin;
}

// ============================================
// 2.1. PARAMETER NAMES # Nomes dos parâmetros
// ============================================
namespace FilmNegInvert {
    // Nomes dos parâmetros para uso interno
    static const char* kParamAlgorithm = "algorithm";
    static const char* kParamMaskColor = "maskColor";
    static const char* kParamGamma = "gamma";
    static const char* kParamExposure = "exposure";
    static const char* kParamInputType = "inputType";
    static const char* kParamFilmType = "filmType";
    static const char* kParamRedFactor = "redFactor";
    static const char* kParamGreenFactor = "greenFactor";
    static const char* kParamBlueFactor = "blueFactor";
    static const char* kParamColorInvert = "colorInvert";
    static const char* kParamAnalyze = "analyze";
    static const char* kParamHelp = "help";

    // Valores para parâmetros Choice
    static const char* kParamAlgorithmRGBReduction = "RGB Reduction";
    static const char* kParamInputTypeLinear = "Linear (RAW)";
    static const char* kParamInputTypeLog = "Log (Cineon/DPX)";
    static const char* kParamFilmTypeVision3 = "Vision3 (Color)";
    static const char* kParamFilmTypeDoubleX = "Double-X (B&W)";
}

// ============================================
// 3. CLASS DEFINITION # Definição da classe
// ============================================
namespace FilmNegInvert {

/**
 * @brief Factory para criação de instâncias do plugin Film Neg Invert
 * 
 * Esta classe é responsável por:
 * - Descrever o plugin para o host OFX
 * - Definir os parâmetros e clips disponíveis
 * - Criar instâncias do plugin
 */
class FilmNegInvertPluginFactory : public OFX::PluginFactoryHelper<FilmNegInvertPluginFactory> {
public:
    // ============================================
    // 3.1. CONSTRUCTOR # Construtor
    // ============================================
    /**
     * @brief Construtor da factory
     */
    FilmNegInvertPluginFactory();

    // ============================================
    // 3.2. DESTRUCTOR # Destrutor
    // ============================================
    /**
     * @brief Destrutor virtual
     */
    ~FilmNegInvertPluginFactory() = default;

    // ============================================
    // 3.3. PLUGIN DESCRIPTION # Descrição do plugin
    // ============================================
    /**
     * @brief Descreve o plugin para o host OFX
     * @param desc Descriptor do efeito a ser preenchido
     */
    void describe(OFX::ImageEffectDescriptor& desc) override;

    /**
     * @brief Descreve o contexto do plugin (parâmetros e clips)
     * @param desc Descriptor do efeito a ser preenchido
     * @param context Contexto do plugin (Filter, General, etc.)
     */
    void describeInContext(OFX::ImageEffectDescriptor& desc, OFX::ContextEnum context) override;

    // ============================================
    // 3.4. INSTANCE CREATION # Criação de instância
    // ============================================
    /**
     * @brief Cria uma nova instância do plugin
     * @param handle Handle OFX para a instância
     * @param context Contexto do plugin
     * @return Ponteiro para a nova instância do plugin
     */
    OFX::ImageEffect* createInstance(OfxImageEffectHandle handle, OFX::ContextEnum context) override;
};

} // namespace FilmNegInvert

#endif // FILMNEGINVERTPLUGINFACTORY_H
