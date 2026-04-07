/*
══════════════════════════════════════════════════════════════════════════════
 [FILM NEG INVERT] [1.1.0] - FILMNEGINVERTPLUGINFACTORY.H
 [Plugin factory declaration and parameter identifiers.]
 [C++]
══════════════════════════════════════════════════════════════════════════════
Taller Lab - 2026
https://tallerlab.com/
VAI CORINTHIANS!!
══════════════════════════════════════════════════════════════════════════════
*/

#ifndef FILMNEGINVERTPLUGINFACTORY_H
#define FILMNEGINVERTPLUGINFACTORY_H

// ===========================================
// 1. INCLUDES
// ===========================================

#include "ofxsImageEffect.h"

// ===========================================
// 2. NAMESPACE
// ===========================================

namespace FilmNegInvert {

// ===========================================
// 3. FORWARD DECLARATIONS
// ===========================================

class FilmNegInvertPlugin;

// ===========================================
// 4. PARAMETER IDENTIFIERS
// ===========================================

static const char* kParamMaskColor = "maskColor";
static const char* kParamGamma = "gamma";
static const char* kParamExposure = "exposure";
static const char* kParamInputType = "inputType";
static const char* kParamFilmType = "filmType";
static const char* kParamRedFactor = "redFactor";
static const char* kParamGreenFactor = "greenFactor";
static const char* kParamBlueFactor = "blueFactor";
static const char* kParamColorInvert = "colorInvert";
static const char* kParamReset = "reset";

static const char* kParamInputTypeLinear = "Linear (RAW)";
static const char* kParamInputTypeLog = "Log (Cineon/DPX)";
static const char* kParamFilmTypeVision3 = "Vision3 (Color)";
static const char* kParamFilmTypeDoubleX = "Double-X (B&W)";

// ===========================================
// 5. PLUGIN FACTORY CLASS
// ===========================================

class FilmNegInvertPluginFactory : public OFX::PluginFactoryHelper<FilmNegInvertPluginFactory> {
public:
    FilmNegInvertPluginFactory();
    ~FilmNegInvertPluginFactory() = default;

    void describe(OFX::ImageEffectDescriptor& desc) override;
    void describeInContext(OFX::ImageEffectDescriptor& desc, OFX::ContextEnum context) override;

    OFX::ImageEffect* createInstance(OfxImageEffectHandle handle, OFX::ContextEnum context) override;
};

} // namespace FilmNegInvert

#endif // FILMNEGINVERTPLUGINFACTORY_H
