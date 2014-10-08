#include "OutPNG.h"

#include <cstdio>
#include <vector>
#include <stdexcept>

#include <png.h> // libpng header
#ifdef NDEBUG
#   pragma comment(lib, "libpng.lib")
#   pragma comment(lib, "zlib.lib")
#else
#   pragma comment(lib, "libpngd.lib")
#   pragma comment(lib, "zlibd.lib")
#endif


namespace hsc { namespace fitsthumb {


namespace {
    class FileCloser
    {
    public:
        explicit FileCloser(std::FILE* f): f_(f) {}
        void operator()() const { std::fclose(f_); }
    private:
        std::FILE* f_;
    };
}


void
OutPNG(
    Image<uint8_t> const& image, char const* szFile
){
    std::vector<uint8_t const*> vRows;
    vRows.reserve(image.Height());

    // Images are stored upside down in FITS,
    // so output inversely
    for(std::size_t y = image.Height(); y > 0; --y){
        vRows.push_back(image[y-1]);
    }

    FILE* f = std::fopen(szFile, "wb");
    if(!f){
        throw std::runtime_error(MSG("OutPNG: " << szFile << ": cannot be created"));
    }
    Destructor const& fileCloser = MakeDestructor(FileCloser(f));

    png_structp ps = png_create_write_struct(
        PNG_LIBPNG_VER_STRING, NULL, NULL, NULL
    );
    if(!ps){
        throw std::runtime_error(MSG("OutPNG: " << szFile <<
            ": png_create_write_struct failed"));
    }

    png_infop pi = png_create_info_struct(ps);
    if(! pi){
        png_destroy_write_struct(&ps, NULL);
        throw std::runtime_error(MSG("OutPNG: " << szFile <<
            ": png_create_info_struct failed"));
    }

    if (setjmp(png_jmpbuf(ps))){
        png_destroy_write_struct(&ps, &pi);
        throw std::runtime_error(MSG("OutPNG: " << szFile <<
            ": some error in writing PNG file"));
    }

    png_init_io(ps, f);

    png_set_IHDR(
        ps, pi,
        image.Width(), image.Height(),
        8, PNG_COLOR_TYPE_GRAY,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );

    png_write_info(ps, pi);
    png_write_image(ps, (png_bytepp)vRows.data());
    png_write_end(ps, pi);
    png_destroy_write_struct(&ps, &pi);
}

}} // namespace hsc::fitsthumb
