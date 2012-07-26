#include "hsc/fitsthumb/OutJPG.h"

#include <cstdio>

#include <jpeglib.h> // libjpeg header
#ifdef NDEBUG
#   pragma comment(lib, "jpeg.lib")
#else
#   pragma comment(lib, "jpegd.lib")
#endif

#include <stdexcept>


namespace fitmb
{

void
OutJPG(
    const C2DArray<uint8>& image, const char* szFile
){
    std::vector<uint8*> vRows;
    vRows.reserve(image.height());

    // Images are stored upside down in FITS,
    // so output inversely
    uint8* pRow = image.ptr() + image.width() * image.height();
    for(int i = image.height(); i > 0; --i){
        pRow -= image.width();
        vRows.push_back(pRow);
    }

    // prepare JPG
    jpeg_error_mgr        jerr;
    jpeg_compress_struct  jcom;
    jcom.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&jcom);

    File f(std::fopen(szFile, "wb"));
    if(! f.get()){
        throw std::runtime_error(MSG("OutJPG: " << szFile << ": cannot be created"));
    }

    jpeg_stdio_dest(&jcom, f.get());

    jcom.image_width  = image.width();
    jcom.image_height = image.height();
    jcom.input_components = 1; /* number of colors */
    jcom.in_color_space = JCS_GRAYSCALE;
    jpeg_set_defaults(&jcom);

    jpeg_set_quality(&jcom, 80 /*0 - 100*/, true);

    // begin compression
    jpeg_start_compress(&jcom, true);
    jpeg_write_scanlines(
        &jcom, (JSAMPARRAY)&vRows[0], image.height()
    );
    jpeg_finish_compress(&jcom);

    // end
    jpeg_destroy_compress(&jcom);
}

} // namespace fitmb
