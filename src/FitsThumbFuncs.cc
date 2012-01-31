#include "hsc/onsite/qa/fitsthumb/ZScale.h"
#include "hsc/onsite/qa/fitsthumb/InFITS.h"
#include "hsc/onsite/qa/fitsthumb/Output.h"
#include "hsc/onsite/qa/fitsthumb/Resize.h"
#include "hsc/onsite/qa/fitsthumb/CommandLine.h"
#include "hsc/onsite/qa/fitsthumb/FitsThumbFuncs.h"
//#include "lsst/afw/image/Image.h"
// 'fitmb::Ptr< fitmb::C2DArray< float > > *',
#include <cstdio>

//namespace afwImage = lsst::afw::image;
//typedef afwImage::Image<int> ImageT;
//typedef afwImage::Image<float> ImageF;

// Input is file path to FITS image file
int hsc::onsite::qa::fitsthumb::createFitsThumb(const char *inputFile, const char * outputFile, const char* outputType = "png", int width = 0, int height = 0, bool dynamicRangeFirst = true)
//int hsc::onsite::qa::fitsthumb::createFitsThumb(const char *inputFile, const char * outputFile, const char* outputType, int width, int height, bool dynamicRangeFirst) 
{
	using namespace fitmb;
	using namespace std;

//        outputtype = bmp|jpg|png

	// Input and output files

	//const char* szInput;
	//const char* szOutput;

	// Starting image conversion

	try{
		// reading FITS
		Ptr<C2DArray<float> > pSrc = InFITS<float>(inputFile);
		Ptr<C2DArray<uint8> > pDest;

		if(width <= 0 && height <= 0){
			// no resize
			pDest = ConvRange_ZScale<uint8>(*pSrc);
			Del(pSrc);
		}
		else if(dynamicRangeFirst == true){
			// dynamic range change first, then resize
			Ptr<C2DArray<uint8> > pZScale =
				ConvRange_ZScale<uint8>(*pSrc);
			Del(pSrc);

			// doing resize
			Ptr<C2DArray<double> > pResize =
				ResizeDown<double>(
					*pZScale, width, height
				);
			Del(pZScale);

			pDest = ToUInt8(*pResize);
			Del(pResize);
		}
		else{
			// resizing first, then changing dynamic range
			Ptr<C2DArray<double> > pResize =
				ResizeDown<double>(*pSrc, width, height);
			Del(pSrc);

			// changing dynamic range
			pDest = ConvRange_ZScale<uint8>(*pResize);
			Del(pResize);
		}

		// outputing file
		Output(
			*pDest,
			outputType,
			outputFile ? outputFile : inputFile,
			! outputFile
		);
                Del(pDest);

	}
	catch(std::exception& e){
		std::fprintf(stdout, "error: %s\n", e.what());
		return 1;
	}

	return 0;
}


// Input is afw.Image object
int hsc::onsite::qa::fitsthumb::createFitsThumb(lsst::afw::image::Image<float>::Ptr &image, const char *outputFile, const char* outputType = "png", int width = 0, int height = 0, bool dynamicRangeFirst = true)
{

    using namespace fitmb;
    using namespace std;

    // Starting image conversion
    try{
        // preparing pixel data
        Ptr<C2DArray<float> > pSrc = hsc::onsite::qa::fitsthumb::convertImageToC2dArray(image);
        //Ptr<C2DArray<float> > pSrc;

        Ptr<C2DArray<uint8> > pDest;

        if(width <= 0 && height <= 0){
            // no resize
            pDest = ConvRange_ZScale<uint8>(*pSrc);
            Del(pSrc);
        }
        else if(dynamicRangeFirst == true){
            // dynamic range change first, then resize
            Ptr<C2DArray<uint8> > pZScale =
                ConvRange_ZScale<uint8>(*pSrc);
            Del(pSrc);
            
            // doing resize
            Ptr<C2DArray<double> > pResize =
                ResizeDown<double>(
                    *pZScale, width, height
                    );
            Del(pZScale);
            
            pDest = ToUInt8(*pResize);
            Del(pResize);
        }
        else {
            // resizing first, then changing dynamic range
            Ptr<C2DArray<double> > pResize =
                ResizeDown<double>(*pSrc, width, height);
            Del(pSrc);

            // changing dynamic range
            pDest = ConvRange_ZScale<uint8>(*pResize);
            Del(pResize);
        }

        // outputing file
        Output(
            *pDest,
            outputType,
//            outputFile ? outputFile : inputFile,
            outputFile,
            ! outputFile
            );

        // FH
        Del(pDest);
    }
    catch(std::exception& e){
        std::fprintf(stdout, "error: %s\n", e.what());
        return 1;
    }
    
    return 0;
}

fitmb::Ptr< fitmb::C2DArray<float> > hsc::onsite::qa::fitsthumb::convertImageToC2dArray(lsst::afw::image::Image<float>::Ptr &image)
{
    using namespace fitmb;
    using namespace std;

    int width = image->getWidth();
    int height = image->getHeight();
    // std::vector<float> pixelData(width*height);

    Ptr<C2DArray<float> > imageArray (new C2DArray<float>(width, height));
    imageArray->resize(width * height);

//    int i = 0;

//    vector<float, allocator<float> >::pointer pixPtr = imageArray->pointer();
    float * pixPtr = imageArray->ptr();

//    memcpy( imageArray->pointer(), image->begin(), width*height);
    for (lsst::afw::image::Image<float>::iterator ptr = image->begin(), end = image->end(); ptr != end; ++ptr) {
        *pixPtr = *ptr;
        pixPtr++;
//        i += 1;
    }
    
    return imageArray;

}
