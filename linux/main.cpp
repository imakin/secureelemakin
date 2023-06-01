/**
 * linux version but using the same crypto library,
 * for building encrypted data in securedata_*.cpp
 * and for unittest and sandbox testing
 */
 
#include <iostream>
#include <cstdint>
    
#include "Crypto.h"
#include "AES.h"
#include "GCM.h"
#include "SHA256.h"
#include "HKDF.h"

#include "enc.h"
#include "password.h"
#include "randrange.h"

#define ZERO 0

#define array8(n) (uint8_t *)malloc((n)*sizeof(uint8_t))

void echoln(){
    std::cout<<"\n";
}
void echo(std::string str){
    std::cout<<str;
}
void echo(bool b){
    std::cout<<b;
}
void echo(uint8_t c){
    std::cout<<(int)c;
}

void make_securedata(bool debug);
void test_enc();
void test_randrange();

/**
 * similar to Serial.readStringUntil, but use predefined output buffer and max length
 * to save SRAM. without the need of reinstatiating String when the SRAM already have the buffer to use
 * @param terminator(uint8_t): terminator charracter of read-string-until (typical value: '\n')
 * @param *output (uint8_t*): output buffer 
 * @param max_length (uint8_t): stop reading and consider 'until' is reached if total read already reached max_length (typical value: length of output buffer)
 * @param start_index (uint8_t): start index when writing to *output (typical value: 0)
 * @return (uint8_t) length/how many charracters it actually writes into output buffer
 */
uint8_t readStringUntil(uint8_t terminator, uint8_t *output, uint8_t max_length, uint8_t start_index);

int main(int argc, char *argv[]){
    bool verbose = false;
    for (int i=0;i<argc;i++){
        std::cout<<i<<":"<<argv[i]<<"\n";
    }
    if (argc>=2 && std::string(argv[1]).compare("-v")==0){
        std::cout<<"verbose\n";
        verbose = true;
    }
    //~ test_randrange();
    make_securedata(verbose);
    return (0);
}

void make_securedata(bool debug){
    if (debug) std::cout<<("making securedata_makin.cpp. input password:\n");
    uint8_t *pwd = array8(200);
    uint8_t pwd_len = readStringUntil('\n',pwd,200,0);
    enc.set_password(pwd, pwd_len);
    free(pwd);
    if (debug) {
        std::cout<<("key_iv_auth: ");
        for (uint8_t i=0;i<(enc.keysize+enc.ivsize+enc.authsize);i++){
            std::cout<<(enc.key_iv_auth[i]+ZERO);
            std::cout<<(", ");
        }std::cout<<("\n");
        std::cout<<("key: \n");
        for (uint8_t i=0;i<enc.keysize;i++){
            std::cout<<(enc.key[i]+ZERO);
            std::cout<<(", ");
        }std::cout<<("\n");
        std::cout<<("iv: \n");
        for (uint8_t i=0;i<enc.ivsize;i++){
            std::cout<<(enc.iv[i]+ZERO);
            std::cout<<(", ");
        }std::cout<<("\n");
        std::cout<<("auth: \n");
        for (uint8_t i=0;i<enc.authsize;i++){
            std::cout<<(enc.auth[i]+ZERO);
            std::cout<<(", ");
        }std::cout<<("\n");
    }
    enc.clear_block();
    
    
    
    if (debug) std::cout<<("input string to be encrypted: ");
    //~ enc.block[0] = enc.block_default_item();
    //~ enc.block[1] = enc.block_default_item();
    uint8_t length = readStringUntil('\n',enc.block,enc.blocksize,0);
    uint8_t blocklength = 16;
    while (blocklength < enc.blocksize){
        if (length >= (blocklength-8)) { //even if length==blocklength-8, use next increased length for better stirring security
            blocklength += 16;
        }
        else {
            break;
        }
    }
    if (debug) std::cout<<("to optimize load, it could be 16,32, or 48. length is: ");
    if (debug) std::cout<<(int)(blocklength);
    if (debug) std::cout<<"\n";
    
    
    if (debug) {
        std::cout<<("before encrypted: ");
        for (uint8_t i=0;i<blocklength;i++){
            std::cout<<(int)(enc.block[i]);
            std::cout<< (", ");
        }std::cout<<"\n";
    }
    
    uint8_t tag = enc.encrypt_gcm(blocklength);
    if (debug)  {
        std::cout<<("after encrypted: ");
        for (uint8_t i=0;i<blocklength;i++){
            std::cout<<(int)(enc.block[i]);
            std::cout<< (", ");
        }std::cout<<"\n";
        std::cout<<("tag:");
        std::cout<<(tag);
        std::cout<<"\n";
        std::cout<<"\n";
    }
    //output code
    if (debug) std::cout<<"metacoding\n\n";
    std::cout<<"const PROGMEM uint8_t data_{SOMETHING}[] = {\n";
    std::cout<<"    ";
    for (uint8_t i=0;i<blocklength;i++){
        std::cout<<(int)(enc.block[i]);
        std::cout<< (", ");
    }
    std::cout<<"\n};\n";
    //output code
    if (debug) std::cout<<"for esp flash:\n\n";
    for (uint8_t i=0;i<blocklength;i++){
        std::cout<<(char)(enc.block[i]);
    }
    std::cout<<"\n\n";
    
    
    
    if (debug) std::cout<<"\n";
    if (debug) std::cout<<"\n";
    bool ok = enc.decrypt_gcm(tag+1,blocklength);
    std::cout<<("after decrypted: ");
    for (uint8_t i=0;i<blocklength;i++){
        std::cout<<(int)(enc.block[i]);
        std::cout<< (", ");
    }std::cout<<"\n";
    if (debug) std::cout<<("tag ok?:");
    if (debug) std::cout<<(ok);
    if (debug) std::cout<<"\n";
    for (uint8_t i=0;i<blocklength;i++){
        std::cout<<(char)(enc.block[i]);
    }std::cout<<"\n";
    
}

void test_randrange(){
    std::cout<<randrange( 48,122)<<" ";
    std::cout<<randrange( 48,122)<<" ";
    std::cout<<randrange( 48,122)<<" ";
    std::cout<<randrange( 48,122)<<" ";
    std::cout<<"\n";
}

/**
 * similar to Serial.readStringUntil, but use predefined output buffer and max length
 * to save SRAM. without the need of reinstatiating String when the SRAM already have the buffer to use
 * @param terminator(uint8_t): terminator charracter of read-string-until (typical value: '\n')
 * @param *output (uint8_t*): output buffer 
 * @param max_length (uint8_t): stop reading and consider 'until' is reached if total read already reached max_length (typical value: length of output buffer)
 * @param start_index (uint8_t): start index when writing to *output (typical value: 0)
 * @return (uint8_t) length/how many charracters it actually writes into output buffer
 */
uint8_t readStringUntil(uint8_t terminator, uint8_t *output, uint8_t max_length, uint8_t start_index) {
    uint8_t c;
    uint8_t i=start_index;
    uint8_t str_i = 0;
    uint8_t maximum_length = max_length;
    
    std::string str;
    std::getline(std::cin, str);
    if ((uint8_t)str.length()<maximum_length) {
        maximum_length = (uint8_t)str.length();
    }
    std::cout<<"maximum length is "<<(int)maximum_length;
    
    for(;;){
        c = (uint8_t)str.at(str_i);
        if (c==terminator) {
            //~ std::cout<<"muncul "<<terminator<<"\n";
            break;
        }
        output[i] = c;
        i++;
        str_i++;
        if ((i-start_index)>=maximum_length) {//length = i-start_index;
            break;
        }
    }
    return (i-start_index);
}
