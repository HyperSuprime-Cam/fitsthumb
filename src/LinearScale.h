#ifndef  g8180d88d_2168_428a_b62b_bbbfc066b77b
#define  g8180d88d_2168_428a_b62b_bbbfc066b77b


namespace hsc { namespace fitsthumb {

class LinearScale
{
public:
    /** Create a function output = a * input + b
        on which specified three points will be.
    */
    LinearScale(
        double inMin ,  double inMax,
        double outMin,  double outMax
    )
        : inMin_ (inMin )
        , inMax_ (inMax )
        , outMin_(outMin)
        , outMax_(outMax)
        , a_     ((outMax - outMin) / (inMax - inMin))
    {}

    double operator()(double in) const {
        if(in <= inMin_) return outMin_;
        if(inMax_ <= in) return outMax_;

        return a_ * (in - inMin_) + outMin_;
    }

private:
    double inMin_;
    double inMax_;
    double outMin_;
    double outMax_;

    double a_;
};

}} // namespace hsc::fitsthumb
#endif //g8180d88d_2168_428a_b62b_bbbfc066b77b
