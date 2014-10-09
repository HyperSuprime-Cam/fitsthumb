#include "hsc/fitsthumb.h"
#include "InFITS.h"
#include "Output.h"
#include "Resize.h"

// for linear scale
#include "ZScale.h"

// for log scale
#include "NScale.h"
#include "sRGBScale.h"

#include <cstdio>
#include <string>


namespace hsc { namespace fitsthumb {

namespace {

    template <class TElem, class TFunc>
    Image<TElem> Apply(TFunc const& func, Image<TElem>& image)
    {
        if(image.Width() == image.Stride()){
            for(std::size_t i = 0; i < image.size(); ++i){
                image.data()[i] = func(image.data()[i]);
            }
        }
        else{
            for(std::size_t y = 0; y < image.Height(); ++y){
                TElem* it  = image[y];
                TElem* end = it + image.Width();

                for(; it != end; ++it){
                    *it = func(*it);
                }
            }
        }

        return image;
    }


    template <class TDest, class TSrc, class TFunc>
    Image<TDest> Apply(TFunc const& func, Image<TSrc> const& image)
    {
        Image<TDest> dest(image.Height(), image.Width());

        if(image.Width() == image.Stride()){
            for(std::size_t i = 0; i < image.size(); ++i){
                dest.data()[i] = func(image.data()[i]);
            }
        }
        else{
            TDest* itD = dest.data();

            for(std::size_t y = 0; y < image.Height(); ++y){
                TSrc const* it  = image[y];
                TSrc const* end = it + image.Width();

                for(; it != end; ++it){
                    *itD = func(*it);
                    ++itD;
                }
            }
        }

        return dest;
    }


    char const* PathFindExtension(char const* path)
    {
        char const* basename = path;
        std::size_t i = 0;
        for(; path[i]; ++i){
            if(path[i] == '/') basename = path;
        }

        char const* ext = path + i;

        for(i = 1; basename[i]; ++i){
            if(basename[i] == '.') ext = basename + i;
        }

        return ext;
    }

} // anonymous namespace


void createFitsThumb(
    char const* inputFile ,
    char const* outputFile,
    int         width ,
    int         height,
    bool        dynamicRangeFirst
){
    std::string output = outputFile;
    if(output == "bmp"
    || output == "png"
    || output == "jpg"
    ){
        // output = basename(inputFile) + "." + output
        output.insert(output.begin(), '.');
        output.insert(output.begin(),
            inputFile, PathFindExtension(inputFile)
        );
    }

    createFitsThumb(
        InFITS<float>(inputFile),
        output.c_str(),
        width,
        height,
        dynamicRangeFirst
    );
}


template <class T>
void createFitsThumb(
    Image<T> const& image,
    char     const *outputFile,
    int             width,
    int             height,
    bool            dynamicRangeFirst
){
    char const* outputType = PathFindExtension(outputFile);
    if(outputType[0] == char()){
        throw std::runtime_error(
            MSG("Cannot infer file type from path extension: " << outputFile));
    }
    ++outputType; // skip '.'

    Image<double> dest;

    if(width <= 0 && height <= 0){
        // no resize
        dest = Apply<double>(ZScale(image), image);
    }
    else if(dynamicRangeFirst){
        // dynamic range change first, then resize
        dest = Apply<double>(ZScale(image), image);

        // do resize
        dest = ResizeDown<double>(dest, width, height);
    }
    else {
        // resize first, then change dynamic range
        dest = ResizeDown<double>(image, width, height);

        // change dynamic range
        dest = Apply<double>(ZScale(dest), dest);
    }

    // output file
    Output(
        dest,
        outputType,
        outputFile
    );
}


template
void createFitsThumb(
    Image<float> const& image,
    char         const *outputFile,
    int                 width,
    int                 height,
    bool                dynamicRangeFirst
);

template
void createFitsThumb(
    Image<double> const& image,
    char          const *outputFile,
    int                  width,
    int                  height,
    bool                 dynamicRangeFirst
);


//---------------------------------------------------------//

void createLogFitsThumb(
    char const* inputFile ,
    char const* outputFile,
    int         width ,
    int         height,
    bool        dynamicRangeFirst
){
    std::string output = outputFile;
    if(output == "bmp"
    || output == "png"
    || output == "jpg"
    ){
        // output = basename(inputFile) + "." + output
        output.insert(output.begin(), '.');
        output.insert(output.begin(),
            inputFile, PathFindExtension(inputFile)
        );
    }

    createLogFitsThumb(
        InFITS<float>(inputFile),
        output.c_str(),
        width,
        height,
        dynamicRangeFirst
    );
}


template <class T>
void createLogFitsThumb(
    Image<T> const& image,
    char     const *outputFile,
    int             width,
    int             height,
    bool            dynamicRangeFirst
){
    char const* outputType = PathFindExtension(outputFile);
    if(outputType[0] == char()){
        throw std::runtime_error(
            MSG("Cannot infer file type from path extension: " << outputFile));
    }
    ++outputType; // skip '.'

    Image<double> dest;

    if(width <= 0 && height <= 0){
        // no resize
        dest = Apply<double>(NScale(image), image);
    }
    else if(dynamicRangeFirst){
        // dynamic range change first, then resize
        dest = Apply<double>(NScale(image), image);

        // do resize
        dest = ResizeDown<double>(dest, width, height);
    }
    else {
        // resize first, then change dynamic range
        dest = ResizeDown<double>(image, width, height);

        // change dynamic range
        dest = Apply<double>(NScale(dest), dest);
    }

    // output file

    dest = Apply<double>(sRGBScale(), dest);

    Output(
        dest,
        outputType,
        outputFile
    );
}


template
void createLogFitsThumb(
    Image<float> const& image,
    char         const *outputFile,
    int                 width,
    int                 height,
    bool                dynamicRangeFirst
);

template
void createLogFitsThumb(
    Image<double> const& image,
    char          const *outputFile,
    int                  width,
    int                  height,
    bool                 dynamicRangeFirst
);


}} // namespace hsc::fitsthumb
