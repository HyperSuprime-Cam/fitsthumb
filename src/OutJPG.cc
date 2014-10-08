#include "OutJPG.h"

#include <cstdio>
#include <vector>

#include <jpeglib.h> // libjpeg header
#ifdef NDEBUG
#   pragma comment(lib, "jpeg.lib")
#else
#   pragma comment(lib, "jpegd.lib")
#endif

#include <stdexcept>
#include <csetjmp>

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


    struct ErrorManager
        : public jpeg_error_mgr
    {
        std::jmp_buf  jumpLabel;
        char errorMsg[JMSG_LENGTH_MAX];

        static void ErrorExit(j_common_ptr cp)
        {
            ErrorManager* This = static_cast<ErrorManager*>(cp->err);

            This->format_message(cp, This->errorMsg);
            std::longjmp(This->jumpLabel, 1);
        }
    };
}

void
OutJPG(
    Image<uint8_t> const& image,
    char           const* szFile
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
        throw std::runtime_error(MSG("OutJPG: " << szFile << ": cannot be created"));
    }
    Destructor const& fileCloser = MakeDestructor(FileCloser(f));

    // prepare JPG
    ErrorManager          jerr;
    jpeg_compress_struct  jcom;
    jcom.err = jpeg_std_error(&jerr);
    jerr.error_exit = ErrorManager::ErrorExit;
    if(setjmp(jerr.jumpLabel)){
        // jumped into because error has ocurred during compression
        jpeg_destroy_compress(&jcom);
        throw std::runtime_error(MSG("OutJPG: " << szFile << ": " << jerr.errorMsg));
    }

    jpeg_create_compress(&jcom);
    jpeg_stdio_dest(&jcom, f);

    jcom.image_width  = image.Width();
    jcom.image_height = image.Height();
    jcom.input_components = 1; /* number of colors */
    jcom.in_color_space = JCS_GRAYSCALE;
    jpeg_set_defaults(&jcom);

    jpeg_set_quality(&jcom, 80 /*0 - 100*/, static_cast<boolean>(true));

    // begin compression
    jpeg_start_compress(&jcom, static_cast<boolean>(true));
    jpeg_write_scanlines(
        &jcom, (JSAMPARRAY)vRows.data(), image.Height()
    );
    jpeg_finish_compress(&jcom);

    // end
    jpeg_destroy_compress(&jcom);
}

}} // namespace hsc::fitsthumb
