#ifndef fitmb_Scale_hpp__
#define fitmb_Scale_hpp__

#include "2DArray.h"
#include "Common.h"
#include <cmath>
#include <algorithm>
#include <cstdio>

namespace fitmb
{

template <class T>
void _ZSC_Sample(
	const C2DArray<T>&  image,
	int                 nSamples,
	std::vector<T>&     vSample_o
){
	// image から 格子状に nSample 個「くらい」サンプリング

	int stride = std::max(
		1,
		int(std::sqrt(image.size() / (double)nSamples))
	);

	vSample_o.reserve(nSamples);

	for(int y = 0; y < image.height(); y += stride){
		for(int x = 0; x < image.width(); x += stride){
			vSample_o.push_back(image(x, y));
		}
	}
}


inline double
_ZSC_ComputeSigma(
	const std::vector<double>& vFlat,
	const std::vector<int>&    vBadPix,
	int nGoodPix
){
	switch(nGoodPix){
	case 0:
	case 1:
		return 0;
	}

	double sumz = 0, sumsq = 0;
	for(unsigned i = 0; i < vFlat.size(); ++i){
		if(! vBadPix[i]){
			double z = vFlat[i];

			sumz  += z;
			sumsq += z*z;
		}
	}

	double goodPix = (double)nGoodPix;
	double tmp =
		sumsq / (goodPix - 1) - sumz * sumz / (goodPix * (goodPix - 1));

	return (tmp > 0) ? std::sqrt(tmp) : 0;
		
}


template <class T>
void _ZSC_FitLine(
	const std::vector<T>& vSample,
	double                krej,
	int                   nGrow,
	int                   minpix,
	int                   nIter,
	int*                  pnGoodPix_o,
	double*               pzstart_o,
	double*               pzslope_o
){
	// vSample のインデクスを -1.0 - 1.0 にマップ
	double xscale = 2.0 / (vSample.size() - 1);
	std::vector<double> xnorm;
	xnorm.reserve(vSample.size());
	for(unsigned i = 0; i < vSample.size(); ++i){
		xnorm.push_back((double)i * xscale - 1.0);
	}

	// k-sigmaクリップに使うマスク
	std::vector<int> vBadPix(vSample.size(), 0);

	int nGoodPix      =  vSample.size();
	int last_nGoodPix = nGoodPix + 1;

	// 求めるべき値
	double intercept = 0;
	double slope     = 0;

	for(int iteration = 0; iteration < nIter; ++iteration){
		if(nGoodPix >= last_nGoodPix) break;
		if(nGoodPix <  minpix)        break;

		// サム
		double sum = (double)nGoodPix;
		double sumx = 0, sumy = 0, sumxx = 0, sumxy = 0;
		for(unsigned i = 0; i < vSample.size(); ++i){
			if(! vBadPix[i]){
				double x = xnorm[i];
				double y = (double)vSample[i];

				sumx  += x;
				sumy  += y;
				sumxx += x*x;
				sumxy += x*y;
			}
		}

		double delta = sum * sumxx - sumx * sumx;

		// 傾きと切片
		intercept = (sumxx * sumy - sumx * sumxy) / delta;
		slope     = (sum * sumxy - sumx * sumy) / delta;

		// 残差
		std::vector<double> vFlat;
		vFlat.reserve(vSample.size());
		for(unsigned i = 0; i < vSample.size(); ++i){
			vFlat.push_back(
				((double)vSample[i]) - (xnorm[i] * slope + intercept)
			);
		}

		// k-sigma クリップのスレッシュホルド
		double sigma = _ZSC_ComputeSigma(vFlat, vBadPix, nGoodPix);
		double hcut = sigma * krej;
		double lcut = - hcut;

		// vBadPix を更新
		last_nGoodPix = nGoodPix;
		nGoodPix = 0;

		for(unsigned i = 0; i < vSample.size(); ++i){
			double val = vFlat[i];
			if(val < lcut || hcut < val){
				vBadPix[i] = 1;
			}
		}

		// vBadPix をぼかす
		std::vector<int>vBadPix_new;
		vBadPix_new.reserve(vSample.size());
		for(unsigned x = 0; x < vSample.size(); ++x){
			int imin = ((int)x > nGrow) ? x - nGrow : -1;
			int val = 0;
			for(int i = x; i > imin ; --i){
				val += vBadPix[i];
			}
			vBadPix_new.push_back(val ? 1 : 0);
			if(!val){
				++nGoodPix;
			}
		}
		vBadPix = vBadPix_new;
	}

	// x方向のスケールを元に戻す
	*pnGoodPix_o = nGoodPix;
	*pzstart_o   = intercept - slope;
	*pzslope_o   = slope * xscale;
}


template <class T>
void
_ZScale(
	const C2DArray<T>& image,
	int                nSamples,
	double             contrast,
	double*            pZ1,
	double*            pZ2
){
	const int iMAX_REJECT_RATIO = 2;
	const int iMIN_NPIXELS      = 5;
	const double dKREJ          = 2.5;
	const int iITERATIONS       = 5;

	// サンプルを抽出
	std::vector<T> vSample;
	_ZSC_Sample(image, nSamples, vSample);
	int nPix = vSample.size();

	if(vSample.empty()){
		throw CException("ZScale: 画像サイズが 0");
	}

	// ソート
	std::sort(vSample.begin(), vSample.end());

	// 最大値、最小値、メディアン
	double zmin = vSample.front();
	double zmax = vSample.back();
	int iCenter = nPix / 2;
	T median = (nPix & 1) ?
		vSample[iCenter] :
		(vSample[iCenter] + vSample[iCenter+1]) / 2 ;

	// ソートしたサンプルに直線をフィット
	int minpix = std::max(iMIN_NPIXELS, nPix / iMAX_REJECT_RATIO);
	int nGrow  = std::max(1, nPix / 100);
	int nGoodPix;
	double zstart, zslope;
	_ZSC_FitLine(
		vSample, dKREJ, nGrow, minpix, iITERATIONS,
		&nGoodPix, &zstart, &zslope
	);

	if(nGoodPix < minpix){
		*pZ1 = zmin;
		*pZ2 = zmax;
	}
	else{
		zslope /= contrast;
//		z1 = std::max(zmin, median - (iCenter - 1) * zslope);
//		z2 = std::min(zmax, median + (nPix - iCenter) * zslope
		*pZ1 = std::max(zmin, median - iCenter * zslope);
		*pZ2 = std::min(zmax, median + (nPix - iCenter - 1) * zslope);
	}
}


template <class Tto, class Tfrom>
Ptr<C2DArray<Tto> >
ConvRange_ZScale(
	const C2DArray<Tfrom>& image,
	int                    nSamples = 1000,
	double                 contrast = 0.25
){
	double z1, z2;
	_ZScale(image, nSamples, contrast, &z1, &z2);

	std::printf("dynamrange: %g - %g\n", z1, z2);

	Ptr<C2DArray<Tto> > pDest (
		new C2DArray<Tto>(image.width(), image.height())
	);

	for(typename C2DArray<Tfrom>::const_iterator it = image.begin();
		it != image.end(); ++it)
	{
		double val = (double)(*it);
		val = 256 * (val - z1) / (z2 - z1);
		Tto c;
		if(val < 0){
			c = 0;
		}
		else if(256 <= val){
			c = 255;
		}
		else{
			c = static_cast<Tto>(val);
		}

		pDest->push_back(c);
	}

	return pDest;
}


} // namespace fitmb
#endif //fitmb_Scale_hpp__
