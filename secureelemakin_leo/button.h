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
 * @param wait: bool (false or true), if true, busyloop until any button is pressed. if wait==false, return button even if value is BUTTON_NONE
 * @return the button value constant BUTTON_RIGHT, or BUTTON_LEFT, or BUTTON_UP, or BUTTON_DOWN, or BUTTON_SELECT, or BUTTON_NONE
 * 
 */
uint8_t button(bool wait);
/*
 * like button(wait) but block until button press is released
 */
uint8_t button_wait();
#endif
