#include "Keyboard.h"
#include "Wire.h"
#include "src/util_arraymalloc.h"


String textS0 = "";

bool is_serial_capturingpassword = false;

int8_t keyboard_echo_flag_pin = 6;

char* password;
int passwordlen;
#define passwordmaxlen 64

char* command = array(uint8_t,128);
int commandlen = 0;

class WireSend{
    public:
        char* data;
        int length;
        int pos;
        bool is_sending = false;
        bool free_on_done = false;//if true, after all *data sent, free(data) will be called
        
        WireSend(){};
        
        void send(int length, char* data, bool free_data_after_done){
            this->data = data;
            this->length = length;
            this->pos = 0;
            this->is_sending = false;
            this->free_on_done = free_data_after_done;
        }
        
        bool send_available(){
            if (pos >= length) {
                is_sending = false;
                length = 0;
                if (free_on_done) {
                    free(data);
                }
                return false;
            }
            return true;
        }
        
        bool send_next_char(){
            if (!is_sending){
                return false;
            }
            if (!send_available()){
                return false;
            }
            Wire.write(data[pos]);
            Serial.print(" s:");
            Serial.print(data[pos]);
            pos += 1;
            return send_available();
        }
};
WireSend wiresend;

void setup(){
    pinMode(keyboard_echo_flag_pin, INPUT);
    digitalWrite(keyboard_echo_flag_pin, LOW);
    delay(1000);
    Serial.begin(115200);
    //~ Serial1.begin(115200);
    Serial.println("ok0");
    //~ Serial1.println("ok1");
    Keyboard.begin();
    Wire.begin(1);//address=1 as slave, leonardo (D2 SDA), (D3 SCL). on esp: (D2 GPIO4 SDA) (D1 GPIO5 SCL)
    Wire.onReceive(wireOnReceive);
    Wire.onRequest(wireOnRequest);
}


void loop() {
    uint8_t chr;
    if (Serial.available()>0) {
        chr = (uint8_t)Serial.read();
        
        if (chr=='\n') {
            //line reading is done
            //will send data on the next ping from esp, 
            wiresend.send(commandlen,command,false);//asynchronous sending
            Serial.println("wiresend initialized");
            Serial.println(wiresend.length);
            commandlen = 0;
        }
        else {
            // Any number 0~255 is okay, except 254, it somehow could hang the 32u4 i2c
            // and the problem persist after hard reset / power reset.
            // can only be fixed after arduino bootloader's upload
            if (chr!=254) {
                command[commandlen] = chr;
                commandlen += 1;
            }
        }
    }
}

/**
 * esp8266 only has 1 usable full duplex tx rx already used by python repl. use wire instead.
 * unfortunately the esp8266 micropython only support master mode wire, so 32u4 will act as slave/peripheral
 * 
 * esp will ping request and 32u4 will answer with 1 byte variable (wiresend.length).
 * (wiresend.length) is the length of data that needs to be transmitted by 32u4
 * (wiresend.length) = 0 if 32u4 has nothing to transmit
 * (wiresend.length) > 0 if 32u4 has something to transmit,
 *      if previously sent data from wireOnRequest has (wiresend.length)>0, esp will request data again and
 *      the next wireOnRequest event will just send the data with length (wiresend.length)
 * 
 * secure data is stored in esp8266
 * keyboard USB HID and usb interface is handled by 32u4
 * 32u4 may transmit:
 *      securedata-id requested by USB-Host connected in 32u4's USB 
 * 
 * esp8266 may transmit:
 *      securedata typing initiated by 32u4
 *      securedata typing initiated by esp8266's own web service
 */

bool is_printable(uint8_t c){
    if (c==9 || c==10 || c==13){
        return true;
    }
    else if (c>=32 && c<=127){
        return true;
    }
    else {
        //non printable ascii data
        return false;
    }
}
bool is_printable(int8_t c){
    return is_printable((uint8_t)c);
}
bool is_printable(char c){
    return is_printable((uint8_t)c);
}

void wireOnRequest(){
    if (wiresend.is_sending){
        //send the data
        while(wiresend.send_next_char());
    }
    else {
        Wire.write(wiresend.length);
        //will send data on next wireOnRequest
        if (wiresend.length>0){
            Serial.print(wiresend.length);
            Serial.println(" begin sending data to esp");
            wiresend.is_sending = true;
        }
    }
}

void wireOnReceive(int received_length){
    while (Wire.available()) {
        char c = Wire.read();
        if (digitalRead(keyboard_echo_flag_pin)==HIGH) {
            if (is_printable(c)){
                Keyboard.print(c);
            }
            else {
                //non printable ascii data
            }
        }
        else {
            //flag pin is not high, echo back to USB-Host
            if (is_printable(c)){
                Serial.write(c);
            }
        }
    }
    Serial.println("wireOnReceiv done");
}


