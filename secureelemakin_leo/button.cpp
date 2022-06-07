#include "button.h"

uint8_t button_keyval(int analogvalue) {
  if (analogvalue<60) {
    return BUTTON_RIGHT;
  }
  else if (analogvalue<200) {
    return BUTTON_UP;
  }
  else if (analogvalue<400) {
    return BUTTON_DOWN;
  }
  else if (analogvalue<600) {
    return BUTTON_LEFT;
  }
  else if (analogvalue<800) {
    return BUTTON_SELECT;
  }
  return BUTTON_NONE;
}

uint8_t button(bool wait) {
  int a = analogRead(0);
  int button_value = button_keyval(a);
  if (
    (button_value!=BUTTON_NONE) ||
    (wait==false)
  ) {
    return button_value;
  }
  while (button_value==BUTTON_NONE) {
    delay(400);
    a = analogRead(0);
    button_value = button_keyval(a);
  }
  return button_value;
}
uint8_t button_wait(){
  int a = analogRead(0);
  int button_value = button_keyval(a);
  //wait for press
  while (button_value==BUTTON_NONE) {
    delay(100);
    a = analogRead(0);
    button_value = button_keyval(a);
  }
  //wait for release
  while (button_keyval(analogRead(0))!=BUTTON_NONE) {
    delay(100);
  }
  return button_value;
}
