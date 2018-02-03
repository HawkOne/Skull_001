//Skull_001.
//Author: Augusto Marinucci
#include "system.h"
#include "config.h"
#include "gen_init.h"
#include "debounce.h"
#include "synth.h"
#include "led.h"
#include "adc.h"

volatile uint8_t synth_menu = 9;
volatile uint8_t timer_100ms = 0;
volatile uint8_t timer_10ms = 0;

void main (void)
{
    Gen_Init();
    ADC_Init();
    Synth_Init();
    
    while(1)
	{ 
        uint8_t x = 0;

 		if(flag_timer_5ms) 
		{     
            ADGO = 1; // start adc sampling
            flag_timer_5ms = 0;
			Synth_Handler();
            
			timer_100ms++;
            timer_10ms++;
            if(timer_10ms == 2) // 10ms
			{                
				timer_10ms = 0;
                Debounce_Handler(); 
			} 
            
			if(timer_100ms == 20) // 100 ms
			{				
				timer_100ms = 0;
                Led_Blink_Handler();
			}             
		} 

        if(ADIF) // adc conversion done
        {        
            ADIF = 0;
            ADC_Handler();
        }

        if(synth_menu<9) 
        {
            Synth_Menu(synth_menu);     
        } 

		for(x=0;x<5;x++) // check buttons
        {  
            if(debounce_button[x] == BUT_PRESSED_HOLD_RELEASED)
            {
            	debounce_button[x] = 0;
            	ADC_Unlock();
                LED_G_LAT = 0;  LED_B_LAT = 0; // LED OFF
            } 	

            if(debounce_button[x] == BUT_PRESSED_HOLD ) // hold
            {             
                uint8_t but = x;
                uint8_t seq_step = Synth_Read_Step();
                if(x==4) but = 8;
                if(seq_step>7)
                {
                    if(debounce_button[0] == BUT_PRESSED_HOLD & debounce_button[1] == BUT_PRESSED_HOLD ) but = 4; 
                    if(debounce_button[1] == BUT_PRESSED_HOLD & debounce_button[2] == BUT_PRESSED_HOLD ) but = 5; 
                    if(debounce_button[2] == BUT_PRESSED_HOLD & debounce_button[3] == BUT_PRESSED_HOLD ) but = 6; 
                    if(debounce_button[3] == BUT_PRESSED_HOLD & debounce_button[4] == BUT_PRESSED_HOLD ) but = 7; 
                }                
                if(synth_menu == 9)
                {
                    Synth_Hold(but,x);  
                }                              
            }	            

            if(debounce_button[x] == BUT_PRESSED_DOUBLE_RELEASED) // double
            {
                LED_G_LAT = 0;  LED_B_LAT = 0; // LED OFF
                debounce_button[x] = 0;

                if(debounce_button[4] == BUT_PRESSED_HOLD) 	// shift 5
                {    
                    synth_menu = x + 5;     
                    ADC_Unlock();                                                     
                }                  
                else // no shift
                {      
                    synth_menu = x; 
                    ADC_Unlock();                                     	              	                                
                }
            } 

            if(debounce_button[x] == BUT_PRESSED_SINGLE_RELEASED) // singolo
            {
                LED_G_LAT = 0;  LED_B_LAT = 0; // LED OFF
                debounce_button[x] = 0;
                if(debounce_button[4] == BUT_PRESSED_HOLD) // shift 5
                {     
                    Synth_Shift_Single(x); 
                } 
                else // no shift
                {
                    if(x==4) // single 4 (shift)
                    {
                        if(synth_menu<9)
                        {
                            if(synth_menu == 8)
                            {
                                Synth_Save_Erase_Clear();
                            }
                            synth_menu = 9;
                            ADC_Unlock(); 
                        }
                        else
                        {
                            Synth_Seq_Start(); 
                        }                                                
                    }  
                    else
                    {
                        Synth_Single(x);
                    }                               	                	
                }                    	               	
            } 
    	}            
    } 
}