#ifndef fitsThumbFuncs_hpp__
#define fitsThumbFuncs_hpp__
#endif

namespace  hsc {
    namespace  onsite {
        namespace qa {
//   int createFitsThumb(const char *inputFile, const char * outputFile, const char* outputType = "png", int width = 0, int height = 0, bool dynamicRangeFirst = true); 
            int createFitsThumb(const char *inputFile, const char * outputFile, const char* outputType, int width, int height, bool dynamicRangeFirst); 
        }
    }
}
