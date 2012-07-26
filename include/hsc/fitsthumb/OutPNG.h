#ifndef fitmb_OutPNG_hpp__
#define fitmb_OutPNG_hpp__

#include "2DArray.h"
#include "ToUInt8.h"
#include "Common.h"


namespace fitmb
{

void OutPNG(const C2DArray<uint8>& image, const char* szFile);

template <class T>
void OutPNG(const C2DArray<T>& image, const char* szFile)
{
    OutPNG(*ToUInt8(image), szFile);
}

} // namespace fitmb
#endif //fitmb_OutPNG_hpp__
