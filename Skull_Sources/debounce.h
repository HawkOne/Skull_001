//Skull_001.
//Author: Augusto Marinucci
#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include "system.h"
// * 5ms
#define DEBOUNCE_ON 5				
#define DEBOUNCE_OFF 12 // prima 15
#define DEBOUNCE_DOUBLE 1
#define DEBOUNCE_HOLD 25
#define BUT_PRESSED_SINGLE 1
#define BUT_PRESSED_DOUBLE 2
#define BUT_PRESSED_HOLD 3
#define BUT_PRESSED_RELEASED 10
#define BUT_PRESSED_SINGLE_RELEASED 11
#define BUT_PRESSED_DOUBLE_RELEASED 12
#define BUT_PRESSED_HOLD_RELEASED 13

extern volatile uint8_t debounce_button[5];

void Debounce_Handler(void);

#endif