#ifndef  g9bc8e1e9_87fb_43df_a34a_ca65bc139457
#define  g9bc8e1e9_87fb_43df_a34a_ca65bc139457

#include "hsc/fitsthumb/Image.h"
#include "Common.h"

namespace hsc { namespace fitsthumb {

/** Convert an image of floats (value range [0, 1])
    to an image of bytes (value range [0, 255])
*/
template <class TFrom>
Image<uint8_t>
ToUInt8(
    Image<TFrom> const& image
){
    Image<uint8_t> dest(image.Height(), image.Width());
    uint8_t* itDest = dest.data();

    for(std::size_t y = 0; y < image.Height(); ++y){
        TFrom const*       itSrc  = image[y];
        TFrom const* const endSrc = itSrc + image.Width();

        for(; itSrc != endSrc; ++itSrc){
            TFrom val = TFrom(255.0) * (*itSrc);
            uint8_t c;
            if(!(val > 0)){ /* val <= 0 or isnan(val)*/
                c = 0;
            }
            else if(255 <= val){
                c = 255;
            }
            else{
                c = static_cast<uint8_t>(val);
            }

            *itDest = c;
            ++itDest;
        }
    }

    return dest;
}


}} // namespace hsc::fitsthumb
#endif //g9bc8e1e9_87fb_43df_a34a_ca65bc139457
