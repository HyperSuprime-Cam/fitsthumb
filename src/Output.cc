#include "hsc/fitsthumb/Output.h"
#include "hsc/fitsthumb/OutBMP.h"
#include "hsc/fitsthumb/OutJPG.h"
#include "hsc/fitsthumb/OutPNG.h"

#include "hsc/fitsthumb/Common.h"

#include <string>
#include <stdexcept>

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
        throw std::runtime_error(MSG(sType << ": unsupported format"));
    }
}

} // namespace fitmb
