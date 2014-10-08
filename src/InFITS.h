#ifndef  g645aa3e1_8275_4899_80c2_7aebed1ee669
#define  g645aa3e1_8275_4899_80c2_7aebed1ee669

#include "hsc/fitsthumb/Image.h"
#include "Common.h"
#include <stdexcept>

#include <fitsio.h> // cfitsio header

#ifdef NDEBUG
#   pragma comment(lib, "cfitsio.lib")
#else
#   pragma comment(lib, "cfitsiod.lib")
#endif


namespace hsc { namespace fitsthumb {

template <class T>
struct InFits_IsSigned_
{
    static const bool value = (T(-1) < T(0));
};

template <class T>
struct FitsIOInternalType_;

template <> struct FitsIOInternalType_<char          > { static const int value = InFits_IsSigned_<char>::value? TSBYTE: TBYTE ; };
template <> struct FitsIOInternalType_<signed char   > { static const int value = TSBYTE    ; };
template <> struct FitsIOInternalType_<unsigned char > { static const int value = TBYTE     ; };
template <> struct FitsIOInternalType_<short         > { static const int value = TSHORT    ; };
template <> struct FitsIOInternalType_<unsigned short> { static const int value = TUSHORT   ; };
template <> struct FitsIOInternalType_<int           > { static const int value = TINT      ; };
template <> struct FitsIOInternalType_<unsigned int  > { static const int value = TUINT     ; };
template <> struct FitsIOInternalType_<long          > { static const int value = TLONG     ; };
template <> struct FitsIOInternalType_<unsigned long > { static const int value = TULONG    ; };
template <> struct FitsIOInternalType_<long long     > { static const int value = TLONGLONG ; };
template <> struct FitsIOInternalType_<float         > { static const int value = TFLOAT    ; };
template <> struct FitsIOInternalType_<double        > { static const int value = TDOUBLE   ; };

class FitsIOClose_
{
public:
    explicit FitsIOClose_(fitsfile* f): f_(f) {}

    void operator()() const {
        int status = 0;
        ::fits_close_file(f_, &status);
    }

private:
    fitsfile* f_;
};


template <class T>
Image<T>
InFITS(const char* szFile)
{
    int status = 0;
    fitsfile* f;
    fits_open_file(&f, szFile, READONLY, &status);
    if(status != 0){
        throw std::runtime_error(MSG(szFile << " cannot be opened."));
    }
    Destructor const& closefits = MakeDestructor(FitsIOClose_(f));

    long naxis[2];
    int num = 0;
    fits_read_keys_lng(f, "NAXIS", 1, 2, naxis, &num, &status);
    if(num != 2){
        throw std::runtime_error(MSG(szFile << ": is not a 2-dimensional image."));
    }

    int width  = naxis[0];
    int height = naxis[1];

    Image<T> image(height, width);

    int anynul;
    fits_read_img
    (   f
    ,   FitsIOInternalType_<T>::value
    ,   1
    ,   width*height
    ,   0
    ,   image.data()
    ,   &anynul
    ,   &status
    );

    if(status != 0){
        throw std::runtime_error(MSG(szFile << ": read error."));
    }

    return image;
}

}} // namespace hsc::fitsthumb
#endif //g645aa3e1_8275_4899_80c2_7aebed1ee669
