#ifndef fitmb_ToUInt8_hpp__
#define fitmb_ToUInt8_hpp__

#include "2DArray.h"
#include "Common.h"


namespace fitmb
{

template <class Tfrom>
Ptr<C2DArray<uint8> >
ToUInt8(
    const C2DArray<Tfrom>& image
){
    Ptr<C2DArray<uint8> > pDest (
        new C2DArray<uint8>(image.width(), image.height())
    );

    for(typename C2DArray<Tfrom>::const_iterator it = image.begin();
        it != image.end(); ++it)
    {
        Tfrom val = *it;
        uint8 c;
        if(val < 0){
            c = 0;
        }
        else if(256 <= val){
            c = 255;
        }
        else{
            c = static_cast<uint8>(val);
        }

        pDest->push_back(c);
    }

    return pDest;
}


} // namespace fitmb
#endif // fitmb_ToUInt8_hpp__

