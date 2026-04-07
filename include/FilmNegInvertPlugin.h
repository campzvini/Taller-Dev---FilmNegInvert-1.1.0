/*
══════════════════════════════════════════════════════════════════════════════
 [FILM NEG INVERT] [1.1.0] - FILMNEGINVERTPLUGIN.H
 [Main plugin class declaration.]
 [C++]
══════════════════════════════════════════════════════════════════════════════
Taller Lab - 2026
https://tallerlab.com/
VAI CORINTHIANS!!
══════════════════════════════════════════════════════════════════════════════
*/

#ifndef FILMNEGINVERTPLUGIN_H
#define FILMNEGINVERTPLUGIN_H

// ===========================================
// 1. INCLUDES
// ===========================================

#include "ofxsImageEffect.h"
#include <string>

// ===========================================
// 2. NAMESPACE
// ===========================================

namespace FilmNegInvert {

// ===========================================
// 3. FORWARD DECLARATIONS
// ===========================================

class FilmNegInvertPluginFactory;

// ===========================================
// 4. PLUGIN CLASS
// ===========================================

class FilmNegInvertPlugin : public OFX::ImageEffect {
public:
    explicit FilmNegInvertPlugin(OfxImageEffectHandle handle);
    ~FilmNegInvertPlugin() override = default;

    void render(const OFX::RenderArguments& args) override;
    void changedParam(const OFX::InstanceChangedArgs& args, const std::string& paramName) override;
    bool isIdentity(const OFX::IsIdentityArguments& args, OFX::Clip*& identityClip, double& identityTime) override;

private:
    void applyFilmTypePreset(int filmType);
    void resetToDefaults();

    OFX::Clip* _srcClip;
    OFX::Clip* _dstClip;

    OFX::RGBParam* _paramMaskColor;
    OFX::DoubleParam* _paramGamma;
    OFX::DoubleParam* _paramExposure;
    OFX::ChoiceParam* _paramInputType;
    OFX::ChoiceParam* _paramFilmType;
    OFX::DoubleParam* _paramRedFactor;
    OFX::DoubleParam* _paramGreenFactor;
    OFX::DoubleParam* _paramBlueFactor;
    OFX::BooleanParam* _paramColorInvert;
};

} // namespace FilmNegInvert

#endif // FILMNEGINVERTPLUGIN_H
