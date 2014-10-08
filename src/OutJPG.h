#ifndef  g4b01e304_e8b5_43ae_b57b_02c62ac6b7d2
#define  g4b01e304_e8b5_43ae_b57b_02c62ac6b7d2

#include "hsc/fitsthumb/Image.h"
#include "ToUInt8.h"
#include "Common.h"


namespace hsc { namespace fitsthumb {

void OutJPG(Image<uint8_t> const& image, char const* szFile);

template <class T>
void OutJPG(Image<T> const& image, char const* szFile)
{
    OutJPG(ToUInt8(image), szFile);
}

}} // namespace hsc::fitsthumb
#endif //g4b01e304_e8b5_43ae_b57b_02c62ac6b7d2
