#ifndef  gcfcbd781_9254_4304_8f94_e5ea4c35781a
#define  gcfcbd781_9254_4304_8f94_e5ea4c35781a

#include <string>
#include <sstream>


//____________________________________________________________________________
// Message string, used as:
//     throw std::runtime_error(MSG("ERROR with a = " << a << ", " << "b = " << b));

#define MSG(msg__) \
    static_cast<std::stringstream&> \
    (    const_cast<std::stringstream&> \
         (  static_cast<std::stringstream const&>(std::stringstream()) \
         ) << msg__ \
    ).str()

#define CMSG(msg__)   MSG(msg__).c_str()


namespace hsc { namespace fitsthumb {


//__________________________________________________________________________
// Install a destructor in a scope:
// {
//     Destructor const& dtor = MakeDestructor(func);
//     ...
//     // func() is called just before this scope is exited
// }
class Destructor
{
protected:
    Destructor(){}
private:
    Destructor(Destructor const&);
    void operator=(Destructor const&);
};

template <class X> class Destructor_: public Destructor
{
public:
    explicit Destructor_(X const& x)
        : x_(x), valid_(true)
    {}

    Destructor_(Destructor_ const& other)
        : x_(other.x_), valid_(other.valid_)
    {
        other.valid_ = false;
    }

    ~Destructor_(){
        try { this->Call(); } catch(...){}
    }

    void Call() const {
        if(valid_){
            valid_ = false;
            x_();
        }
    }

private:
    void operator=(Destructor_ const& other);

    mutable X    x_;
    mutable bool valid_;
};


template <class X>
inline
Destructor_<X> MakeDestructor(X const& x){
    return Destructor_<X>(x);
}


//__________________________________________________________________________
// type selecter
template <bool B, typename T, typename F>
struct type_select;

template <typename T, typename F>
struct type_select <true, T, F>
{
    typedef T  type;
};

template <typename T, typename F>
struct type_select <false, T, F>
{
    typedef F  type;
};


//__________________________________________________________________________
// uint8_t - uint64_t
template <unsigned size, bool bSigned>
struct size_to_type
{
    typedef
        typename type_select < (sizeof(char) == size),
            typename type_select<bSigned, signed char, unsigned char>::type,
        typename type_select < (sizeof(short) == size),
            typename type_select<bSigned, short, unsigned short>::type,
        typename type_select < (sizeof(int) == size),
            typename type_select<bSigned, int, unsigned int>::type,
        typename type_select < (sizeof(long) == size),
            typename type_select<bSigned, long, unsigned long>::type,
        typename type_select < (sizeof(long long) == size),
            typename type_select<bSigned, long long, unsigned long long>::type,
        void
        >::type>::type>::type>::type>::type      type;
};


typedef type_select<((unsigned char)(0x180) == 0x80), size_to_type<1, false>::type, void>::type   uint8_t;
typedef type_select<((unsigned char)(0x180) == 0x80), size_to_type<1, true >::type, void>::type    int8_t;
typedef type_select<((unsigned char)(0x180) == 0x80), size_to_type<2, false>::type, void>::type  uint16_t;
typedef type_select<((unsigned char)(0x180) == 0x80), size_to_type<2, true >::type, void>::type   int16_t;
typedef type_select<((unsigned char)(0x180) == 0x80), size_to_type<4, false>::type, void>::type  uint32_t;
typedef type_select<((unsigned char)(0x180) == 0x80), size_to_type<4, true >::type, void>::type   int32_t;
typedef type_select<((unsigned char)(0x180) == 0x80), size_to_type<8, false>::type, void>::type  uint64_t;
typedef type_select<((unsigned char)(0x180) == 0x80), size_to_type<8, true >::type, void>::type   int64_t;

}} // namespace hsc::fitsthumb
#endif //gcfcbd781_9254_4304_8f94_e5ea4c35781a
