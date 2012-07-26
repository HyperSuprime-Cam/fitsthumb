#ifndef fitmb_Common_hpp__
#define fitmb_Common_hpp__

#include <string>
#include <sstream>
#include <cstdio>


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


namespace fitmb
{

//____________________________________________________________________________
// Bool return-type
// That cannot be added, deleted, called, nor converted to void*
class _common_CUncallable {};
typedef  _common_CUncallable (*BoolType) (_common_CUncallable);
template <class Ty>
inline BoolType BoolTypeCast(const Ty& p){ return reinterpret_cast<BoolType>(reinterpret_cast<long>(p)); }


//____________________________________________________________________________
// smart ptr

struct Ptr_DoNotOwn {};
struct Ptr_StaticCast {};
struct Ptr_DynamicCast {};


class _common_CPtrInfo
{
public:
    virtual ~_common_CPtrInfo(){}
    _common_CPtrInfo(): cnt_(1){}

    // add reference
    void AddRef(){ ++ cnt_; }

    // sub reference and it being 0 delete ptr to return true.
    // if there remain other refs, return false.
    bool SubRef(){
        if(--cnt_ == 0){ this->Delete(); return true; }
        else return false;
    }

    long Count(){ return cnt_; }

protected:
    virtual void Delete() = 0;

private:
    long cnt_;
};


template<class Ty>
class Ptr {
    // subset of boost::shared_ptr
public:
    typedef Ty element_type;

    // standard deleters
    static void Normal  (Ty* p){ delete   p; }
    static void Array   (Ty* p){ delete[] p; }

    class CInfo : public _common_CPtrInfo
    {
    public:
        virtual ~CInfo(){}
        CInfo(Ty* p, void (*del)(Ty*) = Normal): p_(p), del_(del) {}

    protected:
        virtual void Delete(){ if(p_) del_(p_); }

    private:
        Ty* p_; void (*del_)(Ty*);
    };

    // destructor
    ~Ptr(){ this->reset(); }

    // default constructor
    Ptr(): p_(0), info_(0)
    {}

    // new
    template<class Other>
    explicit Ptr(Other* p) : p_(p), info_(new CInfo(p))
    {}

    template<class Other>
    explicit Ptr(Other* p, void (*del)(Ty*))
    : p_(p), info_(new CInfo(p, del))
    {}

    // non garbage collection
    template<class Other>
    Ptr(Other* p, const Ptr_DoNotOwn&) : p_(p), info_(0)
    {}

    // copy
    Ptr(const Ptr& sp)
    { this->Share(sp); }

    template<class Other>
    Ptr(const Ptr<Other>& sp)
    { this->Share(sp); }

    // down cast
    template<class Other>
    Ptr(const Ptr<Other>& sp, const Ptr_StaticCast&)
    { this->Share_StaticCast(sp); }

    template<class Other>
    Ptr(const Ptr<Other>& sp, const Ptr_DynamicCast&)
    { this->Share_DynamicCast(sp); }

    // operator=
    Ptr& operator=(const Ptr& sp)
    { this->Release(); this->Share(sp); return *this; }

    template<class Other>
    Ptr& operator=(const Ptr<Other>& sp)
    { this->Release(); this->Share(sp); return *this; }

    // public members
    void swap(Ptr& sp)
    {
        Ty* t = p_; p_ = sp.p_; sp.p_ = t;
        _common_CPtrInfo* s = info_; info_ = sp.info_; sp.info_ = s;
    }

    void reset()
    {  this->Release(); p_ = 0; info_ = 0; }

    template<class Other>
    void reset(Other* p)
    {  this->Release(); p_ = p; info_ = new CInfo(p); }

    template<class Other>
    void reset(Other* p, void (*del)(Ty*))
    {  this->Release(); p_ = p; info_ = new CInfo(p, del); }

    template<class Other>
    void reset(Other* p, const Ptr_DoNotOwn&)
    {  this->Release(); p_ = p; info_ = 0; }

    // pointer operations
    Ty *get() const { return p_; }
    Ty& operator*() const { return *p_; }
    Ty *operator->() const { return p_; }

    // smart-ptr info
    long use_count() const { return info_ ? info_->Count() : 0; }
    bool unique() const { return info_? (info_->Count() == 1) : false; }

    // enable 'if(p)' and 'if(!p)'
    // but not 'delete p;'
    operator BoolType() const { return BoolTypeCast(p_); }

//_____________________________________________
// followings are public, but do not use them.
public:
    template<class Other>
    void Share(const Ptr<Other>& sp)
    { p_ = sp.p_; info_ = sp.info_; if(info_) info_->AddRef(); }

    template<class Other>
    void Share_StaticCast(const Ptr<Other>& sp)
    {
        p_ = static_cast<Ty*>(sp.p_);
        info_ = sp.info_;
        if(info_) info_->AddRef();
    }

    template<class Other>
    void Share_DynamicCast(const Ptr<Other>& sp)
    {
        p_ = dynamic_cast<Ty*>(sp.p_);
        if(p_){
            info_ = sp.info_; if(info_) info_->AddRef();
        }
        else{
            info_ = 0;
        }
    }

