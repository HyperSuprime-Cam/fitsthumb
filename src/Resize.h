#ifndef  gc02fb528_12f8_4309_8d29_895d8d2d9b69
#define  gc02fb528_12f8_4309_8d29_895d8d2d9b69

#include "hsc/fitsthumb/Image.h"
#include "hsc/fitsthumb/SizeOption.h"
#include "Common.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace hsc { namespace fitsthumb {

template <class Tto, class Tfrom>
Image<Tto>
ResizeDown(
    Image<Tfrom> const& image,
    int w_new,
    int h_new
){
    if(w_new <= 0 && h_new <= 0){
        w_new = image.Width();
        h_new = image.Height();
    }

    if(w_new <= 0){
        w_new = std::max(std::size_t(1), h_new * image.Width() / image.Height());
    }
    if(h_new <= 0){
        h_new = std::max(std::size_t(1), w_new * image.Height() / image.Width());
    }

    if(w_new > (int)image.Width() || h_new > (int)image.Height()){
        throw std::runtime_error(MSG
        (   "Resize: Rescaling larger is not supported: ("
        <<  image.Width() << ", " << image.Height() << ") -> ("
        <<  w_new << ", " << h_new << ")"
        ));
    }

    Image<Tto> dest(h_new, w_new);
    std::fill_n(dest.data(), dest.size(), 0);

    double w_ratio = w_new / (double)image.Width();
    double h_ratio = h_new / (double)image.Height();

    double yTo0, yTo1 = 0;
    for(int y = 0; y < (int)image.Height(); ++y){
        Tfrom const* pLine = image[y];

        yTo0 = yTo1;
        yTo1 = (double)(y+1) * h_ratio;

        int yTo0i = (int)yTo0;
        int yTo1i = (int)yTo1;

        double yfrac = 0;
        if(yTo0i != yTo1i){
            yfrac = (yTo1 - yTo1i) / h_ratio;
        }

        double xTo0, xTo1 = 0;
        for(int x = 0; x < (int)image.Width(); ++x){
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

            dest[yTo0i][xTo0i] += (Tto)vx0y0;
            if(yTo1i < h_new){
                dest[yTo1i][xTo0i] += (Tto)vx0y1;
            }
            if(xTo1i < w_new){
                dest[yTo0i][xTo1i] += (Tto)vx1y0;
                if(yTo1i < h_new){
                    dest[yTo1i][xTo1i] += (Tto)vx1y1;
                }
            }
        }
    }

    double scale = w_new * h_new / ((double)image.Width() * image.Height());
    for(std::size_t i = 0; i < dest.size(); ++i){
        dest.data()[i] *= scale;
    }

    return dest;
}


template <class Tto, class Tfrom>
Image<Tto>
ResizeDown(
    Image<Tfrom> const& image,
    option::Size const& size
){
    if(size.IsRelative()){
        option::RelativeSize const& rel
            = static_cast<option::RelativeSize const&>(size);

        int width  = (rel.width  > 0) ?
            std::max(1, (int)(0.5 + rel.width  * image.Width ())) : 0;
        int height = (rel.height > 0) ?
            std::max(1, (int)(0.5 + rel.height * image.Height())) : 0;

        return ResizeDown<Tto>(image, width, height);
    }
    else{
        option::AbsoluteSize const& abs
            = static_cast<option::AbsoluteSize const&>(size);
        return ResizeDown<Tto>(image, abs.width, abs.height);
    }
}


}} // namespace hsc::fitsthumb
#endif //gc02fb528_12f8_4309_8d29_895d8d2d9b69
