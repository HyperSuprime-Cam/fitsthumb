#ifndef fitmb_OutBMP_hpp__
#define fitmb_OutBMP_hpp__

#include "2DArray.h"
#include "ToUInt8.h"
#include "Common.h"


namespace fitmb
{

void OutBMP(const C2DArray<uint8>& image, const char* szFile);

template <class T>
void OutBMP(const C2DArray<T>& image, const char* szFile)
{
	OutBMP(*ToUInt8(image), szFile);
}

} // namespace fitmb
#endif //fitmb_OutBMP_hpp__
