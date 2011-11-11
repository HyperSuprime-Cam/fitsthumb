#include "Output.h"
#include "OutBMP.h"
#include "OutJPG.h"
#include "OutPNG.h"

#include <string>

namespace fitmb
{

void
Output(
	const C2DArray<uint8>& image,
	const char* szOutType,
	const char* szFile,
	bool bAppendExtension
){
	std::string sFile = szFile;
	std::string sType = szOutType;

	if(sType == "bmp"){
		if(bAppendExtension) sFile += ".bmp";
		OutBMP(image, sFile.c_str());
	}
	else if(sType == "jpg"){
		if(bAppendExtension) sFile += ".jpg";
		OutJPG(image, sFile.c_str());
	}
	else if(sType == "png"){
		if(bAppendExtension) sFile += ".png";
		OutPNG(image, sFile.c_str());
	}
	else{
		throw CException(sType, ": サポートされていない形式");
	}
}

} // namespace fitmb
