
#include "Keyboard.h"
#include <LiquidCrystal.h>

#include "button.h"

#include "charmenu.h"
#include "enc.h"
#include "readstringuntil.h"
#include "autotyper.h"

#define ENABLE_TESTING //comment to disable test to save more flash/ram
#ifdef ENABLE_TESTING
  #include "test.h"
#endif

#define array(tipe,n) (tipe *)malloc(n*sizeof(tipe))
#define array8(n) (uint8_t *)malloc((n)*sizeof(uint8_t))
#define echo Serial.print
#define echoln Serial.println

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
  uint8_t bt = button_wait(1);
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
  Menu* menu_autotyper = new Menu("Autotyper", charmenu.root);
  menu_autotyper->hoveraction = &led_high;
  autotypers_register_charmenu(menu_autotyper);
  
  
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



/**
 * read button sequence with timeout, and call enc.set_password with the sequence
 * @param uint8_t first_input_timeout: timeout in second to wait for the first input. typical value: 10seconds
 * @param uint8_t between_input_timeout: timeout in second to wait for between button pressed. typical value: 2seconds
 * @param bool debug: if true, echo password
 * @return uint8_t: length of the sequence
 */
uint8_t set_password_with_button_sequence(uint8_t first_input_timeout, uint8_t between_input_timeout, bool debug){
  if (enc.key_iv_auth!=0) {//pointer is not null, means it's been set
    echo("password is already set");
    return;
  }
  button_wait_release(10);
  // get password with button presses, if no more button pressed after timeout, password input is considered done
  uint8_t maxlength = 15;
  uint8_t *pw = array8(maxlength);
  uint8_t pwlength=0;
  uint8_t timeout;
  while (pwlength<=maxlength){
      pwlength += 1;
      timeout = between_input_timeout;
      if (pwlength==1){
        timeout = first_input_timeout;
      }
      uint8_t bt = button_wait(timeout);
      if (bt==BUTTON_NONE) {
          pwlength -= 1;
          break;
      }
      echo(".");
      pw[pwlength-1] = bt;//fill the last item of pw
  }
  echoln(" selesai");
  echoln(pwlength);
  if (debug) {
    for (uint8_t i=0;i<pwlength;i++){
      echo(pw[i]);
      echo(", ");
    }echoln();
  }
  enc.set_password(pw,pwlength);//enc.cpp
  free(pw);
  return pwlength;
}

