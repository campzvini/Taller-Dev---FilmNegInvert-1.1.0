/*
══════════════════════════════════════════════════════════════════════════════
 FILM NEG INVERT 1.0 - FILMNEGINVERTPLUGIN.H
 Header principal do plugin Film Neg Invert para inversão de negativos de filme
 C++ Header
══════════════════════════════════════════════════════════════════════════════
Taller Dev - 2026
VAI CORINTHIANS!!
══════════════════════════════════════════════════════════════════════════════
*/

#ifndef FILMNEGINVERTPLUGIN_H
#define FILMNEGINVERTPLUGIN_H

// ============================================
// 1. INCLUDES # Bibliotecas necessárias
// ============================================
#include "ofxsImageEffect.h"
#include "ofxsMultiThread.h"
#include <memory>
#include <vector>

// ============================================
// 2. FORWARD DECLARATIONS # Declarações antecipadas
// ============================================
namespace FilmNegInvert {
    class FilmNegInvertPluginFactory;
}

// ============================================
// 3. CLASS DEFINITION # Definição da classe
// ============================================
namespace FilmNegInvert {

/**
 * @brief Classe principal do plugin Film Neg Invert
 * 
 * Esta classe é responsável por:
 * - Processar imagens de negativos de filme
 * - Remover a máscara laranja (D-min)
 * - Aplicar correção de gama e contraste
 * - Inverter as cores do negativo
 */
class FilmNegInvertPlugin : public OFX::ImageEffect {
public:
    // ============================================
    // 3.1. CONSTRUCTOR # Construtor
    // ============================================
    /**
     * @brief Construtor do plugin
     * @param handle Handle OFX para a instância
     */
    FilmNegInvertPlugin(OfxImageEffectHandle handle);

    // ============================================
    // 3.2. DESTRUCTOR # Destrutor
    // ============================================
    /**
     * @brief Destrutor virtual
     */
    ~FilmNegInvertPlugin() override = default;

    // ============================================
    // 3.3. RENDER # Renderização
    // ============================================
    /**
     * @brief Renderiza o efeito
     * @param args Argumentos de renderização
     */
    void render(const OFX::RenderArguments& args) override;

    // ============================================
    // 3.4. CHANGED PARAM # Mudança de parâmetro
    // ============================================
    /**
     * @brief Chamado quando um parâmetro é alterado
     * @param args Argumentos da mudança de parâmetro
     */
    void changedParam(const OFX::InstanceChangedArgs& args, const std::string& paramName) override;

    // ============================================
    // 3.5. IS IDENTITY # Verificação de identidade
    // ============================================
    /**
     * @brief Verifica se o efeito é identidade (pass-through)
     * @param args Argumentos de identidade
     * @param identityClip Clip de identidade de saída
     * @param identityTime Tempo de identidade de saída
     * @return true se o efeito é identidade, false caso contrário
     */
    bool isIdentity(const OFX::IsIdentityArguments& args, OFX::Clip*& identityClip, double& identityTime) override;

private:
    // ============================================
    // 3.6. HELPER METHODS # Métodos auxiliares
    // ============================================
    
    /**
     * @brief Processa a imagem de negativo
     * @param srcPtr Ponteiro para imagem de origem
     * @param dstPtr Ponteiro para imagem de destino
     * @param srcRowBytes Bytes por linha da origem
     * @param dstRowBytes Bytes por linha do destino
     * @param renderWindow Janela de renderização
     * @param pixelDepth Profundidade do pixel
     * @param nComponents Número de componentes por pixel
     */
    void processImage(const void* srcPtr, void* dstPtr, int srcRowBytes, int dstRowBytes,
                      const OfxRectI& renderWindow, OFX::BitDepthEnum pixelDepth, int nComponents);

    /**
     * @brief Analisa a imagem para encontrar a cor da máscara
     * @param args Argumentos de renderização
     */
    void analyzeMaskColor(const OFX::RenderArguments& args);

    /**
     * @brief Converte RGB para YUV
     * @param r Componente vermelho
     * @param g Componente verde
     * @param b Componente azul
     * @param y Componente luminância (saída)
     * @param u Componente crominância U (saída)
     * @param v Componente crominância V (saída)
     */
    static void rgbToYuv(double r, double g, double b, double& y, double& u, double& v);

    /**
     * @brief Converte YUV para RGB
     * @param y Componente luminância
     * @param u Componente crominância U
     * @param v Componente crominância V
     * @param r Componente vermelho (saída)
     * @param g Componente verde (saída)
     * @param b Componente azul (saída)
     */
    static void yuvToRgb(double y, double u, double v, double& r, double& g, double& b);

    // ============================================
    // 3.7. CLIPS # Clips do plugin
    // ============================================
    OFX::Clip* _srcClip;  ///< Clip de entrada
    OFX::Clip* _dstClip;  ///< Clip de saída

    // ============================================
    // 3.8. PARAMETERS # Parâmetros do plugin
    // ============================================
    OFX::ChoiceParam* _paramAlgorithm;      ///< Algoritmo de processamento
    OFX::RGBParam* _paramMaskColor;          ///< Cor da máscara
    OFX::DoubleParam* _paramGamma;           ///< Gama do filme
    OFX::DoubleParam* _paramExposure;        ///< Exposição
    OFX::ChoiceParam* _paramInputType;       ///< Tipo de entrada
    OFX::ChoiceParam* _paramFilmType;        ///< Tipo de filme
    OFX::DoubleParam* _paramRedFactor;       ///< Fator de contraste vermelho
    OFX::DoubleParam* _paramGreenFactor;     ///< Fator de contraste verde
    OFX::DoubleParam* _paramBlueFactor;      ///< Fator de contraste azul
    OFX::BooleanParam* _paramColorInvert;    ///< Inversão de cor
};

} // namespace FilmNegInvert

#endif // FILMNEGINVERTPLUGIN_H
