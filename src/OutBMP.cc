#include "hsc/fitsthumb/OutBMP.h"

#include <cstdio>
#include <stdexcept>

namespace fitmb
{


#pragma pack(push, 1)
struct SBitMapFileHeader
{
	uint16   bfType;
	uint32   bfSize;
	uint16   bfReserved1;
	uint16   bfReserved2;
	uint32   bfOffBits;
};

struct SBitMapInfoHeader
{
	uint32   biSize;
	sint32   biWidth;
	sint32   biHeight;
	uint16   biPlanes;
	uint16   biBitCount;
	uint32   biCompression;
	uint32   biSizeImage;
	sint32   biXPelsPerMeter;
	sint32   biYPelsPerMeter;
	uint32   biClrUsed;
	uint32   biClrImportant;
};
#pragma pack(pop)

//___________________________________________________________________________
void
OutBMP(
	const C2DArray<uint8>& image,
	const char*            szFile
){
	SBitMapFileHeader fh = {};
	SBitMapInfoHeader ih = {};

	ih.biSize  = sizeof(ih);
	ih.biWidth = image.width();
	ih.biHeight = image.height();
	ih.biPlanes = 1;
	ih.biBitCount = 8;

	uint32 palette[256];
	for(uint32 i = 0; i < 256; ++i){
		palette[i] = i | (i << 8) | (i << 16);
	}

	sint32 cbLine = (image.width() + 3) & ~(uint32)3;

	fh.bfType = (uint16)'B' | ((uint16)'M' << 8);
	fh.bfOffBits = sizeof(fh) + sizeof(ih) + sizeof(palette);
	fh.bfSize = fh.bfOffBits + cbLine * image.height();

	File f(std::fopen(szFile, "wb"));
	if(! f.get()){
		throw std::runtime_error(MSG("OutBMP: " << szFile << ": cannot be created"));
	}

	sint32 cbPad = cbLine - image.width();

#define BMP_FWRITE(data, size, count, file) \
	if((size_t)count != std::fwrite(data, size, count, file)){ \
		throw std::runtime_error(MSG("OutBMP: " << szFile << ": Error in writing")); \
	}

    BMP_FWRITE(&fh    , sizeof(fh),   1, f.get());
    BMP_FWRITE(&ih    , sizeof(ih),   1, f.get());
    BMP_FWRITE(palette, 4         , 256, f.get());

    // fits は bmp と同じく下から上の順に格納されているので
    // 他のように逆順に出力しなくていい
    uint8* p = image.ptr();
    for(int i = image.height(); i > 0; --i){
        BMP_FWRITE(p, 1, image.width(), f.get());
        if(cbPad > 0){ uint32 pad = 0; BMP_FWRITE(&pad, 1, cbPad, f.get()); }
        p += image.width();
    }
}


} // namespace fitmb
