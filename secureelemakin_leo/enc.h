#ifndef FILE_EN_H
#define FILE_EN_H

#include "Arduino.h"
#include <AESLib.h>
#include "button.h"


namespace enc {
    extern void padkey();
    extern void setkey(uint8_t*);
    extern void setkey(String);
    extern uint32_t test_encrypt(String);
    
    extern const uint8_t key_default_item;
    extern const uint8_t block_default_item;
    extern uint8_t key[];
    extern uint8_t data[];
    extern uint8_t block[];
}


#endif
