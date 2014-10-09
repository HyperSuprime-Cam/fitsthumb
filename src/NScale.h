#ifndef  gdb323b31_2c86_45f0_a87f_0ca7eb604ca6
#define  gdb323b31_2c86_45f0_a87f_0ca7eb604ca6

/**
    We define a mapping
        y = a * log(x + b) + c   (1)
    where x is the value of a pixel in the input image,
    and y in the output image.

    Let mean and sigma be the sky level and the sky noise respectively.
    We let the mapping curve (1) pass through the three point:
        (xMin, yMin), (xMid, yMid), (xMax, yMax)
    where
        xMin = mean - T1 * sigma
        xMid = mean
        xMax = mean + T2 * sigma

        yMin = 0
        yMid = S
        yMax = 1

    T1, T2, and S are the parameters of this mapping.
    We call them the "low threshold", the "high threshold"
    and the (output) "sky level", respectively.

    The curve (1) is convex upward, and therefore
        (yMid - yMin) / (xMid - xMin) > (yMax - yMin) / (xMax - xMin)

    vix. the three parameters must satisfy
        S*(1 + T2/T1) > 1.

    Let us name the mapping NScale after the glorious ZScale,
    in the hope that it will be a mighty alternative.
*/

#include "Statistics.h"
#include "LogScale.h"

namespace hsc { namespace fitsthumb {

template <class Tfrom>
LogScale
NScale(
    Image<Tfrom> const& image,
    double              threshLo = 3,
    double              threshHi = 1000,
    double              skyLevel = 0.1
){
    Statistics stat = GetStatistics(image);
    if(stat.stddev > 0){
        return LogScale(
            // from
            stat.mean - threshLo * stat.stddev,
            stat.mean,
            stat.mean + threshHi * stat.stddev,
            // to
            0, skyLevel, 1
        );
    }

    // on error

    MinMax<Tfrom> minmax = GetMinMax(image);

    return LogScale(
        // from
        minmax.min, minmax.max,
        // to
        0, 1
    );
}



}} // namespace hsc::fitsthumb
#endif //gdb323b31_2c86_45f0_a87f_0ca7eb604ca6
