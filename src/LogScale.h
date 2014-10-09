#ifndef  gfac128e5_2398_43cd_a85b_b85204077da9
#define  gfac128e5_2398_43cd_a85b_b85204077da9

#include <cmath>

namespace hsc { namespace fitsthumb {

class LogScale
{
public:
    /** Create a function output = a * log(input + b) + c
        on which specified three points will be.
    */
    LogScale(
        double inMin , double inMid , double inMax,
        double outMin, double outMid, double outMax
    );

    LogScale(
        double inMin , double inMax,
        double outMin, double outMax
    )
        : inMin_ (inMin )
        , inMax_ (inMax )
        , outMin_(outMin)
        , outMax_(outMax)
        , a_((outMax - outMin) / std::log(inMax - inMin + 1.0))
        , b_(1.0 - inMin)
        , c_(outMin     )
    {}

    double operator()(double in) const {
        if(in <= inMin_) return outMin_;
        if(inMax_ <= in) return outMax_;

        return a_ * std::log(in + b_) + c_;
    }

private:
    double inMin_;
    double inMax_;
    double outMin_;
    double outMax_;

    double a_; // output =
    double b_; //   a * log(input + b) + c
    double c_; //
};

}} // namespace hsc::fitsthumb
#endif //gfac128e5_2398_43cd_a85b_b85204077da9
