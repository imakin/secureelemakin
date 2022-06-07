#include "enc.h"
#define enc_blocksize 16
#define enc_datasize 160
const uint8_t enc::key_default_item = 0;
const uint8_t enc::block_default_item = 0;
#define _k enc::key_default_item
#define _b enc::block_default_item

uint8_t enc::key[enc_blocksize] =
    {_k, _k, _k, _k, _k, _k, _k, _k, _k, _k, _k, _k, _k, _k, _k, _k};
uint8_t enc::data[enc_datasize] = {
    _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b,
    _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b,
    _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b,
    _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b,
    _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b,
    _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b,
    _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b,
    _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b,
    _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b,
    _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b
};
uint8_t enc::block[enc_blocksize] = {
    _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b, _b
};
#undef _k
#undef _b

void enc::padkey(){
    uint8_t i=0;
    //prefil pad
    for (i=0;i<enc_blocksize;i++){
        enc::key[i] = enc::key_default_item;
    }
}
void enc::setkey(uint8_t* text){
    uint8_t i=0;
    enc::padkey();
    uint8_t length = sizeof(text)/sizeof(uint8_t);
    for (int i=0;i<length;i++){
        enc::key[i] = text[i];
        if (i>=enc_blocksize) {
            break;//only up to 16 byte, ignore the rest
        }
    }
}
void enc::setkey(String text){
    uint8_t i=0;
    uint8_t length = text.length();
    for (int i=0;i<length;i++){
        enc::key[i] = text.charAt(i);
        if (i>=enc_blocksize) {
            break;//ignore the rest
        }
    }
}
//return length, buffer will be data+space padding, make it array of char
uint32_t enc::test_encrypt(String data){
    uint32_t length = data.length();
    uint32_t additional_count = 0;
    if (length<enc_blocksize) {
        additional_count = enc_blocksize - length;
    }
    else {
        additional_count = enc_blocksize - (length%enc_blocksize);
    }
    uint32_t new_length = length+additional_count;
    Serial.print("new length:");
    Serial.println(new_length);
    uint8_t* buffer = malloc(new_length*sizeof(uint8_t));
    for (uint32_t i=0;i<new_length;i++){
        if (i<length) {
            buffer[i] = data.charAt(i);
        }
        else {
            buffer[i] = 98;//space charracter
        }
    }
    Serial.print("before enc:");
    for (uint32_t i=0;i<new_length;i++){
        Serial.print(buffer[i]);
        Serial.print(" ");
    }
    Serial.print('\n');
    Serial.print("after enc:");
    aes128_enc_single(enc::key, buffer);
    for (uint32_t i=0;i<new_length;i++){
        Serial.print(buffer[i]);
        Serial.print(" ");
    }
    Serial.print('\n');
    aes128_dec_single(enc::key, buffer);
    Serial.print("after dec:");
    for (uint32_t i=0;i<new_length;i++){
        Serial.print(buffer[i]);
        Serial.print(" ");
    }
    Serial.print('\n');
    return new_length;
}
