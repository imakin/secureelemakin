#include "randrange.h"

#ifdef __linux__ 
    template<typename T>
    T randrangeT(T range_from, T range_to) {
        std::random_device                  rand_dev;
        std::mt19937                        generator(rand_dev());
        std::uniform_int_distribution<T>    distr(range_from, range_to);
        return distr(generator);
    }
    uint8_t randrange(uint8_t min,uint8_t max) {
        return (uint8_t)randrangeT(min,max);
    }
#elif defined ARDUINO
    uint8_t randrange(uint8_t min,uint8_t max) {
        randomSeed(analogRead(1));
        return random(min,max);
    }
#endif

uint64_t rand64(){
    uint64_t r = 0;
    for (int i=0;i<7; i++){
        r = r*randrange(1,255);
    }
    return r;
}
