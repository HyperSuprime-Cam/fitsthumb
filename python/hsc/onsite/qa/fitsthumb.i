%module fitsthumb
%{
#include "ZScale.h"
#include "InFITS.h"
#include "Output.h"
#include "Resize.h"
#include "CommandLine.h"
#include "FitsThumbFuncs.h"
#include <cstdio>
%}

//defined in image.i? %define %imagePtr(NAME, TYPE, PIXEL_TYPE...)
//SWIG_SHARED_PTR_DERIVED(NAME##TYPE##Base, lsst::daf::data::LsstBase, lsst::afw::image::ImageBase<PIXEL_TYPE>);
//defined in image.i?  SWIG_SHARED_PTR_DERIVED(NAME##TYPE, lsst::afw::image::ImageBase<PIXEL_TYPE>, lsst::afw::image::Image<PIXEL_TYPE>);
//SWIG_SHARED_PTR(Decorated##NAME##TYPE, lsst::afw::image::DecoratedImage<PIXEL_TYPE>);
//%declareNumPyConverters(lsst::afw::image::ImageBase<PIXEL_TYPE>::Array);
//%declareNumPyConverters(lsst::afw::image::ImageBase<PIXEL_TYPE>::ConstArray);
// %enddef


%include "FitsThumbFuncs.h"
//namespace hsc {
//namespace onsite {
//namespace qa {
//   int createFitsThumb(char *input, char *output, char *outtype, float width, float height, bool dynamicRangeFirst);
//}
//}
//}

//%imagePtr(Image, U, boost::uint16_t);
//%imagePtr(Image, I, int);
//defined in image.i? %imagePtr(Image, F, float);
//%imagePtr(Image, D, double);


