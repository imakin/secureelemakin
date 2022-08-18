
#ifndef PASSWORD_H
#define PASSWORD_H

#define BUTTON_RIGHT 0
#define BUTTON_LEFT 1
#define BUTTON_UP 2
#define BUTTON_DOWN 3
#define BUTTON_SELECT 4
#define BUTTON_NONE 5

#include <cstdint> //uint8_t,
#include <cstdlib> //malloc, free

uint8_t* get_password();
uint8_t get_password_length();

#endif
