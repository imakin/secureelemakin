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

void test_securedata(){
    const uint8_t data_sample_[][32] = {
        {
            23, 65, 143, 230, 171, 167, 25, 212, 34, 44, 11, 190, 183, 229, 193, 69,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        },
        {
            184, 64, 240, 173, 174, 36, 171, 14, 87, 209, 182, 119, 9, 42, 83, 127,
            184, 64, 240, 173, 174, 36, 141, 14, 87, 209, 182, 119, 9, 42, 83, 127
        }
    };
    Serial.println("running automated test");
    Serial.println(" testing securedata.cpp and securedata_sample.cpp");
    Serial.println("  testing securedatas length");
    assert_equal(securedatas[0].length, 16);
    assert_equal(securedatas[1].length, 32);
    Serial.println("  testing EEPROM reading");
    for (uint8_t i=0;i<2;i++) {
        for (uint8_t j=0;j<securedatas[i].length;j++){
            uint8_t val = SecureData::getDataAt(securedatas[i],j);
            Serial.print("data_sample_");Serial.print(i);
            Serial.print("[");Serial.print(j);Serial.print("]");
            Serial.print(": ");Serial.println(val);
            assert_equal(val, data_sample_[i][j]);
        }
    }
}

extern CharMenuClass charmenu;
void test_charmenu_checkhover(){
    
}
void test_charmenu(){
    Serial.println("testing charmenu and securedata.h");
    Serial.println("testing if current menu is root->testscript");
    assert_equal(&dotest, charmenu.current_menu->action);
    Serial.println("adding submenu for root -> testscript-new");
    String menutext = "new";
    menutext += millis();
    menutext += "       ";
    uint8_t *menutext_arr = malloc(sizeof(uint8_t)*16);
    menutext.toCharArray(menutext_arr, 16);
    Menu *menu_testnew = new Menu(menutext_arr, charmenu.root);
    Serial.println("adding sub submenu");
    Menu* menu_test_datasample0 = new Menu(securedatas[0].name, menu_testnew);
    Menu* menu_test_datasample1 = new Menu(securedatas[1].name, menu_testnew);
    
    Serial.println("charmenu trigger next button clicked");
    charmenu.action_next();
    Serial.println("testing if menu is correctly created");
    assert_equal(charmenu.current_menu, menu_testnew);
    
    Serial.println("charmenu trigger enter button clicked");
    charmenu.action_enter();
    Serial.println("testing if menu has correct 1st children ");
    assert_equal(charmenu.current_menu, menu_test_datasample0);
    
    Serial.println("charmenu trigger next button clicked");
    charmenu.action_next();
    Serial.println("testing if menu has correct 2nd children ");
    assert_equal(charmenu.current_menu, menu_test_datasample1);
    
    
    Serial.println("charmenu trigger back button clicked");
    charmenu.action_back();
    Serial.println("testing if back to parent ");
    assert_equal(charmenu.current_menu, menu_testnew);
    
    Serial.println("charmenu trigger back button clicked");
    charmenu.action_back();
    Serial.println("testing if back to root ");
    assert_equal(charmenu.current_menu, charmenu.root);
    
    
    
}
extern uint8_t set_password_with_button_sequence(uint8_t first_input_timeout, uint8_t between_input_timeout, bool debug);
void test_enc(){
    echoln("testing enc.cpp");
    echoln("get password with button presses, if no more button pressed after 2 seconds, password input is considered done");
    set_password_with_button_sequence(10,2,true);

    echo("key_iv_auth: ");
    for (uint8_t i=0;i<(enc.keysize+enc.ivsize+enc.authsize);i++){
        echo(enc.key_iv_auth[i]);
        echo(", ");
    }echoln();
    echoln("key: ");
    for (uint8_t i=0;i<enc.keysize;i++){
        echo(enc.key[i]);
        echo(", ");
    }echoln();
    echoln("iv: ");
    for (uint8_t i=0;i<enc.ivsize;i++){
        echo(enc.iv[i]);
        echo(", ");
    }echoln();
    echoln("auth: ");
    for (uint8_t i=0;i<enc.authsize;i++){
        echo(enc.auth[i]);
        echo(", ");
    }echoln();
    enc.clear_block();
    echoln("input string to be encrypted:");
    uint8_t length = readStringUntil('\n',enc.block,enc.blocksize,0);
    uint8_t blocklength = 48;
    if (length<=32) blocklength = 32;
    if (length<=16) blocklength = 16;
    echo("to optimize load, it could be 16,32, or 48. length is: ");
    echoln(blocklength);
    
    echoln("before encrypted:");
    for (uint8_t i=0;i<blocklength;i++){
        echo(enc.block[i]);
        echo (", ");
    }echoln();
    
    uint8_t tag = enc.encrypt_gcm(blocklength);
    
    echoln("after encrypted:");
    for (uint8_t i=0;i<blocklength;i++){
        echo(enc.block[i]);
        echo (", ");
    }echoln();
    echo("tag:");
    echoln(tag);
    
    bool ok = enc.decrypt_gcm(tag+1,blocklength);
    echoln("after decrypted:");
    for (uint8_t i=0;i<blocklength;i++){
        echo(enc.block[i]);
        echo (", ");
    }echoln();
    echo("tag ok?:");
    echoln(ok);
}

