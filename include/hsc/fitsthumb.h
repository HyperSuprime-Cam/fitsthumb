#ifndef  gdf056312_2c43_4e4a_b5b2_1948545d50c4
#define  gdf056312_2c43_4e4a_b5b2_1948545d50c4

#include "fitsthumb/Image.h"
#include "fitsthumb/SizeOption.h"
#include "fitsthumb/ScaleOption.h"

#include <cstdio> // to show error message "this is deprecated"

#if defined(SWIG)
    #define DEPRECATED
#elif defined(__GNUC__) || defined(__clang__)
    #define DEPRECATED  __attribute__((deprecated))
#elif defined(_MSC_VER)
    #define DEPRECATED  __declspec(deprecated)
#else
    #define DEPRECATED
#endif


namespace hsc { namespace fitsthumb {


/** Create a thumbnail image from FITS file.

    @param inputFile
        Path to the input image.
    @param outputFile
        Path to the output image.
        The file path must have a suffix either of
            ".png", ".jpg", or ".bmp"
    @param size
        Either option::RelativeSize or option::AbsoluteSize.

        If this is option::RelativeSize, it expresses the size of the
        output image, relative to the input.
        If this is option::AbsoluteSize, it expresses the absolute size
        of the output image, in pixels.

        In both cases, the size must be smaller than or equal to that of the
        input image (In RelativeSize case, width <= 1 and height <= 1).

        If either of size.width or size.height is 0, it is calculated from
        the other so that the aspect ratio is conserved.
        If size.width and size.height are both 0, resizing does not occur.
    @param scale
        Either option::LinearScale or option::LogScale.
    @param dynamicRangeFirst
        If this parameter is set true, the dynamic range is fixed before
        resizing occurs. Since fixing the dynamic range usually involves
        saturation, it will make the output dishonest. It will, however,
        create a thumbnail image that would be observed if one called this
        function with width = 0 and height = 0 (i.e. no resizing), viewed
        the output with a photo viewer, and pressed "zoom out" button.
*/
void createFitsThumb(
    char          const* inputFile ,
    char          const* outputFile,
    option::Size  const& size,
    option::Scale const& scale,
    bool                 dynamicRangeFirst = false
);

/** Create a thumbnail image from Image<T> (in python, this is numpy array)
*/
template <class T>
void createFitsThumb(
    Image<T>      const& image,
    char          const* outputFile,
    option::Size  const& size,
    option::Scale const& scale,
    bool                 dynamicRangeFirst = false
);


//
// deprecated interfaces
//

inline
DEPRECATED
void createFitsThumb(
    char const* inputFile ,
    char const* outputFile,
    int         width = 0,
    int         height = 0,
    bool        dynamicRangeFirst = false
){
    std::puts(
        "Warning: Deprecated createFitsThumb() interface."  "\n"
        "    Replace the calling expression with:"          "\n"
        "        createFitsThumb("                          "\n"
        "            inputFile,"                            "\n"
        "            outputFile,"                           "\n"
        "            option::AbsoluteSize(width, height),"  "\n"
        "            option::LinearScale(),"                "\n"
        "            dynamicRangeFirst"                     "\n"
        "        );"
    );

    createFitsThumb(
        inputFile,
        outputFile,
        option::AbsoluteSize(width, height),
        option::LinearScale(),
        dynamicRangeFirst
    );
}

template <class T>
inline
DEPRECATED
void createFitsThumb(
    Image<T> const& image,
    char     const* outputFile,
    int             width = 0,
    int             height = 0,
    bool            dynamicRangeFirst = false
){
    std::puts(
        "Warning: Deprecated createFitsThumb() interface."  "\n"
        "    Replace the calling expression with:"          "\n"
        "        createFitsThumb("                          "\n"
        "            image,"                                "\n"
        "            outputFile,"                           "\n"
        "            option::AbsoluteSize(width, height),"  "\n"
        "            option::LinearScale(),"                "\n"
        "            dynamicRangeFirst"                     "\n"
        "        );"
    );

    createFitsThumb(
        image,
        outputFile,
        option::AbsoluteSize(width, height),
        option::LinearScale(),
        dynamicRangeFirst
    );
}

}} // namespace hsc::fitsthumb
#endif //gdf056312_2c43_4e4a_b5b2_1948545d50c4
