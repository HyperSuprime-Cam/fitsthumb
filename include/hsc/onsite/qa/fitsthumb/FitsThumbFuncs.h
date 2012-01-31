//#ifndef fitsThumbFuncs_hpp__
//#define fitsThumbFuncs_hpp__

#include "lsst/afw/image/MaskedImage.h"

namespace hsc {
    namespace  onsite {
        namespace qa {
            namespace fitsthumb {
                
                int createFitsThumb(const char *inputFile, const char * outputFile, const char* outputType, int width, int height, bool dynamicRangeFirst); 
                int createFitsThumb(lsst::afw::image::Image<float>::Ptr &image, const char * outputFile, const char* outputType, int width, int height, bool dynamicRangeFirst); 

                fitmb::Ptr< fitmb::C2DArray<float> >  convertImageToC2dArray(lsst::afw::image::Image<float>::Ptr &image);
            }
        }
    }
}

//#endif