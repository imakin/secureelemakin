#include "Keyboard.h"
#include "Wire.h"
String textS0 = "";
String textS1 = "";
bool will_do_keyboard;
char last_byte;

void setup(){
    delay(1000);
    Serial.begin(115200);
    Serial1.begin(115200);
    Serial.println("ok0");
    //~ Serial1.println("ok1");
    Keyboard.begin();
    Wire.begin(1);//address=1 as slave, leonardo D2 SDA, D3 SCL
    Wire.onReceive(wireOnReceive);
    Wire.onRequest(wireOnRequest);
}


void loop() {
    uint8_t chr;
    if (Serial.available()>0) {
        chr = (uint8_t)Serial.read();
        Serial1.write(chr);
    }
    if (Serial1.available()>0){
        chr = (uint8_t)Serial1.read();
        Serial.write(chr);
    }
}

void wireOnRequest(){
    Wire.write(last_byte+1);
}

void wireOnReceive(int len){
    while (Wire.available()) {
        last_byte = Wire.read();
    }
}