    void Release()
    {
        if(info_) if(info_->SubRef()){
            delete info_;
        }
    }

    Ty* p_;
    _common_CPtrInfo* info_;
};


//____________________________________________________________________________
// raw-ptr to smart-ptr caster
// You can use it like:
//     foo(Gc(new MyClass())); ('Gc' stands for 'garbage collection')
// instead of:
//     foo(Ptr<MyClass>(new MyClass()));
template <class Ty>
inline Ptr<Ty> Gc(Ty* p){ return Ptr<Ty>(p); }


//____________________________________________________________________________
// raw-ptr to smart-ptr caster for ARray
// You can use it like:
//     Ptr<int> a = ArGc(new int[1000])
// instead of:
//     Ptr<int> a = Ptr<int>(new int[1000], Ptr<int>::Array)
template <class Ty>
inline Ptr<Ty> ArGc(Ty* p){ return Ptr<Ty>(p, Ptr<Ty>::Array); }


//____________________________________________________________________________
// raw-ptr to smart-ptr caster with no garbage collection
template <class Ty>
inline Ptr<Ty> NoGc(Ty* p){ return Ptr<Ty>(p, Ptr_DoNotOwn()); }


//____________________________________________________________________________
// smart-ptr to raw-ptr caster
// It is recommended that you use:
//     foo(Raw(p));
// instead of:
//     foo(p.get());
// because 'p.get()' and 'p->get()' is hardly distinguishable
// when *p has a method 'get'.
template <class Ty>
inline Ty* Raw(const Ptr<Ty>& p){ return p.get(); }


//____________________________________________________________________________
// manual delete of smart-ptr
// It is recommended that you use:
//     Del(p);
// instead of:
//     p.reset();
// because 'p.reset()' and 'p->reset()' is hardly distinguishable
// when *p has a method 'reset'.
template <class Ty>
inline void Del(Ptr<Ty>& p){ p.reset(); }

//____________________________________________________________________________
// smart-ptr down casters
template <class Tto, class Tfrom>
inline Ptr<Tto> StaticCast(const Ptr<Tfrom>& p)
{
    return Ptr<Tto>(p, Ptr_StaticCast());
}

template <class Tto, class Tfrom>
inline Ptr<Tto> DynamicCast(const Ptr<Tfrom>& p)
{
    return Ptr<Tto>(p, Ptr_DynamicCast());
}


//__________________________________________________________________________
// type selecter
template <bool B, typename T, typename F>
struct type_select;

template <typename T, typename F>
struct type_select <true, T, F>
{
    typedef T  val;
};

template <typename T, typename F>
struct type_select <false, T, F>
{
    typedef F  val;
};


//__________________________________________________________________________
// asserter
#define CT_ASSERT(description__, condition__)                   \
    enum { ct_assert ## description__ ## _ ## __LINE__ ## __    \
                = sizeof( type_assert <(condition__)> )         \
    }

template <bool B>
struct type_assert;

template <>
struct type_assert<true>
{ int dummy; };

CT_ASSERT(IntIsWiderThanChar, sizeof(int) > sizeof(char)        );
CT_ASSERT(CharIsOctet       , (int)(unsigned char)0x180 == 0x80 );

//__________________________________________________________________________
// uint8 - uint64
template <unsigned size, bool bSigned>
struct size_to_type
{
    typedef
        typename type_select < (sizeof(char) == size),
            typename type_select<bSigned, signed char, unsigned char>::val,
        typename type_select < (sizeof(short) == size),
            typename type_select<bSigned, short, unsigned short>::val,
        typename type_select < (sizeof(int) == size),
            typename type_select<bSigned, int, unsigned int>::val,
        typename type_select < (sizeof(long) == size),
            typename type_select<bSigned, long, unsigned long>::val,
        typename type_select < (sizeof(long long) == size),
            typename type_select<bSigned, long long, unsigned long long>::val,
        char
        >::val>::val>::val>::val>::val      val;

    CT_ASSERT(SizeToTypeExists, sizeof(val) == size);
};

typedef size_to_type<1, false>::val  uint8;
typedef size_to_type<1, true >::val  sint8;
typedef size_to_type<2, false>::val  uint16;
typedef size_to_type<2, true >::val  sint16;
typedef size_to_type<4, false>::val  uint32;
typedef size_to_type<4, true >::val  sint32;
typedef size_to_type<8, false>::val  uint64;
typedef size_to_type<8, true >::val  sint64;


//__________________________________________________________________________
// File pointer holder
class File
{
public:
    File(): f_() {}
    explicit File(std::FILE* f): f_(f) {}
    ~File(){ this->Close(); }

    void Close(){
        if(f_){
            std::fclose(f_);
            f_ = NULL;
        }
    }

    std::FILE* get() const { return f_; }

private:
    File(File const&);
    void operator=(File const&);

    std::FILE* f_;
};


} // namespace fitmb
#endif //fitmb_Common_hpp__
