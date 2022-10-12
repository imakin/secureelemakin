//author: Muhammad Izzulmakin. copyright(c) 2022
#include "test.h"
#define ENABLE_TESTING
#ifdef ENABLE_TESTING

#include "button.h"
#include "charmenu.h"
#include "enc.h"
#include "securedata.h"
#include <SHA256.h>
#include <HKDF.h>
#include "readstringuntil.h"
#include "autotyper.h"

#define echo Serial.print
#define echoln Serial.println
#define br Serial.println

#define array(tipe,n) (tipe *)malloc(n*sizeof(tipe))
#define array8(n) (uint8_t *)malloc((n)*sizeof(uint8_t))

void assert_equal(int a, int b){
    if (a!=b) {
        Serial.print("test failed: expecting ");
        Serial.print(a);
        Serial.print("==");
        Serial.println(b);
    }
    else {
        Serial.print("test equal ok ");
        Serial.print(a);
        Serial.print("==");
        Serial.println(b);
    }
}

void test_thumb(){
    //~ autotyper_execute();
}

void dotest(){
    test_thumb();
}

#endif