void test_other(){
    echoln("test others..");
    echoln("test sizeof");
    const uint8_t sample[7] = {1,2,3,4,5,6,7};
    assert_equal(7,sizeof(sample)/sizeof(uint8_t));
    echoln("test pointer");
    uint8_t *testdata = 0;
    echo("testdata value before malloc: ");
    if (testdata==0){
        echoln("is 0");
    }
    else {
        echoln("is not 0");
    }
    echoln((int)testdata);
    testdata = (uint8_t *)malloc(sizeof(uint8_t)*9);
    echo("after malloc: ");
    if (testdata==0){
        echoln("is 0");
    }
    else {
        echoln("is not 0");
    }
    echoln((int)testdata);
    free(testdata);
    
    echoln("testing macro malloc array");
    testdata = array(uint8_t, 4);
    for (uint8_t i=0;i<4;i++){
        testdata[i] = i*2;
    }
    for (uint8_t i=0;i<4;i++){
        assert_equal(testdata[i],i*2);
    }
    
    echoln("testing memcopy and pointer start index");
    uint8_t *source = array8(8);
    uint8_t *dest = array8(8);
    
    for (uint8_t i=0;i<8;i++) {
        source[i] = (uint8_t)('A'+i);
        Serial.write(source[i]);
    } echoln();
    
    memcpy(dest,source,8);
    
    echoln("destination after fullcopy: ");
    for (uint8_t i=0;i<8;i++) {
        Serial.write(dest[i]);
    } echoln();
    
    echoln("reclearing dest");
    for (uint8_t i=0;i<8;i++){
        dest[i] = ' ';
    }
    
    uint8_t *sourceshifted = source+3;
    memcpy(dest,sourceshifted,5);
    
    echoln("destination after fullcopy: ");
    for (uint8_t i=0;i<8;i++) {
        Serial.write(dest[i]);
    } echoln();
}

