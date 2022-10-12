#include "2fa.h"

uint8_t* otpkey = (uint8_t *)malloc((32)*sizeof(uint8_t));
//make otp instance
TOTP * totp = new TOTP(otpkey, 32);

void otp_typer(long utctimestamp, SecureData securedata_target) {
    int j=0;
    //get secure data and decrypt it
    enc.clear_block();
    for (j=0;j<(int)securedata_target.length;j++){
        enc.block[j] = SecureData::getDataAt(securedata_target,j);//securedata_target.getDataAt(j);
    }
    enc.decrypt_gcm(99,securedata_target.length);
    
    //make buffer with size length and fill it with decrypted
    int pos = 0;
    for (j=0;j<(int)securedata_target.length;j++){
        if (false==EncClass::is_block_default_item(enc.block[j])){
            otpkey[pos] = enc.block[j];
            Serial.write((char)otpkey[pos]);
            pos += 1;
        }
    }
    //pos == length
    enc.clear();
    Serial.print('\n');
    
    //reset length
    totp->setHmacLength(pos);
    
    char* code = totp->getCode(utctimestamp);
    //*code is singleton buffer from TOTP.h/cpp
    //contain ascii/utf8 encoded charracter
    Serial.print("OTP CODE: ");
    for (int pos=0;pos<6;pos++){
        #ifdef ARDUINO
            Keyboard.print((char)code[pos]);
        #endif
        Serial.write((char)code[pos]);
    }
    Serial.println();
    free(code);
}
