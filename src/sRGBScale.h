#ifndef  g44510409_ec3d_45b4_9c79_be1ca26c00f2
#define  g44510409_ec3d_45b4_9c79_be1ca26c00f2

#include <cmath>

namespace hsc { namespace fitsthumb {

/** from linear rgb to sRGB value
*/
struct sRGBScale
{
    double operator()(double in) const {
        if(in <= 0.0031308){
            return 12.92 * in;
        }
        else{
            return (1 + 0.055) * std::pow(in, 1.0/2.4) - 0.055;
        }
    }
};

}} // namespace hsc::fitsthumb
#endif //g44510409_ec3d_45b4_9c79_be1ca26c00f2
