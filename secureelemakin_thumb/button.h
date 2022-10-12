#include "Arduino.h"

//makin 2022
#ifndef FILE_BUTTON_H
#define FILE_BUTTON_H

#define BUTTON_RIGHT 0
#define BUTTON_LEFT 1
#define BUTTON_UP 2
#define BUTTON_DOWN 3
#define BUTTON_SELECT 4
#define BUTTON_NONE 5

/*
 * translate analog data into 
 *     BUTTON_RIGHT , or 
 *     BUTTON_LEFT  , or 
 *     BUTTON_UP    , or 
 *     BUTTON_DOWN  , or 
 *     BUTTON_SELECT, or 
 *     BUTTON_NONE 
 */
uint8_t button_keyval(int analogvalue);
/*
 * read lcd-keypad shield button (using analog0)
 * @param timeout_seconds: timeout for waitin until button is pressed, and waiting until button is released
 *  for example timeout_seconds=1, if BUTTON_RIGHT is held forever, function will return BUTTON_RIGHT after timeout_seconds reached
 * @return button pressed value (BUTTON_UP, BUTTON_DOWN, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_SELECT)
 * if timeout reached, return BUTTON_NONE
 * 
 */
uint8_t button_wait(uint8_t timeout_seconds);
uint8_t button_wait();
void button_wait_release(uint8_t timeout_seconds);
#endif
