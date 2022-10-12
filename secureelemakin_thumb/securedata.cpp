//author: Muhammad Izzulmakin. copyright(c) 2022
#include "securedata.h"

SecureData::SecureData(uint8_t *address, uint8_t length, uint8_t *name) {
    this->address = address;
    this->length = length;
    this->name = name;
}
uint8_t SecureData::getDataAt(SecureData securedata_target, uint8_t i) {
    return (uint8_t)pgm_read_word(
        &(securedata_target.address[i])
    );
}
