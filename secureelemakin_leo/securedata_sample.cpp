//author: Muhammad Izzulmakin. copyright(c) 2022
/**
 * sample securedata
 * copy this file and edit the data as needed
 * and adjust the #define SECUREDATA_SAMPLE  in securedata.h
 * according to the #ifdef in this code bellow
 */
#include "securedata.h"
#include "autotyper.h"

//after incldue securedata.h
#ifdef SECUREDATA_SAMPLE

const PROGMEM uint8_t data_h1[] = {
    146, 175, 60, 246, 248, 87, 198, 2, 105, 1, 41, 250, 64, 200, 170, 105, 44, 109, 37, 18, 63, 87, 168, 212, 10, 255, 187, 142, 104, 222, 110, 108, 26, 40, 105, 180, 59, 77, 76, 121, 202, 92, 241, 79, 236, 90, 248, 123, 
};
const PROGMEM uint8_t data_h2[] = {
    2, 184, 64, 27, 248, 25, 115, 193, 188, 1, 221, 151, 153, 174, 13, 31, 86, 247, 89, 121, 105, 232, 213, 182, 224, 65, 254, 100, 171, 193, 117, 221, 158, 156, 133, 111, 41, 83, 192, 187, 202, 95, 135, 169, 41, 202, 55, 23, 
};


const uint8_t securedatas_size = 3;
SecureData securedatas[] = { //malloc(sizeof(SecureData) * securedatas_size);
    SecureData(data_h1,48,"hello1"),
    SecureData(data_h2,48,"hello2"),
    
};

const uint8_t autotypers_act_ENTER_ENTER[]    = {AUTOTYPER_ENTER,AUTOTYPER_ENTER};
const uint8_t autotypers_helloi[] = {0,1};
Autotyper autotypers[] = {
    Autotyper(
        autotypers_helloi,
        autotypers_act_ENTER_ENTER,
        2,
        ("helloworld")
    )
};
const uint8_t autotypers_size = sizeof(autotypers)/sizeof(Autotyper);


#endif
