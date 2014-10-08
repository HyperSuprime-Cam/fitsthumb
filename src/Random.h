#ifndef  g72e0af32_b5cc_4515_9af3_1b30b7ca089a
#define  g72e0af32_b5cc_4515_9af3_1b30b7ca089a

/** Here's implemented XORSHIFT-ADD (XSadd),
    introduced by Mutsuo Saito and Makoto Matsumoto (2014)
    See http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/XSADD/index.html
*/

#include "Common.h"

namespace hsc { namespace fitsthumb {

class Random
{
public:
    Random(){
        state[0] = 0x72e0af32;
        state[1] = 0xb5cc4515;
        state[2] = 0x9af31b30;
        state[3] = 0xb7ca089a;
    }

    Random(uint32_t seed[4]){
        state[0] = seed[0];
        state[1] = seed[1];
        state[2] = seed[2];
        state[3] = seed[3];
    }

    uint32_t operator()() {
        const int sh1 = 15;
        const int sh2 = 18;
        const int sh3 = 11;
        uint32_t t = this->state[0];
        t ^= t << sh1;
        t ^= t >> sh2;
        t ^= this->state[3] << sh3;
        this->state[0] = this->state[1];
        this->state[1] = this->state[2];
        this->state[2] = this->state[3];
        this->state[3] = t;

        return this->state[3] + this->state[2];
    }

    double Double() {
        uint64_t a = (*this)();
        uint64_t b = (*this)();
        a = (a << 32) | b;
        return a * (1.0 / 18446744073709551616.0);
    }

    template <class Integer>
    Integer Int(Integer beg, Integer end){
        while(1){
            Integer val = beg + (Integer)((end - beg) * this->Double());
            if(val < end) return val;
        }
    }

private:
    uint32_t state[4];
};

}} // namespace hsc::fitsthumb
#endif //g72e0af32_b5cc_4515_9af3_1b30b7ca089a
