#include "Output.h"
#include "OutBMP.h"
#include "OutJPG.h"
#include "OutPNG.h"

#include "Common.h"

#include <cstring>
#include <stdexcept>

namespace hsc { namespace fitsthumb {

void
Output(
    Image<uint8_t> const& image,
    char           const* szOutType,
    char           const* szFile
){
    if(0 == std::strcmp(szOutType, "bmp")){
        OutBMP(image, szFile);
    }
    else if(0 == std::strcmp(szOutType, "jpg")){
        OutJPG(image, szFile);
    }
    else if(0 == std::strcmp(szOutType, "png")){
        OutPNG(image, szFile);
    }
    else{
        throw std::runtime_error(MSG(szOutType << ": unsupported format"));
    }
}

}} // namespace hsc::fitsthumb
