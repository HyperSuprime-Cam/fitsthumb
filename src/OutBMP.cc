#include "OutBMP.h"

#include <cstdio>
#include <stdexcept>
#include "Common.h"

namespace hsc { namespace fitsthumb {


#pragma pack(push, 1)
struct BitMapFileHeader
{
    uint16_t   bfType;
    uint32_t   bfSize;
    uint16_t   bfReserved1;
    uint16_t   bfReserved2;
    uint32_t   bfOffBits;
};

struct BitMapInfoHeader
{
    uint32_t   biSize;
     int32_t   biWidth;
     int32_t   biHeight;
    uint16_t   biPlanes;
    uint16_t   biBitCount;
    uint32_t   biCompression;
    uint32_t   biSizeImage;
     int32_t   biXPelsPerMeter;
     int32_t   biYPelsPerMeter;
    uint32_t   biClrUsed;
    uint32_t   biClrImportant;
};
#pragma pack(pop)


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
OutBMP(
    Image<uint8_t> const& image,
    char           const* szFile
){
    BitMapFileHeader fh = {};
    BitMapInfoHeader ih = {};

    ih.biSize  = sizeof(ih);
    ih.biWidth = image.Width();
    ih.biHeight = image.Height();
    ih.biPlanes = 1;
    ih.biBitCount = 8;

    uint32_t palette[256];
    for(uint32_t i = 0; i < 256; ++i){
        palette[i] = i | (i << 8) | (i << 16);
    }

    int32_t cbLine = (image.Width() + 3) & ~(std::size_t)3;

    fh.bfType = (uint16_t)'B' | ((uint16_t)'M' << 8);
    fh.bfOffBits = sizeof(fh) + sizeof(ih) + sizeof(palette);
    fh.bfSize = fh.bfOffBits + cbLine * image.Height();

    FILE* f = std::fopen(szFile, "wb");
    if(!f){
        throw std::runtime_error(MSG("OutBMP: " << szFile << ": cannot be created"));
    }
    Destructor const& fileCloser = MakeDestructor(FileCloser(f));

    int32_t cbPad = cbLine - image.Width();

#define BMP_FWRITE(data, size, count, file) \
    if((std::size_t)count != std::fwrite(data, size, count, file)){ \
        throw std::runtime_error(MSG("OutBMP: " << szFile << ": Error in writing")); \
    }

    BMP_FWRITE(&fh    , sizeof(fh),   1, f);
    BMP_FWRITE(&ih    , sizeof(ih),   1, f);
    BMP_FWRITE(palette, 4         , 256, f);

    // FITS and BMP both store an image upside down,
    // so we don't have to care about that.
    for(std::size_t y = 0; y < image.Height(); ++y){
        BMP_FWRITE(image[y], 1, image.Width(), f);
        if(cbPad > 0){ uint32_t pad = 0; BMP_FWRITE(&pad, 1, cbPad, f); }
    }
}


}} // namespace hsc::fitsthumb
