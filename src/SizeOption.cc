#include "hsc/fitsthumb/SizeOption.h"

#include <algorithm>

namespace hsc { namespace fitsthumb {

namespace option {

AbsoluteSize
AbsoluteSize::ComputeActualSize(int srcWidth, int srcHeight) const
{
    int absW = this->width;
    int absH = this->height;

    if(absW > 0 && absH > 0) return *this;

    if(absW <= 0 && absH <= 0){
        return AbsoluteSize(srcWidth, srcHeight);
    }

    if(absW <= 0){
        absW = (std::max)(1, (int)(((int64_t)absH*srcWidth + srcHeight/2) / srcHeight));
    }
    else{
        absH = (std::max)(1, (int)(((int64_t)absW*srcHeight + srcWidth/2) / srcWidth));
    }

    return AbsoluteSize(absW, absH);
}


AbsoluteSize
RelativeSize::ComputeActualSize(int srcWidth, int srcHeight) const
{
    double relW = (this->width  > 0) ? this->width  : this->height;
    double relH = (this->height > 0) ? this->height : this->width ;

    int absW;
    int absH;
    if(relW > 0 && relH > 0){
        absW = (std::max)(1, (int)(0.5 + relW * srcWidth ));
        absH = (std::max)(1, (int)(0.5 + relH * srcHeight));
    }
    else{
        absW = srcWidth ;
        absH = srcHeight;
    }

    return AbsoluteSize(absW, absH);
}


} // namespace option
}} // namespace hsc::fitsthumb
