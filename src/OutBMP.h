#ifndef  g43547885_163e_4032_bd04_9f6e4ab9c979
#define  g43547885_163e_4032_bd04_9f6e4ab9c979

#include "hsc/fitsthumb/Image.h"
#include "ToUInt8.h"
#include "Common.h"


namespace hsc { namespace fitsthumb {

void OutBMP(Image<uint8_t> const& image, char const* szFile);

template <class T>
void OutBMP(Image<T> const& image, char const* szFile)
{
    OutBMP(ToUInt8(image), szFile);
}

}} // namespace hsc::fitsthumb
#endif //g43547885_163e_4032_bd04_9f6e4ab9c979
