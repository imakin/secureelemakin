//author: Muhammad Izzulmakin. copyright(c) 2022
#ifndef SECUREDATA_H
#define SECUREDATA_H

#define SECUREDATA_SAMPLE //link securedata_sample.cpp instead
//~ #define SECUREDATA_MAKIN //link securedata_makin.cpp instead

#include <avr/pgmspace.h>
//store encrypted data, and the length
//length always multiply of 16


class SecureData {
    public:
        SecureData(uint8_t *address, uint8_t length, uint8_t *name);
        uint8_t getDataAt(uint8_t i);
        uint8_t *address;
        uint8_t length;
        uint8_t *name;
};
extern const uint8_t securedatas_size;
extern SecureData securedatas[];

#endif

