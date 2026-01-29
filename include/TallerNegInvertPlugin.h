#ifndef TALLER_NEG_INVERT_PLUGIN_H
#define TALLER_NEG_INVERT_PLUGIN_H

#include "ofxsImageEffect.h"

class TallerNegInvertPlugin : public OFX::ImageEffect {
public:
  TallerNegInvertPlugin(OfxImageEffectHandle handle);
  virtual ~TallerNegInvertPlugin();

  // The main rendering function
  virtual void render(const OFX::RenderArguments &args) override;

  // Optional: Identify if the effect is currently doing nothing (identity)
  virtual bool isIdentity(const OFX::IsIdentityArguments &args,
                          OFX::Clip *&identityClip,
                          double &identityTime) override;

  // Handle parameter changes
  virtual void changedParam(const OFX::InstanceChangedArgs &args,
                            const std::string &paramName) override;

private:
  // Parameters
  OFX::RGBParam *mMaskColor;
  OFX::DoubleParam *mGamma;
  OFX::DoubleParam *mExposure;
  OFX::ChoiceParam *mInputType;
  OFX::ChoiceParam *mFilmType;
};

#endif // TALLER_NEG_INVERT_PLUGIN_H
