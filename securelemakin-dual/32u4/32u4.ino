#include "Keyboard.h"
String textS0 = "";
String textS1 = "";
bool will_do_keyboard;
void setup(){
    delay(1000);
    Serial.begin(115200);
    Serial1.begin(115200);
    Serial.println("ok0");
    Serial1.println("ok1");
    Keyboard.begin();
}


void loop() {
    uint8_t chr;
    if (Serial.available()>0) {
        chr = (uint8_t)Serial.read();
        Serial1.write(chr);
    }
    if (Serial1.available()>0){
        chr = (uint8_t)Serial1.read();
        if (will_do_keyboard) {
            will_do_keyboard = false;
            Keyboard.write(chr);
            delay(100);
        }
        else if  (chr==254){
            will_do_keyboard = true;
        }
        else {
            Serial.write(chr);
        }
    }
}