void test_gcm(){
    uint8_t i;
    Serial.print("test enc.cpp gcm\n");
    uint8_t key[16] = {0, 156, 23, 174, 145, 29, 28, 219, 99, 239, 14, 143, 101, 115, 89, 167};
    uint8_t iv[12]  = {161, 170, 246, 167, 147, 21, 210, 133, 54, 153, 214, 21};
    uint8_t authdata[4] = {8, 55, 116, 197};
    uint8_t tag[6] = {0,0,0,0,0,0};
    const uint8_t blocksize = 32;
    uint8_t *decryptedblock = malloc(sizeof(uint8_t)*blocksize);
    uint8_t *encryptedblock = malloc(sizeof(uint8_t)*blocksize);
    bool correct;
    
    Serial.print("input string to encrypt:\n");
    String *datastring = new String("Izzulmakin");
    Serial.print("before encrypted:\n");
    for (i=0;i<blocksize;i++){
        uint8_t v = 0;
        if (i<datastring->length()) {
           v = datastring->charAt(i);
        }
        decryptedblock[i] = v;
        Serial.print(v);Serial.print(", ");
    }
    Serial.print("\n");
    
    enc.gcm->setKey(key, 16);
    enc.gcm->setIV(iv, 12);
    enc.gcm->addAuthData(authdata,11);
    enc.gcm->encrypt(encryptedblock,decryptedblock,blocksize);
    enc.gcm->computeTag(tag,6);

    Serial.print("after encrypted:\n");
    for (i=0;i<blocksize;i++){
        Serial.print(encryptedblock[i]);Serial.print(", ");
    }
    Serial.print("\n");

    Serial.print("try to decrypt\n");
    enc.gcm->setKey(key, 16);
    enc.gcm->setIV(iv, 12);
    enc.gcm->addAuthData(authdata,11);
    enc.gcm->decrypt(decryptedblock,encryptedblock,blocksize);
    Serial.print("after decrypted:\n");
    for (i=0;i<blocksize;i++){
        Serial.print(decryptedblock[i]);Serial.print(", ");
    }
    Serial.print("\n");
    correct = enc.gcm->checkTag(tag,6);
    Serial.println(correct);
    
    
    Serial.print("try if authdata on decrypt change something\n");
    enc.gcm->setKey(key, 16);
    enc.gcm->setIV(iv, 12);
    authdata[1] = 59;
    enc.gcm->addAuthData(authdata,11);
    enc.gcm->decrypt(decryptedblock,encryptedblock,blocksize);
    Serial.print("after decrypted:\n");
    for (i=0;i<blocksize;i++){
        Serial.print(decryptedblock[i]);Serial.print(", ");
    }
    Serial.print("\n");
    correct = enc.gcm->checkTag(tag,6);
    Serial.println(correct);
    
    
    Serial.print("try if iv on decrypt change something\n");
    enc.gcm->setKey(key, 16);
    iv[5] = iv[5]+1;
    enc.gcm->setIV(iv, 12);
    enc.gcm->addAuthData(authdata,11);
    enc.gcm->decrypt(decryptedblock,encryptedblock,blocksize);
    Serial.print("after decrypted:\n");
    for (i=0;i<blocksize;i++){
        Serial.print(decryptedblock[i]);Serial.print(", ");
    }
    Serial.print("\n");
    correct = enc.gcm->checkTag(tag,6);
    Serial.println(correct);
    
    Serial.print("try if iv correct on decrypt change something\n");
    enc.gcm->setKey(key, 16);
    iv[5] = iv[5]-1;
    enc.gcm->setIV(iv, 12);
    enc.gcm->addAuthData(authdata,11);
    enc.gcm->decrypt(decryptedblock,encryptedblock,blocksize);
    Serial.print("after decrypted:\n");
    for (i=0;i<blocksize;i++){
        Serial.print(decryptedblock[i]);Serial.print(", ");
    }
    Serial.print("\n");
    correct = enc.gcm->checkTag(tag,6);
    Serial.println(correct);
    
    
    Serial.print("try if key on decrypt change something\n");
    key[0] = key[0]+4;
    enc.gcm->setKey(key, 16);
    enc.gcm->setIV(iv, 12);
    enc.gcm->addAuthData(authdata,11);
    enc.gcm->decrypt(decryptedblock,encryptedblock,blocksize);
    Serial.print("after decrypted:\n");
    for (i=0;i<blocksize;i++){
        Serial.print(decryptedblock[i]);Serial.print(", ");
    }
    Serial.print("\n");
    correct = enc.gcm->checkTag(tag,6);
    Serial.println(correct);
    free(decryptedblock);
    free(encryptedblock);
    delete(datastring);
}

void test_readstringuntil(){
    echo("testing readstringuntil");
    br();
    uint8_t *data = malloc(sizeof(uint8_t)*32);
    uint8_t i;
    for(i=0;i<32;i++){
        data[i]=0;
    }
    echo("input string with newline:");
    br();
    readStringUntil('\n',data,16,0);
    echo("data: ");
    for(i=0;i<32;i++){
        echo(data[i]);
        echo(", ");
    }
    br();
    assert_equal(data[16],0);
    
    for(i=0;i<32;i++){
        data[i]=0;
    }
    echo("data reemptied");
    br();
    
    echo("input string with newline:");
    br();
    readStringUntil('\n',data,16,8);
    
    echo("data: ");
    for(i=0;i<32;i++){
        echo(data[i]);
        echo(", ");
    }
    br();
    
    assert_equal(data[7],0);
    assert_equal(data[24],0);
    
    
    
    free(data);
}

