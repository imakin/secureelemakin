//author: Muhammad Izzulmakin. copyright(c) 2022
/**
 * sample securedata
 * copy this file and edit the data as needed
 * and adjust the #define SECUREDATA_SAMPLE  in securedata.h
 * according to the #ifdef in this code bellow
 */
#include "securedata.h"

//after incldue securedata.h
#ifdef SECUREDATA_SAMPLE

const uint8_t securedatas_size = 2;
const PROGMEM uint8_t data_sample_0[] = {
    23, 65, 143, 230, 171, 167, 25, 212, 34, 44, 11, 190, 183, 229, 193, 69
};
const PROGMEM uint8_t data_sample_1[] = {
    184, 64, 240, 173, 174, 36, 171, 14, 87, 209, 182, 119, 9, 42, 83, 127
};

SecureData securedatas[] = { //malloc(sizeof(SecureData) * securedatas_size);
    SecureData(data_sample_0,16,"data sample no-0"),
    SecureData(data_sample_1,16,"data sample #1  ")
};



#endif
