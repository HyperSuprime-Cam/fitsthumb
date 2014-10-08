#ifndef  gf540caa5_a6ca_4fba_8625_16cd628369a9
#define  gf540caa5_a6ca_4fba_8625_16cd628369a9

#include "hsc/fitsthumb/Image.h"
#include "Common.h"
#include "LinearScale.h"
#include <cmath>
#include <algorithm>
#include <cstdio>
#include <vector>
#include <stdexcept>

namespace hsc { namespace fitsthumb {

template <class T>
void _ZSC_Sample(
    Image<T> const&  image,
    int              nSamples,
    std::vector<T>&  vSample_o
){
    std::size_t width  = image.Width();
    std::size_t height = image.Height();

    // extract, from image, about nSample samples
    // such that they form a grid.
    vSample_o.reserve(nSamples);

    int stride = std::max(
        1,
        int(std::sqrt(width*height / (double)nSamples))
    );

    while(stride >= 1){
        vSample_o.clear();

        for(std::size_t y = 0; y < height; y += stride){
            for(std::size_t x = 0; x < width; x += stride){
                T elem = image[y][x];
                if(std::isfinite(elem)) vSample_o.push_back(elem);
            }
        }

        // if more than 80% of nSamples were sampled, OK.
        if(vSample_o.size() * 5 > (std::size_t)nSamples * 4) break;

        --stride;
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
    // map the indices of vSample to [-1.0, 1.0]
    double xscale = 2.0 / (vSample.size() - 1);
    std::vector<double> xnorm;
    xnorm.reserve(vSample.size());
    for(unsigned i = 0; i < vSample.size(); ++i){
        xnorm.push_back((double)i * xscale - 1.0);
    }

    // Mask that is used in k-sigma clipping
    std::vector<int> vBadPix(vSample.size(), 0);

    int nGoodPix      =  vSample.size();
    int last_nGoodPix = nGoodPix + 1;

    // values to be obtained
    double intercept = 0;
    double slope     = 0;

    for(int iteration = 0; iteration < nIter; ++iteration){
        if(nGoodPix >= last_nGoodPix) break;
        if(nGoodPix <  minpix)        break;

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

        // slope and intercept
        intercept = (sumxx * sumy - sumx * sumxy) / delta;
        slope     = (sum * sumxy - sumx * sumy) / delta;

        // residue
        std::vector<double> vFlat;
        vFlat.reserve(vSample.size());
        for(unsigned i = 0; i < vSample.size(); ++i){
            vFlat.push_back(
                ((double)vSample[i]) - (xnorm[i] * slope + intercept)
            );
        }

        // Threshold of k-sigma clipping
        double sigma = _ZSC_ComputeSigma(vFlat, vBadPix, nGoodPix);
        double hcut = sigma * krej;
        double lcut = - hcut;

        // revise vBadPix
        last_nGoodPix = nGoodPix;
        nGoodPix = 0;

        for(unsigned i = 0; i < vSample.size(); ++i){
            double val = vFlat[i];
            if(val < lcut || hcut < val){
                vBadPix[i] = 1;
            }
        }

        // blurr vBadPix
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

    // restore the scale of x-axis
    *pnGoodPix_o = nGoodPix;
    *pzstart_o   = intercept - slope;
    *pzslope_o   = slope * xscale;
}


template <class T>
void
_ZScale(
    Image<T> const& image,
    int             nSamples,
    double          contrast,
    double*         pZ1,
    double*         pZ2
){
    const int iMAX_REJECT_RATIO = 2;
    const int iMIN_NPIXELS      = 5;
    const double dKREJ          = 2.5;
    const int iITERATIONS       = 5;

    // extract samples
    std::vector<T> vSample;
    _ZSC_Sample(image, nSamples, vSample);
    int nPix = vSample.size();

    if(vSample.empty()){
        throw std::runtime_error("ZScale: No pixel in image is finite");
    }

    std::sort(vSample.begin(), vSample.end());

    // max, min, median
    double zmin = vSample.front();
    double zmax = vSample.back();
    int iCenter = nPix / 2;
    T median = (nPix & 1) ?
        vSample[iCenter] :
        (vSample[iCenter] + vSample[iCenter+1]) / 2 ;

    // fit a line to the sorted sample
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
//      z1 = std::max(zmin, median - (iCenter - 1) * zslope);
//      z2 = std::min(zmax, median + (nPix - iCenter) * zslope
        *pZ1 = std::max(zmin, median - iCenter * zslope);
        *pZ2 = std::min(zmax, median + (nPix - iCenter - 1) * zslope);
    }
}


template <class Tfrom>
LinearScale
ZScale(
    Image<Tfrom> const& image,
    int                 nSamples = 1000,
    double              contrast = 0.25
){
    double z1, z2;
    _ZScale(image, nSamples, contrast, &z1, &z2);

    return LinearScale(
        // from
        z1, z2,
        // to
        0, 1
    );
}

}} // namespace hsc::fitsthumb
#endif //gf540caa5_a6ca_4fba_8625_16cd628369a9
