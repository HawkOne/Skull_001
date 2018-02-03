//Skull_001.
//Author: Augusto Marinucci
#include "gen_init.h"

void Gen_Init(void)
{
	OSCCON = 0b11110000; // 32 MHz
	while(!HFIOFS); 

	ANSELA = 0;	//all digital
	LATA = 0;
	TRISA = 0xff; //all input
    OPTION_REGbits.nWPUEN = 0; //pull-up enable
    WPUA = 0b00111000; //all pull-up on
	
	ANSELC = 0; //all digital, anal in adc.c
	LATC = 0;
	TRISC = 0xff; //all input, pwm out in synth.c
	WPUC = 0;   // no pull-up on port C
	LED_G_TRIS = 0;
	LED_B_TRIS = 0;
}