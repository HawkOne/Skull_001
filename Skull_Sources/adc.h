//Skull_001.
//Author: Augusto Marinucci
#ifndef ADC_H
#define ADC_H
#include "system.h"

#define SOGLIA_LOCK 2

#define POT1 0
#define POT2 1
#define FLAG 2

#define UNLOCKED 1
#define FLAG1 2
#define FLAG2 4
#define TO_LOCK 8
#define LOCKED 16

extern volatile uint8_t adc[3];

void ADC_Init(void);
void ADC_Handler(void);
void ADC_Lock(void);
void ADC_Unlock(void);

#endif	