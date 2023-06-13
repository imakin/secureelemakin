//author: Muhammad Izzulmakin. copyright(c) 2023
#ifndef SECUREDATAFLASH_H
#define SECUREDATAFLASH_H

#include "Arduino.h"
#include "LittleFS.h"

class SecureDataFlash{
    public:
        char* name;
        int pos;//byte position of chiper data in securedata_file
        int length;//length of chiper data in securedata_file
        SecureDataFlash* prev;//previous data linked list
        SecureDataFlash* next;//next data
        SecureDataFlash(){
            prev = 0;
            next = 0;
        };
        
        int read(uint8_t* target_buffer);
};

#endif
