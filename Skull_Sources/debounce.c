//Skull_001.
//Author: Augusto Marinucci
#include "debounce.h"

volatile uint8_t debounce_button[5] = {0};

void Debounce_Handler(void) // works, don't remember how
{
    static uint8_t debounce_on[5] = {0};
    static uint8_t debounce_off[5] = {0};
	uint8_t i = 0;
    uint8_t c = 0;
    // multiplexing 
    ROW1_TRIS = 0;
    ROW1_LAT = 0;
    __delay_us(5);
    
    if(!BUT1) c |= 1;
    if(!BUT2) c |= 2;
    if(!BUT3) c |= 4;
    
    ROW1_TRIS = 1;
    ROW2_TRIS = 0;
    ROW2_LAT = 0;
    __delay_us(5);
    
    if(!BUT3) c |= 8;
    if(!BUT2) c |= 16;
    
    ROW2_TRIS = 1;
    // debounce
	for(i=0;i<5;i++)
	{
		if(c & (1<<i)) // button pressed right now
		{    
			debounce_on[i]++; // increment on
            if(debounce_button[i] == BUT_PRESSED_SINGLE) 
            {                
                if(debounce_on[i] == DEBOUNCE_HOLD )
                {                	
                    debounce_off[i] = 0;
                	debounce_button[i] = BUT_PRESSED_HOLD;
                }
                
                if(debounce_on[i] == DEBOUNCE_ON & debounce_off[i] > DEBOUNCE_DOUBLE ) 
                {   
                    debounce_off[i] = 0;
                    debounce_button[i] = BUT_PRESSED_DOUBLE;
                }
            }
            else if(debounce_button[i] == 0 )
            {
                debounce_off[i] = 0;
                if(debounce_on[i] == DEBOUNCE_ON ) 
                {  
                    debounce_button[i] = BUT_PRESSED_SINGLE;
                }                
            }           
		}
		else // button not pressed
		{
            debounce_on[i] = 0; // as soon i release zero the on counter
            if(debounce_button[i] )
            {
                debounce_off[i]++;
                debounce_on[i] = 0;            
                if(debounce_off[i] == DEBOUNCE_OFF ) 
                {                                           
                    debounce_button[i] += BUT_PRESSED_RELEASED;                    
                }
            }
        }
    }
}