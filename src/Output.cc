#include "Output.h"
#include "OutBMP.h"
#include "OutJPG.h"
#include "OutPNG.h"

#include "Common.h"

#include <string>
#include <stdexcept>

namespace hsc { namespace fitsthumb {

void
Output(
    Image<uint8_t> const& image,
    char           const* szOutType,
    char           const* szFile
){
    std::string sFile = szFile;
    std::string sType = szOutType;

    if(sType == "bmp"){
        OutBMP(image, sFile.c_str());
    }
    else if(sType == "jpg"){
        OutJPG(image, sFile.c_str());
    }
    else if(sType == "png"){
        OutPNG(image, sFile.c_str());
    }
    else{
        throw std::runtime_error(MSG(sType << ": unsupported format"));
    }
}

}} // namespace hsc::fitsthumb
