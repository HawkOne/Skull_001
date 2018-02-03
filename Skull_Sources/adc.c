//Skull_001.
//Author: Augusto Marinucci
#include "adc.h"

volatile uint8_t adc[3] = 0; // [0-1] adc out after average, [2] flag

void ADC_Init(void)
{   
    ANSELC = POT1_ANSEL + POT2_ANSEL; //pot1 e pot2 anal
    ADCON0 = 0;
    ADCON0bits.CHS = POT1_AN;
    ADCON1 = 0; 
    ADCON1bits.ADFM = 0;     
    ADCON1bits.ADCS = 0b111; // frc 
    ADIF = 0;
    ADIE = 0;
    ADON = 1;
    adc[FLAG] = UNLOCKED;
}
void ADC_Handler(void) 
{
    static uint8_t adc_channel = 0;   
    static uint8_t adc_lock[2] = 0; 
    static uint16_t adc_m = 0;
    static uint8_t adc_c = 0;

    adc_m += ADRESH ; // read value
    // average
    adc_c++;
    if(adc_c == 16)
    {
        adc_c = 0;
        adc[adc_channel] = adc_m>>4;
        adc_m  = 0;
        adc_channel ^=1; // change channel
        ADCON0bits.CHS = POT1_AN + adc_channel;
    }

    if(adc[FLAG] == TO_LOCK)
    {
        adc_lock[POT1] = adc[POT1];
        adc_lock[POT2] = adc[POT2];
        adc[FLAG] = LOCKED;
    } 

    if(adc[FLAG] & LOCKED) // check if pot moved
    {
        if(adc_lock[POT1] > adc[POT1])
        {
            if(adc_lock[POT1] - adc[POT1] > SOGLIA_LOCK) adc[FLAG] |= FLAG1;
        }
        else
        {
            if(adc[POT1] - adc_lock[POT1] > SOGLIA_LOCK) adc[FLAG] |= FLAG1;
        }
        
        if(adc_lock[POT2] > adc[POT2])
        {
            if(adc_lock[POT2] - adc[POT2] > SOGLIA_LOCK) adc[FLAG] |= FLAG2;
        }
        else
        {
            if(adc[POT2] - adc_lock[POT2] > SOGLIA_LOCK) adc[FLAG] |= FLAG2;
        }
    }
}

void ADC_Lock(void)
{
	if(adc[FLAG] & UNLOCKED) adc[FLAG] = TO_LOCK;
}

void ADC_Unlock(void)
{
	adc[FLAG] |= UNLOCKED;
}