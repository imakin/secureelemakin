#ifndef H_2FA_H
#define H_2FA_H

#include "enc.h"
#include "securedata.h"
#ifdef ARDUINO
    #include "Keyboard.h"
#endif
//https://github.com/lucadentella/TOTP-Arduino
#include "TOTPmkn.h"


void otp_typer(long utctimestamp, SecureData securedata_target);
extern uint8_t* otpkey;
extern TOTP* totp;

//~ uint8_t * otpkey = (uint8_t *)malloc((32)*sizeof(uint8_t));
//~ //make otp instance
//~ TOTP *totp = new TOTP(otpkey, 32);
#endif
