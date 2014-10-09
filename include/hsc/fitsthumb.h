#ifndef  gdf056312_2c43_4e4a_b5b2_1948545d50c4
#define  gdf056312_2c43_4e4a_b5b2_1948545d50c4

#include "fitsthumb/Image.h"

namespace hsc { namespace fitsthumb {

/** Create a thumbnail image from FITS file.
    ZScale with contrast = 0.25.
    (i.e. the dynamic range is +-5 sigma, sigma = sky noise)

    @param inputFile
        Path to the input image.
    @param outputFile
        Path to the output image.
        The file path must have a suffix either of
            ".png", ".jpg", or ".bmp"
    @param width
        Width of the output image, in pixels.
        Must be smaller than or equal to that of the input image.
        If this parameter is 0, it is calculated from the height
        so that the aspect ratio is conserved.
        If both width and height are 0, resizing does not occur.
    @param height
        Height of the output image, in pixels.
        Must be smaller than or equal to that of the input image.
        If this parameter is 0, it is calculated from the width
        so that the aspect ratio is conserved.
        If both width and height are 0, resizing does not occur.
    @param dynamicRangeFirst
        If this parameter is set true, the dynamic range is fixed before
        resizing occurs. Since fixing the dynamic range usually involves
        saturation, it will make the output dishonest. It will, however,
        create a thumbnail image that would be observed if one called this
        function with width = 0 and height = 0 (i.e. no resizing), viewed
        the output with a photo viewer, and pressed "zoom out" button.
*/
void createFitsThumb(
    char const* inputFile ,
    char const* outputFile,
    int         width = 0,
    int         height = 0,
    bool        dynamicRangeFirst = false
);

/** Create a thumbnail image from Image<T> (in python, this is numpy array)
*/
template <class T>
void createFitsThumb(
    Image<T> const& image,
    char     const* outputFile,
    int             width = 0,
    int             height = 0,
    bool            dynamicRangeFirst = false
);

/** Create a thumbnail image from FITS file.
    The input pixel values are mapped logarithmically to the output.
    The logarithm curve is defined so that it passes through the three point:
        (input, output) = (mean - T1 sigma, 0), (mean, S), (mean + T2 sigma, 1)
    where mean is the average sky, sigma is the standard deviation of the sky,
    (T1, S, T2) are parameters that are arbitrary but are here fixed:
    (T1, S, T2) = (-3, 0.1, 1000).
*/
void createLogFitsThumb(
    char const* inputFile ,
    char const* outputFile,
    int         width = 0,
    int         height = 0,
    bool        dynamicRangeFirst = false
);

/** Create a thumbnail image from Image<T> (in python, this is numpy array)
*/
template <class T>
void createLogFitsThumb(
    Image<T> const& image,
    char     const* outputFile,
    int             width = 0,
    int             height = 0,
    bool            dynamicRangeFirst = false
);

}} // namespace hsc::fitsthumb
#endif //gdf056312_2c43_4e4a_b5b2_1948545d50c4
