#ifndef  g059ba468_03e1_4ab2_8713_6ea78f16d923
#define  g059ba468_03e1_4ab2_8713_6ea78f16d923

#include "hsc/fitsthumb/Image.h"

namespace hsc { namespace fitsthumb {

struct Statistics
{
    double mean  ;
    double stddev;
};

template <class Elem>
Statistics GetStatistics(Image<Elem> const& image);


template <class T>
struct MinMax
{
    T min;
    T max;
};

template <class Elem>
MinMax<Elem> GetMinMax(Image<Elem> const& image);


}} // namespace hsc::fitsthumb
#endif //g059ba468_03e1_4ab2_8713_6ea78f16d923
