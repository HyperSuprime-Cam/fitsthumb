#ifndef fitmb_InFITS_hpp__
#define fitmb_InFITS_hpp__

#include "2DArray.h"
#include "Common.h"
#include <fitsio.h> // cfitsio header

#ifdef NDEBUG
#   pragma comment(lib, "cfitsio.lib")
#else
#   pragma comment(lib, "cfitsiod.lib")
#endif


namespace fitmb
{

template <class T>
struct _InFits_IsSigned
{
    static const bool value = (T(-1) < T(0));
};

template <class T>
struct _FitsIOInternalType;

template <> struct _FitsIOInternalType<char          > { static const int value = _InFits_IsSigned<char>::value? TSBYTE: TBYTE ; };
template <> struct _FitsIOInternalType<signed char   > { static const int value = TSBYTE    ; };
template <> struct _FitsIOInternalType<unsigned char > { static const int value = TBYTE     ; };
template <> struct _FitsIOInternalType<short         > { static const int value = TSHORT    ; };
template <> struct _FitsIOInternalType<unsigned short> { static const int value = TUSHORT   ; };
template <> struct _FitsIOInternalType<int           > { static const int value = TINT      ; };
template <> struct _FitsIOInternalType<unsigned int  > { static const int value = TUINT     ; };
template <> struct _FitsIOInternalType<long          > { static const int value = TLONG     ; };
template <> struct _FitsIOInternalType<unsigned long > { static const int value = TULONG    ; };
template <> struct _FitsIOInternalType<long long     > { static const int value = TLONGLONG ; };
template <> struct _FitsIOInternalType<float         > { static const int value = TFLOAT    ; };
template <> struct _FitsIOInternalType<double        > { static const int value = TDOUBLE   ; };


template <class T>
Ptr<C2DArray<T> >
InFITS(const char* szFile)
{
    int status = 0;
    fitsfile* f;
    fits_open_file(&f, szFile, READONLY, &status);
    if(status != 0){
        throw std::runtime_error(MSG(szFile << " cannot be opened."));
    }

    long naxis[2];
    int num = 0;
    fits_read_keys_lng(f, "NAXIS", 1, 2, naxis, &num, &status);
    if(num != 2){
        status = 0;
        fits_close_file(f, &status);
        throw std::runtime_error(MSG(szFile << ": is not a 2-dimensional image."));
    }

    int width  = naxis[0];
    int height = naxis[1];

    Ptr<C2DArray<T> > pImage (new C2DArray<T>(width, height));
    pImage->resize(width * height);

    int anynul;
    fits_read_img
    (   f
    ,   _FitsIOInternalType<T>::value
    ,   1
    ,   width*height
    ,   0
    ,   pImage->ptr()
    ,   &anynul
    ,   &status
    );

    if(status != 0){
        status = 0;
        fits_close_file(f, &status);
        throw std::runtime_error(MSG(szFile << ": read error."));
    }

    status = 0;
    fits_close_file(f, &status);

    return pImage;
}

} // namespace fitmb
#endif // fitmb_InFITS_hpp__
