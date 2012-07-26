#ifndef fitmb_Resize_hpp__
#define fitmb_Resize_hpp__

#include "2DArray.h"
#include "Common.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace fitmb {

template <class Tto, class Tfrom>
Ptr<C2DArray<Tto> >
ResizeDown(
    const C2DArray<Tfrom>& image,
    int w_new,
    int h_new
){
    if(w_new < 0 && h_new < 0){
        throw std::runtime_error("Resize: Target image size is 0");
    }

    if(w_new == 0){
        w_new = std::max(1, h_new * image.width() / image.height());
    }
    if(h_new == 0){
        h_new = std::max(1, w_new * image.height() / image.width());
    }

    if(w_new > image.width() || h_new > image.height()){
        throw std::runtime_error(MSG
        (   "Resize: Rescaling larger is not supported: ("
        <<  image.width() << ", " << image.height() << ") -> ("
        <<  w_new << ", " << h_new << ")"
        ));
    }

    Ptr<C2DArray<Tto> > pDest (new C2DArray<Tto>(w_new, h_new));
    pDest->resize(w_new * h_new, 0);

    double w_ratio = w_new / (double)image.width();
    double h_ratio = h_new / (double)image.height();

    Tfrom* pLine = image.ptr();
    double yTo0, yTo1 = 0;
    for(int y = 0; y < image.height(); ++y){
        yTo0 = yTo1;
        yTo1 = (double)(y+1) * h_ratio;

        int yTo0i = (int)yTo0;
        int yTo1i = (int)yTo1;

        double yfrac = 0;
        if(yTo0i != yTo1i){
            yfrac = (yTo1 - yTo1i) / h_ratio;
        }

        double xTo0, xTo1 = 0;
        for(int x = 0; x < image.width(); ++x){
            xTo0 = xTo1;
            xTo1 = (double)(x+1) * w_ratio;

            int xTo0i = (int)xTo0;
            int xTo1i = (int)xTo1;

            double xfrac = 0;
            if(xTo0i != xTo1i){
                xfrac = (xTo1 - xTo1i) / w_ratio;
            }

            double v = (double)pLine[x];
            double vx1   = v * xfrac;
            double vx0   = v - vx1;
            double vx1y1 = vx1 * yfrac;
            double vx1y0 = vx1 - vx1y1;
            double vx0y1 = vx0 * yfrac;
            double vx0y0 = vx0 - vx0y1;

            (*pDest)(xTo0i, yTo0i) += (Tto)vx0y0;
            if(yTo1i < h_new){
                (*pDest)(xTo0i, yTo1i) += (Tto)vx0y1;
            }
            if(xTo1i < w_new){
                (*pDest)(xTo1i, yTo0i) += (Tto)vx1y0;
                if(yTo1i < h_new){
                    (*pDest)(xTo1i, yTo1i) += (Tto)vx1y1;
                }
            }
        }

        pLine += image.width();
    }

    double scale = w_new * h_new / ((double)image.width() * image.height());
    for(typename C2DArray<Tto>::iterator it = pDest->begin();
        it != pDest->end(); ++it)
    {
        *it *= scale;
    }

    return pDest;
}

} // namespace fitmb
#endif // fitmb_Resize_hpp__
