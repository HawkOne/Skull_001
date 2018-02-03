//Skull_001.
//Author: Augusto Marinucci
#ifndef SYSTEM_H
#define SYSTEM_H

#define _XTAL_FREQ 32000000 // for delay_us in debounce.c,todo: remove multiplexing in new HW
#include <xc.h>
#include <stdint.h>

#define BUT1 RA5
#define BUT2 RA4
#define BUT3 RA3 

#define ROW1_TRIS TRISAbits.TRISA0
#define ROW1_LAT LATA0

#define ROW2_TRIS TRISAbits.TRISA1
#define ROW2_LAT LATA1

#define POT1_AN 4 // AN4 
#define POT1_ANSEL 1 // RC0

#define POT2_AN 5 // AN5 
#define POT2_ANSEL 2 // RC1

#define LED_G_TRIS TRISCbits.TRISC4
#define LED_G_LAT LATC4

#define LED_B_TRIS TRISCbits.TRISC2
#define LED_B_LAT LATC2

#define PWM2_H_TRIS TRISCbits.TRISC3
#define PWM1_L_TRIS TRISCbits.TRISC5
#define PWM1_L_LAT LATC5

#endif