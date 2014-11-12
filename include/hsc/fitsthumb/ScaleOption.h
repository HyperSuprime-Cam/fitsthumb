#ifndef  g36bf8853_f7de_4dc5_85df_7d40f227a211
#define  g36bf8853_f7de_4dc5_85df_7d40f227a211

#include <cmath>

/** Options used by class *Scale
*/

namespace hsc { namespace fitsthumb {

namespace option {

    struct Scale
    {
        enum ID {
            Minmax, Linear, Log
        };

        virtual ID  GetScale() const = 0;
    };

    /** Linear scale transformation with naive min/max.
        If min or max is NAN, it is obtained from the input image.
    */
    struct MinmaxScale : public Scale
    {
        virtual ID  GetScale() const { return Minmax; }

        double min;
        double max;

        MinmaxScale(double min_ = NAN, double max_ = NAN)
            : min(min_), max(max_)
        {}
    };

    /** ZScale parameters.
        P(x), the cumulative distribution of pixel values, is fit with a
        straight line. Ideally, I think, the line will be tanglent to P(x)
        at its inflection point (or, at the maximum point of the
        (non-cumulative) distribution. The slope of the line is multiplied
        by "contrast". Let x0 and x1 the points where the line intersects
        y = 0 and y = 1 respectively. Then, LinearScale(contrast) is
        equivalent to MinmaxScale(x0, x1).
    */
    struct LinearScale : public Scale
    {
        virtual ID  GetScale() const { return Linear; }

        double contrast;

        LinearScale(double contrast_ = 0.25)
            : contrast(contrast_)
        {}
    };

    /** The input pixel values are mapped logarithmically to the output.
        The logarithm curve is defined so that it passes through the three point:
            (input, output) = (mean - T1 sigma, 0), (mean, S), (mean + T2 sigma, 1)
        where mean is the average sky, sigma is the standard deviation of the sky,
        (T1, T2, S) are parameters. In the program, they are named
        (threshLo, threshHi, skyLevel) respectively,
    */
    struct LogScale : public Scale
    {
        virtual ID  GetScale() const { return Log; }

        double threshLo;
        double threshHi;
        double skyLevel;

        LogScale(double threshLo_ = 3, double threshHi_ = 100, double skyLevel_ = 0.2)
            : threshLo(threshLo_)
            , threshHi(threshHi_)
            , skyLevel(skyLevel_)
        {}
    };

} // namespace option

}} // namespace hsc::fitsthumb
#endif //g36bf8853_f7de_4dc5_85df_7d40f227a211
