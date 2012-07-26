#ifndef fitmb_OutJPG_hpp__
#define fitmb_OutJPG_hpp__

#include "2DArray.h"
#include "ToUInt8.h"
#include "Common.h"


namespace fitmb
{

void OutJPG(const C2DArray<uint8>& image, const char* szFile);

template <class T>
void OutJPG(const C2DArray<T>& image, const char* szFile)
{
    OutJPG(*ToUInt8(image), szFile);
}

} // namespace fitmb
#endif //fitmb_OutJPG_hpp__
