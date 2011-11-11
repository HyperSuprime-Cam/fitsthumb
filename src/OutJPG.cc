#include "OutJPG.h"

#include <cstdio>

#include <jpeglib.h> // libjpeg header
#ifdef NDEBUG
#	pragma comment(lib, "jpeg.lib")
#else
#	pragma comment(lib, "jpegd.lib")
#endif

namespace fitmb
{

void
OutJPG(
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

	// JPG 準備
	jpeg_error_mgr        jerr;
	jpeg_compress_struct  jcom;
	jcom.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&jcom);

	std::FILE* f = std::fopen(szFile, "wb");
	if(! f){
		throw CException("OutJPG: ", szFile, " が開けない");
	}

	jpeg_stdio_dest(&jcom, f);

	jcom.image_width  = image.width();
	jcom.image_height = image.height();
	jcom.input_components = 1; /* 色数 */
	jcom.in_color_space = JCS_GRAYSCALE;
	jpeg_set_defaults(&jcom);

	jpeg_set_quality(&jcom, 80 /*0 - 100*/, true);

	// 圧縮開始
	jpeg_start_compress(&jcom, true);
	jpeg_write_scanlines(
		&jcom, (JSAMPARRAY)&vRows[0], image.height()
	);
	jpeg_finish_compress(&jcom);

	// 終了
	jpeg_destroy_compress(&jcom);
	std::fclose(f);
}

} // namespace fitmb
