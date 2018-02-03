//Skull_001.
//Author: Augusto Marinucci
#ifndef EEPROM_H
#define EEPROM_H

#include "system.h"

#define MEMORY_STATUS 100

uint8_t Read_EEPROM(uint8_t read_addr);
void Write_EEPROM(uint8_t write_data,uint8_t write_addr);

#endif