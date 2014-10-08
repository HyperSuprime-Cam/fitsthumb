#ifndef  g60040e9b_dbc7_42fd_9ace_532d967690c7
#define  g60040e9b_dbc7_42fd_9ace_532d967690c7

#include "hsc/fitsthumb/Image.h"
#include "ToUInt8.h"
#include "Common.h"

namespace hsc { namespace fitsthumb {

void
Output(
    Image<uint8_t> const& image,
    char           const* szOutType,
    char           const* szFile
);

template <class T>
void
Output(
    Image<T> const& image,
    char     const* szOutType,
    char     const* szFile
){
    Output(ToUInt8(image), szOutType, szFile);
}

}} // namespace hsc::fitsthumb

#endif //g60040e9b_dbc7_42fd_9ace_532d967690c7
