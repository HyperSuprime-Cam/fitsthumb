#include "hsc/fitsthumb.h"
#include "InFITS.h"
#include "Output.h"
#include "Resize.h"

// for minmax scale
#include "MinmaxScale.h"

// for linear scale
#include "ZScale.h"

// for log scale
#include "NScale.h"

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


    template <class ScaleCreator, class T>
    Image<double>
    CreateThumbnail_Impl(
        Image<T>      const& image,
        option::Size  const& size,
        option::Scale const& option,
        bool                 dynamicRangeFirst
    ){
        typename ScaleCreator::Option const& castedOption
            = static_cast<typename ScaleCreator::Option const&>(option);

        option::AbsoluteSize absSize
            = size.ComputeActualSize(image.Width(), image.Height());

        if(absSize.width  == (int)image.Width()
        && absSize.height == (int)image.Height()
        ){
            // no resize
            return Apply<double>(ScaleCreator::Create(image, castedOption), image);
        }
        else if(dynamicRangeFirst){
            // change dynamic range first
            Image<double> dest = Apply<double>(ScaleCreator::Create(image, castedOption), image);

            // then resize
            return ResizeDown<double>(dest, absSize);
        }
        else{
            // resize first
            Image<double> dest = ResizeDown<double>(image, absSize);

            // then change dynamic range
            return Apply<double>(ScaleCreator::Create(dest, castedOption), dest);
        }
    }

} // anonymous namespace


void createThumbnail(
    char          const* inputFile ,
    char          const* outputFile,
    option::Size  const& size,
    option::Scale const& scale,
    bool                 dynamicRangeFirst
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

    createThumbnail(
        InFITS<float>(inputFile),
        output.c_str(),
        size,
        scale,
        dynamicRangeFirst
    );
}


template <class T>
void createThumbnail(
    Image<T>      const& image,
    char          const *outputFile,
    option::Size  const& size,
    option::Scale const& scale,
    bool                 dynamicRangeFirst
){
    //
    // Get output file type
    //
    char const* outputType = PathFindExtension(outputFile);
    if(outputType[0] == char()){
        throw std::runtime_error(
            MSG("Cannot infer file type from path extension: " << outputFile));
    }
    ++outputType; // skip '.'

    //
    // Make thumbnail image
    //
    Image<double> dest;
    switch(scale.GetScale()){
    case option::Scale::Minmax:
        dest = CreateThumbnail_Impl<MinmaxScale>(
            image, size, scale, dynamicRangeFirst
        );
        break;
    case option::Scale::Linear:
        dest = CreateThumbnail_Impl<ZScale>(
            image, size, scale, dynamicRangeFirst
        );
        break;
    case option::Scale::Log:
        dest = CreateThumbnail_Impl<NScale>(
            image, size, scale, dynamicRangeFirst
        );
        break;
    }

    //
    // Output file
    //
    Output(dest, outputType, outputFile);
}


template
void createThumbnail(
    Image<float>  const& image,
    char          const *outputFile,
    option::Size  const& size,
    option::Scale const& scale,
    bool                 dynamicRangeFirst
);

template
void createThumbnail(
    Image<double> const& image,
    char          const *outputFile,
    option::Size  const& size,
    option::Scale const& scale,
    bool                 dynamicRangeFirst
);

}} // namespace hsc::fitsthumb
