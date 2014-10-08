#ifndef  gea4abd1f_d998_4cc0_a2f8_ce9acdbd6777
#define  gea4abd1f_d998_4cc0_a2f8_ce9acdbd6777

#include <cstdlib>
#include <new>


namespace hsc { namespace fitsthumb {

template <class Elem>
class Image
{
private:
    static std::size_t PaddedSize(std::size_t height, std::size_t stride){
        return (height*stride*sizeof(Elem) + sizeof(std::size_t) - 1) & ~(sizeof(std::size_t) - 1);
    }

public:
    Image()
        : image_  (NULL)
        , counter_(NULL)
        , height_ (0   )
        , width_  (0   )
        , stride_ (0   )
    {}

    Image(std::size_t height, std::size_t width)
        : image_  ((Elem*)std::malloc(PaddedSize(height, width) + sizeof(std::size_t)))
        , counter_(new((char*)image_ + PaddedSize(height, width)) std::size_t(1))
        , height_ (height)
        , width_  (width)
        , stride_ (width)
    {}

    Image(Elem* data, std::size_t height, std::size_t width, std::size_t stride)
        : image_  (data  )
        , counter_(NULL  )
        , height_ (height)
        , width_  (width )
        , stride_ (stride)
    {}

    Image(Image const& other)
        : image_  (other.image_  )
        , counter_(other.counter_)
        , height_ (other.height_ )
        , width_  (other.width_  )
        , stride_ (other.stride_ )
    {
        if(counter_) ++(*counter_);
    }

    ~Image(){ this->Release(); }

    Image& operator=(Image const& other){
        if(this != &other){
            this->Release();
            this->image_   = other.image_  ;
            this->counter_ = other.counter_;
            this->height_  = other.height_ ;
            this->width_   = other.width_  ;
            this->stride_  = other.stride_ ;

            if(counter_) ++(*counter_);
        }
        return *this;
    }

    void Release(){
        if(image_){
            if(counter_ && --(*counter_) == 0){
                std::free(image_);
            }

            image_   = NULL;
            counter_ = NULL;
            height_  = 0;
            width_   = 0;
            stride_  = 0;
        }
    }

    void Detach(){
        counter_ = NULL;
    }

    std::size_t Height() const { return height_; }
    std::size_t Width () const { return width_ ; }
    std::size_t Stride() const { return stride_; }

    Elem* operator[](std::size_t y){ return image_ + y*stride_; }
    Elem const* operator[](std::size_t y) const { return image_ + y*stride_; }

    Elem* data(){ return image_; }
    Elem const* data() const { return image_; }

    std::size_t size() const { return height_*stride_; }

private:
    Elem       * image_  ;
    std::size_t* counter_;
    std::size_t  height_ ;
    std::size_t  width_  ;
    std::size_t  stride_ ;
};

}} // hsc::fitsthumb
#endif //gea4abd1f_d998_4cc0_a2f8_ce9acdbd6777
