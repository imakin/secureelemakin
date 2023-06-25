//securelemakindual
//for esp8266 where encrypted data stored
//using weak encryption for optimized performance
//@author: makin
//@date: 2023
#include <ESP8266WiFi.h>
#include "src/NTPClient/NTPClient.h"
#include <WiFiUdp.h>
#include "LittleFS.h"
#include "securedataflash.h"
#include "src/util_arraymalloc.h"
#include "src/securelemakin-enc/enc.h"
#include "src/TOTP-Arduino/src/TOTP.h"
#include "secret.h"
Secret secret;
int filesize = 0;

WiFiServer server(80);

IPAddress staticIP(192,168,1,2);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WiFiUDP udp;
//~ NTPClient timeClient(udp, (7*60*60)+(0*60) ); //UTC+7:00
NTPClient timeClient(udp, (int)0 ); //turn out otp timer is based on UTC?

SecureDataFlash* firstitem;

void setupWifi(){
    //~ WiFi.config(staticIP, gateway, subnet);
    WiFi.begin(secret.wifissid, secret.wifipassword);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    timeClient.begin();
}

void setup(){
    delay(4000);
    Serial.begin(115200);
    setupWifi();
    
    if (!LittleFS.begin()) {
        Serial.println("Failed to mount file system");
    }
    
    /**
     * File methods described in 
     * https://reference.arduino.cc/reference/en/language/functions/communication/stream/
     * and 
     * https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html#file-object
     */
    File securefile = LittleFS.open("/securedata","r");
    int file_len = securefile.available();
    //scan securedata
    char* temp = array(char,32);//max 32 bytes
    SecureDataFlash* item = new SecureDataFlash();
    firstitem = item;
    int name_len = 0;
    int chiper_pos;
    int chiper_len;
    while (securefile.available()) {
        name_len = securefile.readBytesUntil(' ',temp,32);
        item->name = array(char, name_len+1);
        for (int i=0;i<name_len;i++){
            item->name[i] = temp[i];
        }
        item->name[name_len+1-1] = 0;//null terminator for name string
        chiper_pos = securefile.position();
        while (securefile.available()){
            if (securefile.read()==0) {
                if (securefile.available() && securefile.read()==0){
                    if (securefile.available() && securefile.read()==0){
                        chiper_len = securefile.position() - 3 - chiper_pos;
                        break;
                    }
                }
            }
        }
        item->pos = chiper_pos;
        item->length = chiper_len;
        item->next = new SecureDataFlash();
        item->next->prev = item;
        item = item->next;
    }
    securefile.close();
    free(temp);
    //delete last item
    item->prev->next = 0;
    SecureDataFlash* nullitem = item;
    item = firstitem;
    delete nullitem;
    
    enc.set_password(secret.encryption_password,11);

    SecureDataFlash* otptko = secureitem_find("otp_tko");
    otptko->read(enc.block);
    enc.decrypt_gcm(0,(uint8_t)otptko->length);
    int readable_len = 0;
    for (int i=0;i<otptko->length;i++){
        Serial.write((char)enc.block[i]);
        if (enc.block[i]!=0) readable_len+=1;
    }
    Serial.print("\nreadable_len");
    Serial.println(readable_len);
    TOTP* totptko = new TOTP(enc.block,readable_len);
    
    
    //~ printWifiStatus();
    //~ while (!timeClient.forceUpdate()) {Serial.println("updating..");delay(1000);};
    while (1){
        delay(1000);
        timeClient.update();
        unsigned long timestamp = timeClient.getEpochTime();
        Serial.println(timestamp,10);
        Serial.println(totptko->getCode(timestamp));
        
    }
    server.begin();

}

bool parse(char* buffer, const char* comparison){
    int len = strlen(comparison);
    for (int i=0;i<len;i++){
        if (buffer[i]!=comparison[i]){
            return false;
        }
    }
    return true;
}

SecureDataFlash* secureitem_find(const char* name){
    SecureDataFlash* item = firstitem;
    while (item!=0){
        if (parse(item->name,name)){
            Serial.print(item->name);
            Serial.print("<--- selected");
            return item;
        }
        item = item->next;
        delay(10);
    }
    return 0;
}

void loop(){
    WiFiClient client = server.available();
    
    if (client) {
        Serial.println("new client");
        // an http request ends with a blank line
        bool currentLineIsBlank = true;
        bool firstLineParsed = false;
        while (client.connected()) {
            if (client.available()) {
                //first line contain the data we're looking for
                if (!firstLineParsed){
                    // first word is method, fetching GET/POST/PUT/DELETE/PATCH/HEAD/OPTION/ etc
                    //~ while (client.read()!=' ');
                    //~ int method_len = client.readBytesUntil(' ', (char*)enc.block, (int)enc.blocksize);
                    
                    // 2nd word is data
                    //~ int message_len = client.readBytesUntil(' ', (char*)enc.block, (int)enc.blocksize);
                    
                    Serial.println("command is: ");
                    char c = 0;
                    do{
                        c = client.read();
                        Serial.write(c);
                        Serial.print(' ');
                        Serial.println((int)c);
                    } while (c!='\n');
                    
                    // last word is http version
                    //~ while (client.read()!='\n');
                    firstLineParsed = true;
                }
                
                char c = client.read();
                Serial.write(c);
                
                
                // if you've gotten to the end of the line (received a newline
                // character) and the line is blank, the http request has ended,
                // so you can send a reply
                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");    // the connection will be closed after completion of the response
                    //~ client.println("Refresh: 5");    // refresh the page automatically every 5 sec
                    client.println();
                    client.println("<p>Hello world!!!</p>");
                    break;
                }
                if (c == '\n') {
                    // starting a new line, this new line is currentLine and is blank
                    currentLineIsBlank = true;
                } else if (c != '\r') {
                    // currentLine has anything other than \r and \n. so it is not blank
                    currentLineIsBlank = false;
                }
            }
        }
        // give the web browser time to receive the data
        delay(1);
        // close the connection:
        client.stop();
        Serial.println("client disconnected");
    }
}



void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}
