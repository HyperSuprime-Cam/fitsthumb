#ifndef  g92249e19_ecdf_4296_919f_b05275b741c9
#define  g92249e19_ecdf_4296_919f_b05275b741c9

#include "hsc/fitsthumb/Image.h"
#include "ToUInt8.h"
#include "Common.h"


namespace hsc { namespace fitsthumb {

void OutPNG(Image<uint8_t> const& image, char const* szFile);

template <class T>
void OutPNG(Image<T> const& image, char const* szFile)
{
    OutPNG(ToUInt8(image), szFile);
}

}} // namespace hsc::fitsthumb
#endif //g92249e19_ecdf_4296_919f_b05275b741c9
