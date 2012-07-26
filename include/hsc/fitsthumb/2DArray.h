#ifndef fitmb_2DArray_hpp__
#define fitmb_2DArray_hpp__

#include <vector>


namespace fitmb
{

template <class T>
class C2DArray
    : public std::vector<T>
{
public:
    C2DArray(): w_(0), h_(0) {}
    C2DArray(int w, int h)
        : w_(w), h_(h)
    {
        this->reserve(w*h);
    }

    T& operator()(int x, int y){
        return (*this)[x + y * w_];
    }

    T operator()(int x, int y) const {
        return (*this)[x + y * w_];
    }

    int width()  const { return w_; }
    int height() const { return h_; }

    T* ptr() const { return const_cast<T*>(&(*this)[0]); }

private:
    int w_;
    int h_;
};


} // namespace fitmb
#endif // fitmb_2DArray_hpp__
