// OpenAuthentication Time-based One-time Password Algorithm (RFC 6238)
// Arduino Library
//
// Luca Dentella (http://www.lucadentella.it)

#include "Arduino.h"

#ifndef _TOTPMKN_H
#define _TOTPMKN_H


class TOTP {

	public:
	
		TOTP(uint8_t* hmacKey, int keyLength);
		TOTP(uint8_t* hmacKey, int keyLength, int timeStep);
		char* getCode(long timeStamp);
		char* getCodeFromSteps(long steps);
		//change hmacKey
		void setHmac(uint8_t *hmacKey, int keyLength);
		//change hmac keyLength, useful if hmacKey is still using the same buffer
		void setHmacLength(int keyLength);
	
	private:

		uint8_t* _hmacKey;
		int _keyLength;
		int _timeStep;
		uint8_t _byteArray[8];
		uint8_t* _hash;
		int _offset;
		long _truncatedHash;
		char _code[7];
};

#endif
