#ifndef  gdf056312_2c43_4e4a_b5b2_1948545d50c4
#define  gdf056312_2c43_4e4a_b5b2_1948545d50c4

#include "fitsthumb/Image.h"

namespace hsc { namespace fitsthumb {

void createFitsThumb(
    char const* inputFile ,
    char const* outputFile,
    int         width = 0,
    int         height = 0,
    bool        dynamicRangeFirst = true
);

template <class T>
void createFitsThumb(
    Image<T> const& image,
    char     const* outputFile,
    int             width = 0,
    int             height = 0,
    bool            dynamicRangeFirst = true
);

void createLogFitsThumb(
    char const* inputFile ,
    char const* outputFile,
    int         width = 0,
    int         height = 0,
    bool        dynamicRangeFirst = true
);

template <class T>
void createLogFitsThumb(
    Image<T> const& image,
    char     const* outputFile,
    int             width = 0,
    int             height = 0,
    bool            dynamicRangeFirst = true
);

}} // namespace hsc::fitsthumb
#endif //gdf056312_2c43_4e4a_b5b2_1948545d50c4
