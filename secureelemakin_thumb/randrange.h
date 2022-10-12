#ifndef FILE_RANDRANGE_H
#define FILE_RANDRANGE_H

#ifdef __linux__ 
    #include <cstdint> //uint8_t,
    #include <cstdlib> //malloc, free
    #include <random>
#endif
#ifdef ARDUINO
    #include "Arduino.h"
    //will use arduino random with the noise from analog pin as the seed
#endif

uint8_t randrange(uint8_t min,uint8_t max);


#endif

