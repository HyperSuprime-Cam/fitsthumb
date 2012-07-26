#ifndef fitmb_InFITS_hpp__
#define fitmb_InFITS_hpp__

#include "2DArray.h"
#include "Common.h"
#include <fitsio.h> // cfitsio header

#ifdef NDEBUG
#	pragma comment(lib, "cfitsio.lib")
#else
#	pragma comment(lib, "cfitsiod.lib")
#endif


namespace fitmb
{

template <class T>
struct _CFitsioType;

template <> struct _CFitsioType<sint8>  { enum { v = TSBYTE  }; };
template <> struct _CFitsioType<uint8>  { enum { v = TBYTE   }; };
template <> struct _CFitsioType<sint16> { enum { v = TSHORT  }; };
template <> struct _CFitsioType<uint16> { enum { v = TUSHORT }; };
template <> struct _CFitsioType<sint32> { enum { v = TLONG   }; };
template <> struct _CFitsioType<uint32> { enum { v = TULONG  }; };
template <> struct _CFitsioType<sint64> { enum { v = TLONGLONG }; };
template <> struct _CFitsioType<float>  { enum { v = TFLOAT  }; };
template <> struct _CFitsioType<double> { enum { v = TDOUBLE }; };


template <class T>
Ptr<C2DArray<T> >
InFITS(const char* szFile)
{
	int status = 0;
	fitsfile* f;
	fits_open_diskfile(&f, szFile, READONLY, &status);
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
	(	f
	,	_CFitsioType<T>::v
	,	1
	,	width*height
	,	0
	,	pImage->ptr()
	,	&anynul
	,	&status
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
