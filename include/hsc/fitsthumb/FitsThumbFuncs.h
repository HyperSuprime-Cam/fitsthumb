//#ifndef fitsThumbFuncs_hpp__
//#define fitsThumbFuncs_hpp__

#include "lsst/afw.h"

namespace hsc {
namespace fitsthumb {

int createFitsThumb(const char *inputFile, const char * outputFile, const char* outputType = "png", int width = 0, int height = 0, bool dynamicRangeFirst = true);
int createFitsThumb(lsst::afw::image::Image<float>::Ptr &image, const char * outputFile, const char* outputType = "png", int width = 0, int height = 0, bool dynamicRangeFirst = true);

fitmb::Ptr< fitmb::C2DArray<float> >  convertImageToC2dArray(lsst::afw::image::Image<float>::Ptr &image);

}}

//#endif
