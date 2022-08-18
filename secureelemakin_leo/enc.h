#ifndef FILE_ENC_H
#define FILE_ENC_H

#define DEBUG

#ifdef __linux__ 
    #include <cstdint> //uint8_t,
    #include <cstdlib> //malloc, free
    #include <iostream>
    #define logprint(x) std::cout<<x;
#endif
#ifdef ARDUINO
    #include "Arduino.h"
    #define logprint(x) Serial.print(x);
#endif


#include "randrange.h"

#include "Crypto.h"
#include "AES.h"
#include "GCM.h"
#include "SHA256.h"
#include "HKDF.h"


class EncClass {
    public:
    EncClass();

    static void set_password(uint8_t *password, uint8_t length);
    static void clear();
    static void clear_block();
    
    static uint8_t encrypt_gcm();//fullstream encrypt with GCM + AES
    static uint8_t encrypt_gcm(uint8_t blocksize);//fullstream encrypt with GCM + AES
    static bool decrypt_gcm(uint8_t tag);//fullstream decrypt with GCM + AES
    static bool decrypt_gcm(uint8_t tag,uint8_t blocksize);//fullstream decrypt with GCM + AES
    static GCM<AESTiny128> *gcm;
    static HKDF<SHA256> *hkdf;
    static void set_salt(uint8_t *salt,uint8_t length);
    static void reattach_key_iv_auth();

    static const uint8_t blocksize;
    static const uint8_t keysize;
    static const uint8_t ivsize;
    static const uint8_t authsize;
    static uint8_t block_default_item();//return default item, which is
    static bool is_block_default_item(uint8_t v);//return true if v is block default item
    static uint8_t *block;
    static uint8_t *key_iv_auth; //key, iv, and auth in one array, in order as its named key,iv,auth considering keysize,ivsize,authsize
    static uint8_t *key; //same pointer as key_iv_auth
    static uint8_t *iv;  //key_iv_auth+keysize
    static uint8_t *auth;  //key_iv_auth+keysize+ivsize
    
};
extern EncClass enc;

#endif
