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

void button_wait_release(uint8_t timeout_seconds){
  uint8_t counter_onetenth = 0;
  //wait for release
  while (button_keyval(analogRead(0))!=BUTTON_NONE) {
    delay(100);
    counter_onetenth += 1;
    if ((counter_onetenth/10)>=timeout_seconds) {
      break;
    }
  }
}
/**
 * busyloop until any button other than BUTTON_NONE is pressed,
 * @param timeout_seconds: timeout for waitin until button is pressed, and waiting until button is released
 *  for example timeout_seconds=1, if BUTTON_RIGHT is held forever, function will return BUTTON_RIGHT after timeout_seconds reached
 * @return button pressed value (BUTTON_UP, BUTTON_DOWN, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_SELECT)
 * if timeout reached, return BUTTON_NONE
 */
uint8_t button_wait(uint8_t timeout_seconds){
  int a = analogRead(0);
  int button_value = button_keyval(a);
  uint8_t counter_onetenth = 0;
  //wait for press
  while (button_value==BUTTON_NONE) {
    delay(100);
    a = analogRead(0);
    button_value = button_keyval(a);
    counter_onetenth += 1;
    if ((counter_onetenth/10)>=timeout_seconds) {
      return BUTTON_NONE;
    }
  }
  //wait for release
  while (button_keyval(analogRead(0))!=BUTTON_NONE) {
    delay(100);
    counter_onetenth += 1;
    if ((counter_onetenth/10)>=timeout_seconds) {
      break;
    }
  }
  return button_value;
}
/**
 * if timeout_seconds ommited, default value is 10seconds
 */
uint8_t button_wait(){
  return button_wait(10);
}
