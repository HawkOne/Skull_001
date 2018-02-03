//Skull_001.
//Author: Augusto Marinucci
#include "led.h"

uint8_t blink_color = 0;
uint8_t blink = 0;

void Led_On(uint8_t color) 
{
    if(!blink) // wait to finish blink
    {
        LED_G_LAT = color&1;
        LED_B_LAT = (color&2)>>1;
    }
}

void Led_Blink(uint8_t user_color, uint8_t user_blink) 
{
    if(!blink)
    {
        if(user_color == 4) user_color = 1;
        if(user_color != blink_color)
        {        
            blink_color = user_color;
            LED_G_LAT = 0;  LED_B_LAT = 0; // LED OFF
        }	
        blink = user_blink;	
    }
	
}

void Led_Blink_Handler(void) // called every 100 ms
{
	if(blink)
	{
        blink--;
        LED_G_LAT ^= blink_color&1;
        LED_B_LAT ^= (blink_color&2)>>1;		
	}
    else
    {
        LED_G_LAT = 0;  LED_B_LAT = 0; // LED OFF
    }
}