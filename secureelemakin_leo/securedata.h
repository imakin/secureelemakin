//author: Muhammad Izzulmakin. copyright(c) 2022
#ifndef SECUREDATA_H
#define SECUREDATA_H

#define SECUREDATA_SAMPLE //link securedata_sample.cpp instead
//~ #define SECUREDATA_MAKIN //link securedata_makin.cpp instead

#include <avr/pgmspace.h>
//store encrypted data, and the length
//length must always multiply of 16


class SecureData {
    public:
        SecureData(uint8_t *address, uint8_t length, uint8_t *name);
        static uint8_t getDataAt(SecureData securedata_target, uint8_t i);//save SRAM with static
        uint8_t *address;
        uint8_t length;
        uint8_t *name;//can be 0 if unused
};
extern const uint8_t securedatas_size;
extern SecureData securedatas[];


#endif

