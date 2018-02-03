//Skull_001.
//Author: Augusto Marinucci
#ifndef SYNTH_H
#define SYNTH_H

#include "system.h"
// synth_state
#define SYNTH_OFF 0
#define SYNTH_UPDATE_FREQ 1
#define SYNTH_RUNNING 2
// env_state
#define ENV_OFF 0
#define ENV_ATTACK 1
#define ENV_SUSTAIN 2
#define ENV_RELEASE 3
// lfo_dest
#define LFO_PITCH 0
#define LFO_PWM 1
#define LFO_LPF 2
// lfo_polarity
#define LFO_POS 0
#define LFO_NEG 1

extern volatile uint8_t flag_timer_5ms;

void Synth_Init(void);
void Synth_Handler(void); 
uint8_t Synth_Read_Step(void);
void Synth_Seq_Start(void);
void Synth_Hold(uint8_t but, uint8_t led);
void Synth_Menu(uint8_t synth_menu);
void Synth_Save_Erase_Clear(void);
void Synth_Single(uint8_t but);
void Synth_Shift_Single(uint8_t but);

#endif	