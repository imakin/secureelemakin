
#include "Keyboard.h"
#include <LiquidCrystal.h>
#include <AESLib.h>

#include "button.h"

#include "charmenu.h"
#include "enc.h"

#define ENABLE_TESTING //comment to disable test to save more flash/ram
#ifdef ENABLE_TESTING
  #include "test.h"
#endif

LiquidCrystal lcd(8,9,4,5,6,7);//lcd(rs,en,d4,d5,d6,d7)
void lcd_print(char* text){
  lcd.print(text);
}
void lcd_setcursor(uint8_t x,uint8_t y){
  lcd.setCursor(x,y);
}
void lcd_clear(){
  lcd.clear();
}
CharMenuClass charmenu(&lcd_print, &lcd_setcursor, &lcd_clear);

String text = "";

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  lcd.begin(16,2);
  // initialize control over the keyboard:
  Serial.begin(115200);
  lcd.setCursor(0,0);
  lcd.print("starting");
  Serial.println("starting");
  Keyboard.begin();
  charmenu_prepare();
  charmenu.begin();
}

void loop(){
  uint8_t bt = button_wait();
  if (bt==BUTTON_DOWN || bt==BUTTON_SELECT) {
    charmenu.action_enter();
  }
  else if (bt==BUTTON_UP) {
    charmenu.action_back();
  }
  else if (bt==BUTTON_LEFT) {
    charmenu.action_prev();
  }
  else if (bt==BUTTON_RIGHT) {
    charmenu.action_next();
  }
}

void led_high(){
  digitalWrite(LED_BUILTIN, HIGH);
}
void led_low(){
  digitalWrite(LED_BUILTIN, LOW);
}


void charmenu_prepare(){
  Menu* menu_aessandbox = new Menu("AES sandbox/tool", charmenu.root);
  menu_aessandbox->hoveraction = &led_high;
  menu_aessandbox->action = &action_aessandbox;
  
  
  Menu* menu_otp = new Menu("OTP 2FA         ",charmenu.root);
  menu_otp->hoveraction = &led_low;
  
  Menu* menu_savedpass = new Menu("Saved password  ", charmenu.root);
  menu_savedpass->hoveraction = &led_high;
  
  Menu* menu_buttonadc = new Menu("Button ADC      ", charmenu.root);
  menu_buttonadc->action = &action_buttonadc;

  #ifdef ENABLE_TESTING
    Menu* menu_test = new Menu("testscript      ", charmenu.root);
    menu_test->action = &dotest;
  #endif

}

void action_buttonadc(){
  int a;
  int bt;
  while (true){
    a = analogRead(0);
    lcd.setCursor(0,1);
    lcd.print(a);
    lcd.print("         ");
    delay(50);
  }
}

void action_aessandbox(){
  Serial.setTimeout(60000);
  Serial.println("input password and enter:");
  String pw = Serial.readStringUntil('\n');
  enc::setkey(pw);
  int i=0;
  Serial.println("key is:");
  for(i=0;i<16;i++){
    Serial.print(enc::key[i]);
    Serial.print(" ");
  }
  Serial.print("\n");
  Serial.println("what to encrypt?");
  String datastring = Serial.readStringUntil('\n');
  Serial.println("encrypting: "+datastring);
  uint32_t length = enc::test_encrypt(datastring); //todo: only doing 16 blocks, manually encrypt the next block!
  Serial.print("\n");
}