void test_hkdf(){
    uint8_t *key = malloc(sizeof(uint8_t)*32);
    uint8_t *hashedkey = malloc(sizeof(uint8_t)*32);
    uint8_t length;
    uint8_t i;
    for(i=0;i<32;i++){
        key[i]=0;
    }
    echo("input string to hash hkdf:");
    br();
    readStringUntil('\n',key,32,0);
    echo("the key: ");br();
    for(i=0;i<32;i++){
        echo(key[i]);
        echo(", ");
    }
    br();
    
    echo("length is: ");
    for (i=31;i>=0;i--){
        if (key[i]!=0) {
            length = i+1;
            break;
        }
    }
    echo(length);
    br();
    
    HKDF<SHA256> hkdf;
    hkdf.setKey(key,32);
    hkdf.extract(hashedkey,32);
    echo("hashed:");
    br();
    for(i=0;i<32;i++){
        echo(hashedkey[i]);
        echo(", ");
    }
    br();
    
    echo("same string with reduced length");
    key[31] = 40;
    hkdf.setKey(key,4);
    hkdf.extract(hashedkey,32);
    echo("hashed:");
    br();
    for(i=0;i<32;i++){
        echo(hashedkey[i]);
        echo(", ");
    }
    br();
    
    echo("same string with additional salt index[31]=40");
    key[31] = 40;
    hkdf.setKey(key,32);
    hkdf.extract(hashedkey,32);
    echo("hashed:");
    br();
    for(i=0;i<32;i++){
        echo(hashedkey[i]);
        echo(", ");
    }
    br();
    
    free(key);
    free(hashedkey);
}
uint64_t positivepower(uint64_t target,uint64_t power){
    uint64_t result = target;
    if (power==0) {
        return 1;
    }
    while (power>1) {
        result = result * target;
        power--;
    }
    return result;
}
long start, startepoch;
int hour,minute,second, start_hour,start_minute,start_second;
void test_time(){
    uint8_t *t = malloc(sizeof(uint8_t)*16);
    //~ uint8_t *timetext = malloc(sizeof(uint8_t)*18);
    //~ echoln("masukkan:");
    //~ uint8_t digits = readStringUntil('\n',timetext,18,0);
    //~ echo("tercatat ");
    //~ echoln(digits);
    //~ for (uint8_t i=0;i<digits;i++){echo(timetext[i]-'0');echo(',');}
    //~ echoln();
    //~ start = 0;
    //~ uint8_t basepower = 0;
    //~ for (int i=(digits-1);i>=0;i--){
        //~ int digit = (timetext[i]-(uint8_t)'0');
        //~ echo("digit: ");
        //~ echoln(digit);
        //~ unsigned long digitvalue = digit * positivepower(10,basepower);
        //~ echo("val: ");
        //~ echoln((unsigned long)digitvalue);
        //~ basepower += 1;
        //~ start += digitvalue;
    //~ }
    Serial.setTimeout(60000);
    charmenu.lcdclear();
    charmenu.lcdgotoxy(0,0);
    charmenu.lcddisplay("masukkan  epoch,");
    charmenu.lcdgotoxy(0,1);
    charmenu.lcddisplay("hour,min,second ");
    startepoch = Serial.parseInt();
    //~ hour =   Serial.parseInt();
    //~ minute = Serial.parseInt();
    //~ second = Serial.parseInt();
    //~ start_hour = hour;
    //~ start_minute = minute;
    //~ start_second = second;
    start = millis();
    while (1){
        long now = millis();
        //~ second += 1;
        charmenu.lcdclear();
        charmenu.lcdgotoxy(0,0);
        sprintf(
            t,
            ("%lu"),
            startepoch
        );
        charmenu.lcddisplay(t);
        //~ if (second>=60) {
            //~ second = 0;
            //~ minute += 1;
        //~ }
        //~ if (minute>=60) {
            //~ minute = 0;
            //~ hour += 1;
        //~ }
        //~ if (hour >=24) {
            //~ hour = 0;
        //~ }
        charmenu.lcdgotoxy(0,1);
        //~ sprintf(t,"%lu %d:%d:%d",
            //~ startepoch+((millis()-start)/1000),hour,minute,second);
        sprintf(t,"%lu",startepoch+((millis()-start)/1000));
        charmenu.lcddisplay(t);
        test_gcm();
        while ((millis()-now)<1000) {
        }
    }
}

void dotest(){
    //~ test_securedata();
    //~ test_charmenu();
    //~ test_enc();
    //~ test_other();
    //~ test_gcm();
    //~ test_readstringuntil();
    //~ test_hkdf();
    test_time();
}

#endif
