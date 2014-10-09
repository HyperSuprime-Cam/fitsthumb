#include "Statistics.h"
#include "Random.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <iterator>


namespace hsc { namespace fitsthumb {

namespace {

    template <class Iterator, class Pred>
    Iterator remove_if(Iterator begin, Iterator end, Pred const& pred){
        for(; begin != end; ++begin){
            if(!pred(*begin)) continue;

            Iterator it = begin;
            ++it;
            for(; it != end; ++it){
                if(!pred(*it)){
                    *begin = *it;
                    ++begin;
                }
            }
            return begin;
        }

        return end;
    }

    inline
    bool is_not_finite(double x){ return !std::isfinite(x); }

    class outside
    {
    public:
        outside(double a, double b): a_(a), b_(b) {}
        bool operator()(double x) const { return x < a_ || b_ < x; }

    private:
        double a_;
        double b_;
    };


    template <class Elem>
    void Copy(Image<Elem> const& src, std::vector<double>& dest)
    {
        if(src.Width() == src.Stride()){
            dest.assign(src.data(), src.data() + src.size());
        }
        else{
            dest.clear();
            dest.reserve(src.Width() * src.Height());

            for(std::size_t y = 0; y < src.Height(); ++y){
                dest.insert(dest.end(), src[y], src[y] + src.Width());
            }
        }
    }


    Statistics GetStatistics(std::vector<double>& samples)
    {
        // exclude NaN and INF from the samples
        samples.erase(
            fitsthumb::remove_if(samples.begin(), samples.end(), is_not_finite),
            samples.end()
        );

        Statistics stat;

        if(samples.size() < 3){
            stat.mean = NAN;
            stat.stddev = NAN;
            return stat;
        }

        // get quantiles
        std::size_t n = samples.size();
        std::vector<double>::iterator it1 = samples.begin() + n/4;
        std::vector<double>::iterator it2 = samples.begin() + n/2;
        std::vector<double>::iterator it3 = samples.begin() + 3*n/4;

        std::nth_element(samples.begin(), it2, samples.end());
        std::nth_element(samples.begin(), it1, it2);
        std::nth_element(it2, it3, samples.end());

        double const invNormalIQR = 7.4130110925280102e-01;

        // cast away >5 sigma samples
        double thresh = (5 * invNormalIQR) * (*it3 - *it1);
        samples.erase(
            fitsthumb::remove_if(samples.begin(), samples.end(), outside(*it2 - thresh, *it2 + thresh)),
            samples.end()
        );

        // re-compute quantiles
        n = samples.size();
        it1 = samples.begin() + n/4;
        it2 = samples.begin() + n/2;
        it3 = samples.begin() + 3*n/4;

        std::nth_element(samples.begin(), it2, samples.end());
        std::nth_element(samples.begin(), it1, it2);
        std::nth_element(it2, it3, samples.end());

        stat.mean   = *it2;
        stat.stddev = invNormalIQR * (*it3 - *it1);
        return stat;
    }


    template <class InputIterator>
    MinMax<typename std::iterator_traits<InputIterator>::value_type>
    GetMinMax(InputIterator begin, InputIterator end)
    {
        typedef typename std::iterator_traits<InputIterator>::value_type  T;

        T min = NAN;
        for(; begin != end; ++begin){
            min = *begin;
            if(min == min) break;
        }

        T max = min;
        ++begin;
        for(; begin != end; ++begin){
            T value = *begin;
            if(value < min) min = value;
            else if(max < value) max = value;
        }

        MinMax<T> mm;
        mm.min = min;
        mm.max = max;

        return mm;
    }

}


template <class Elem>
Statistics GetStatistics(Image<Elem> const& image)
{
    std::size_t const w = image.Width();
    std::size_t const h = image.Height();

    // Extract  a small amount of pixels from the image
    std::size_t const nSamples = 1000;
    std::vector<double> samples;

    std::size_t interval = (std::size_t)(
        std::sqrt((double)(w*h)/(double)(nSamples))
    );

    if(interval > 1){
        Random rand;
        samples.reserve(4*nSamples);

        // For each mesh [yBeg:yBeg+interval, xBeg:xBeg+interval],
        // choose a random pixel in it

        for(std::size_t yBeg = 0; yBeg < h; yBeg += interval){
            std::size_t yEnd = (std::min)(yBeg + interval, h);

            for(std::size_t xBeg = 0; xBeg < w; xBeg += interval){
                std::size_t xEnd = (std::min)(xBeg + interval, w);

                samples.push_back(image[rand.Int(yBeg, yEnd)][rand.Int(xBeg, xEnd)]);
            }
        }
    }
    else{
        Copy(image, samples);
    }

    Statistics stat = GetStatistics(samples);

    if(!(stat.stddev > 0)){
        // if failed, and
        if(interval > 1){
            // if we have not used the whole data,
            // try again with the whole data
            Copy(image, samples);
            stat = GetStatistics(samples);
        }
    }

    return stat;
}

template
Statistics GetStatistics(Image<float> const& image);

template
Statistics GetStatistics(Image<double> const& image);


template <class Elem>
MinMax<Elem> GetMinMax(Image<Elem> const& image)
{
    if(image.Width() == image.Stride()){
        return GetMinMax(image.data(), image.data() + image.size());
    }

    MinMax<Elem> mm;
    mm.min = NAN;
    mm.max = NAN;

    std::size_t y;
    for(y = 0; y < image.Height(); ++y){
        Elem const* image_y = image[y];
        mm = GetMinMax(image_y, image_y + image.Width());
        if(mm.min == mm.min) break;
    }

    ++y;
    for(; y < image.Height(); ++y){
        Elem const* image_y = image[y];
        MinMax<Elem> t = GetMinMax(image_y, image_y + image.Width());

        if(t.min < mm.min) mm.min = t.min;
        else if(mm.max < t.max) mm.max = t.max;
    }

    return mm;
}

template
MinMax<float> GetMinMax(Image<float> const& image);

template
MinMax<double> GetMinMax(Image<double> const& image);



}} // namespace hsc::fitsthumb
