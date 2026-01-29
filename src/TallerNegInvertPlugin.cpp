#include "TallerNegInvertPlugin.h"
#include "cuda_bridge.h"
#include "ofxsProcessing.h"

TallerNegInvertPlugin::TallerNegInvertPlugin(OfxImageEffectHandle handle)
    : OFX::ImageEffect(handle) {
  // Fetch parameters
  mMaskColor = fetchRGBParam("MaskColor");
  mGamma = fetchDoubleParam("Gamma");
  mExposure = fetchDoubleParam("Exposure");
  mInputType = fetchChoiceParam("InputType");
  mFilmType = fetchChoiceParam("FilmType");
}

TallerNegInvertPlugin::~TallerNegInvertPlugin() {}

void TallerNegInvertPlugin::changedParam(const OFX::InstanceChangedArgs &args,
                                         const std::string &paramName) {
  // If parameters change, we might need to trigger a redraw, but OFX handles
  // this mostly. We can handle specific logic here if needed.
}

void TallerNegInvertPlugin::render(const OFX::RenderArguments &args) {
  // Check if CUDA is enabled by the host
  int cudaEnabled = 0;
  try {
    cudaEnabled = getPropertySet().propGetInt("OfxImageEffectPropCudaEnabled");
  } catch (...) {
    // Property might not exist if not in Resolve or not CUDA
  }

  if (!cudaEnabled) {
    setPersistentMessage(OFX::Message::eMessageError, "",
                         "CUDA not enabled or supported by host.");
    return;
  }

  // Get CUDA Stream
  void *streamPtr = nullptr;
  try {
    streamPtr = getPropertySet().propGetPointer("OfxImageEffectPropCudaStream");
  } catch (...) {
    setPersistentMessage(OFX::Message::eMessageError, "",
                         "Could not retrieve CUDA stream.");
    return;
  }
  cudaStream_t stream = (cudaStream_t)streamPtr;

  // Get Clips
  OFX::Clip *srcClip = fetchClip(kOfxImageEffectSimpleSourceClipName);
  OFX::Clip *dstClip = fetchClip(kOfxImageEffectOutputClipName);

  if (!srcClip || !dstClip)
    return;

  // Fetch Images
  std::auto_ptr<OFX::Image> srcImg(srcClip->fetchImage(args.time));
  std::auto_ptr<OFX::Image> dstImg(dstClip->fetchImage(args.time));

  if (!srcImg.get() || !dstImg.get())
    return;

  // Verify Bit Depth (Float only for now)
  if (srcImg->getPixelDepth() != OFX::eBitDepthFloat ||
      dstImg->getPixelDepth() != OFX::eBitDepthFloat) {
    setPersistentMessage(OFX::Message::eMessageError, "",
                         "Only Float32 bit depth is supported.");
    return;
  }

  // Prepare Parameters for Kernel
  InvertParams params;
  params.width = srcImg->getBounds().x2 - srcImg->getBounds().x1;
  params.height = srcImg->getBounds().y2 - srcImg->getBounds().y1;

  double r, g, b;
  mMaskColor->getValueAtTime(args.time, r, g, b);
  params.maskColor[0] = (float)r;
  params.maskColor[1] = (float)g;
  params.maskColor[2] = (float)b;

  double gamma;
  mGamma->getValueAtTime(args.time, gamma);
  params.gammaInv = 1.0f / (float)gamma;

  double exposure;
  mExposure->getValueAtTime(args.time, exposure);
  params.exposure = (float)exposure;

  int inputType;
  mInputType->getValueAtTime(args.time, inputType);
  params.inputType = inputType;

  int filmType;
  mFilmType->getValueAtTime(args.time, filmType);
  params.filmType = filmType;

  // Get Pointers (These are Device Pointers in Resolve when CUDA is on)
  void *srcPtr = srcImg->getPixelData();
  void *dstPtr = dstImg->getPixelData();

  // Call Bridge
  runCudaInversion(srcPtr, dstPtr, params, stream);
}

bool TallerNegInvertPlugin::isIdentity(const OFX::IsIdentityArguments &args,
                                       OFX::Clip *&identityClip,
                                       double &identityTime) {
  // We are not identity anymore, we do processing
  return false;
}
