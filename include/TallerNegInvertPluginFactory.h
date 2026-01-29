#ifndef TALLER_NEG_INVERT_PLUGIN_FACTORY_H
#define TALLER_NEG_INVERT_PLUGIN_FACTORY_H

#include "ofxsImageEffect.h"

// The Factory class describes the plugin to the host
class TallerNegInvertPluginFactory : public OFX::PluginFactoryHelper<TallerNegInvertPluginFactory> {
public:
    TallerNegInvertPluginFactory();
    virtual void describe(OFX::ImageEffectDescriptor &desc) override;
    virtual void describeInContext(OFX::ImageEffectDescriptor &desc, OFX::ContextEnum context) override;
    virtual OFX::ImageEffect* createInstance(OfxImageEffectHandle handle, OFX::ContextEnum context) override;
};

#endif // TALLER_NEG_INVERT_PLUGIN_FACTORY_H
