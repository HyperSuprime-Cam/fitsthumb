#include "LogScale.h"
#include "Common.h"
#include <stdexcept>


namespace hsc { namespace fitsthumb {

namespace {

    double findAddenda_f(double b, double s, double M)
    {
        double Ib = 1.0 / b;

        return (1-s) * std::log(1.0 - Ib) + s * std::log(1 + M*Ib);
    }


    double findAddenda(double s, double M)
    {
        // find b such that
        // (b - 1)^(1-s) (b + M)^s = b
        //

        // Integer type that has the same size as double:
        // This program assumes that if a < b then (intdbl_t&)a < (intdbl_t&)b
        typedef size_to_type<sizeof(double), true>::type  intdbl_t;

        double right = M / (s*(M + 1) - 1);
        double f_right = findAddenda_f(right, s, M);

        if( !(right > 1)
        ||  !std::isfinite(right)
        ||  !(f_right > 0)
        ){
            throw std::domain_error("sky * (hiThresh/loThresh + 1) must be > 1");
        }

        double left = 0.5*(right + 1.0);
        double f_left;
        while(f_left = findAddenda_f(left, s, M), f_left > 0){
            left = 0.5*(left + 1.0);
        }

        while(1 < reinterpret_cast<intdbl_t&>(right)
            - reinterpret_cast<intdbl_t&>(left)
        ){
            double middle = 0.5 * (left + right);
            double f_middle = findAddenda_f(middle, s, M);

            if(f_middle * f_left > 0){
                left   = middle;
                f_left = f_middle;
            }
            else{
                right   = middle;
                f_right = f_middle;
            }
        }

        return 0.5 * (left + right);
    }
} // anonymous namespace


LogScale::LogScale(
    double inMin , double inMid , double inMax,
    double outMin, double outMid, double outMax
)
    : inMin_ (inMin )
    , inMax_ (inMax )
    , outMin_(outMin)
    , outMax_(outMax)
{
    double const M = (inMax - inMid) / (inMid - inMin);
    double const s = (outMid - outMin) / (outMax - outMin);

    double const bPrime = findAddenda(s, M);

    this->a_ = (outMid - outMin) / std::log(bPrime / (bPrime - 1.0));
    this->b_ = bPrime * (inMid - inMin) - inMid;
    this->c_ = outMin - this->a_ * std::log(inMin + this->b_);
}

}} // namespace hsc::fitsthumb
