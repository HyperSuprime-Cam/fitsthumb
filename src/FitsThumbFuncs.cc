#include "ZScale.h"
#include "InFITS.h"
#include "Output.h"
#include "Resize.h"
#include "CommandLine.h"
#include "FitsThumbFuncs.h"
#include <cstdio>

int hsc::onsite::qa::createFitsThumb(const char *inputFile, const char * outputFile, const char* outputType = "png", int width = 0, int height = 0, bool dynamicRangeFirst = true)
//int hsc::onsite::qa::createFitsThumb(const char *inputFile, const char * outputFile, const char* outputType, int width, int height, bool dynamicRangeFirst) 
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
	}
	catch(std::exception& e){
		std::fprintf(stdout, "error: %s\n", e.what());
		return 1;
	}

	return 0;
}

// eof
