//Skull_001.
//Author: Augusto Marinucci
#ifndef LED_H
#define LED_H

#include "system.h"

#define ROSSO   1			
#define BLU     2   
#define VIOLA   3 		

void Led_On(uint8_t color);
void Led_Blink(uint8_t user_color, uint8_t user_blink);
void Led_Blink_Handler(void);

#endif	