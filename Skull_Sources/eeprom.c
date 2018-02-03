//Skull_001.
//Author: Augusto Marinucci
#include "eeprom.h"

uint8_t Read_EEPROM(uint8_t read_addr)
{
	uint8_t read_data;

    EEADRL  = read_addr;    // EEPROM address stored in the byte register

    EECON1bits.EEPGD = 0;             // Access to EEPROM memory
    EECON1bits.RD = 1;                // Start EEPROM reading

    read_data = EEDATL;      // Read the EEDATA
    return read_data;
}

void Write_EEPROM(uint8_t write_data,uint8_t write_addr)
{
    uint8_t temp_int;

	temp_int = GIE;
    EEADRL  = write_addr;
    EEDATL =  write_data;  
    EECON1bits.EEPGD = 0;                               
    WREN  = 1;  
    GIE = 0;   
    /* REQUIRED SEQUENCE */
    EECON2 = 0X55;
    EECON2 = 0XAA;
    WR = 1;
	/* REQUIRED SEQUENCE */    
    GIE = temp_int;     
    while (EEIF == 0);               
    EEIF = 0;                            
    WREN = 1;
}