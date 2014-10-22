#ifndef  g50b59c64_46ed_4150_8c3d_bc16653ae93b
#define  g50b59c64_46ed_4150_8c3d_bc16653ae93b

#include <cmath>

namespace hsc { namespace fitsthumb {

/** from L* in CIE (L*, a*, b*)
    to Y in (X,Y,Z)
*/
struct LabScale
{
    double operator()(double L) const {
        double const delta = 6.0 / 29;
        double const _3dd  = 3 * delta*delta;

        double f = (L + 0.16) * (1.0 / 1.16);

        if(f <= delta){
            return _3dd * (f - 0.16/1.16);
        }
        else{
            return f*f*f;
        }
    }
};

}} // namespace hsc::fitsthumb
#endif //g50b59c64_46ed_4150_8c3d_bc16653ae93b
