#ifndef fitmb_Output_hpp__
#define fitmb_Output_hpp__

#include "2DArray.h"
#include "ToUInt8.h"
#include "Common.h"

namespace fitmb
{

void
Output(
    const C2DArray<uint8>& image,
    const char* szOutType,
    const char* szFile,
    bool bAppendExtension
);

template <class T>
void
Output(
    const C2DArray<T>& image,
    const char* szOutType,
    const char* szFile,
    bool bAppendExtension
){
    Output(*ToUInt8(image), szOutType, szFile, bAppendExtension);
}

} // namespace fitmb

#endif // fitmb_Output_hpp__
