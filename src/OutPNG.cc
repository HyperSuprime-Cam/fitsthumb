#include "hsc/fitsthumb/OutPNG.h"

#include <cstdio>

#include <png.h> // libpng header
#ifdef NDEBUG
#	pragma comment(lib, "libpng.lib")
#	pragma comment(lib, "zlib.lib")
#else
#	pragma comment(lib, "libpngd.lib")
#	pragma comment(lib, "zlibd.lib")
#endif


namespace fitmb
{

void
OutPNG(
	const C2DArray<uint8>& image, const char* szFile
){
	std::vector<uint8*> vRows;
	vRows.reserve(image.height());

	// fits は下から上の順に格納されているので
	// 逆順に出力
	uint8* pRow = image.ptr() + image.width() * image.height();
	for(int i = image.height(); i > 0; --i){
		pRow -= image.width();
		vRows.push_back(pRow);
	}

	std::FILE* f = std::fopen(szFile, "wb");
	if(! f){
		throw CException("OutPNG: ", szFile, " が開けない");
	}

	png_structp ps = png_create_write_struct(
		PNG_LIBPNG_VER_STRING, NULL, NULL, NULL
	);
	if(! ps){
		throw CException("OutPNG: ", szFile,
			": png_create_write_struct 失敗");
	}

	png_infop pi = png_create_info_struct(ps);
	if(! ps){
		throw CException("OutPNG: ", szFile,
			": png_create_info_struct 失敗");
	}

	png_init_io(ps, f);

	png_set_IHDR(
		ps, pi,
		image.width(), image.height(),
		8, PNG_COLOR_TYPE_GRAY,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT
	);

	png_write_info(ps, pi);
	png_write_image(ps, (png_bytepp)&vRows[0]);
	png_write_end(ps, pi);
	png_destroy_write_struct(&ps, &pi);
	std::fclose(f);
}

} // namespace fitmb
