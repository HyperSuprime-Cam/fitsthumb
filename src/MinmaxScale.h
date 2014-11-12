#ifndef  gf4af779b_9d19_484d_9c7a_1e219ef7de9e
#define  gf4af779b_9d19_484d_9c7a_1e219ef7de9e

#include "Statistics.h"
#include "LinearScale.h"
#include "hsc/fitsthumb/ScaleOption.h"


namespace hsc { namespace fitsthumb {


/** MinmaxScale creator
    "*Scale" creator classes have:
    -- typedef ... Scale;
    -- typedef ... Option;
    -- static Scale Create(image, option);
*/
struct MinmaxScale
{
    typedef LinearScale         Scale;
    typedef option::MinmaxScale Option;

    template <class Tfrom>
    static Scale
    Create(
        Image<Tfrom> const& image,
        Option       const& option
    ){
        double min = option.min;
        double max = option.max;

        if(min != min || max != max){
            MinMax<Tfrom> minmax = GetMinMax(image);
            if(min != min){
                min = minmax.min;
            }
            if(max != max){
                max = minmax.max;
            }
        }

        return LinearScale(
            // from
            min, max,
            // to
            0, 1
        );
    }
};

}} // namespace hsc::fitsthumb

#endif //gf4af779b_9d19_484d_9c7a_1e219ef7de9e
