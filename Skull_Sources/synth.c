//Skull_001.
//Author: Augusto Marinucci
#include "synth.h"
#include "led.h"
#include "adc.h"
#include "eeprom.h"
#include <stdlib.h>

//-----FLAG 5ms----
 volatile uint8_t  flag_timer_5ms = 0; //flag 5ms for main.c
//------SYNTH-----
 volatile uint8_t  synth_gate = 0; // on off synth in ISR
 volatile uint8_t  synth_state = 0;// off , running, update
 volatile uint8_t  synth_volume = 240; 
 volatile uint16_t synth_pwm0 = 32768; // dc = (synth_pwm0/65536)*100
 volatile uint16_t synth_pwm1 = 32768; // dc = (synth_pwm0/65536)*100
 volatile uint16_t synth_pwm0_base = 32768; 
 volatile uint16_t synth_pwm1_base = 32768; 
 volatile uint8_t  synth_saw = 0;
 volatile uint16_t synth_osc_x0 = 0; 
 volatile uint16_t synth_osc_x1 = 0; 
 volatile uint16_t synth_osc_x2 = 0;
 volatile uint16_t synth_osc_inc0 = 0; // phase increment
 volatile uint16_t synth_osc_inc1 = 0; // phase increment
 volatile uint16_t synth_osc_inc2 = 0; // phase increment
 volatile uint8_t  synth_osc_out0 = 0;
 volatile uint8_t  synth_osc_out1 = 0;
 volatile uint8_t  synth_osc_out2 = 0;
 volatile int16_t  synth_osc_freq = 0; // base osc intonation
 volatile int16_t  synth_osc_detune[3] = {0}; // osc detune 
 volatile int16_t  synth_detune = 0; // detune geneal in semitone
 volatile uint16_t synth_16 = 0; // gerenal purpose 16 bit variable
 volatile uint8_t  synth_8 = 0; // gerenal purpose 8 bit variable
//-------LFO------
 volatile uint8_t  lfo_wave = 0; // 0 triangle, 1 square, 2 saw
 volatile uint16_t lfo_inc = 0;  // lfo phase increment
 volatile uint16_t lfo_x = 0;
 volatile uint8_t  lfo_mod = 0;  // 0.8 fixed point
 volatile uint8_t  lfo_out = 0; 
 volatile uint8_t  lfo_dest = LFO_PITCH; // see synth.h
 volatile uint8_t  lfo_polarity = LFO_POS; // 0 positive, 1 negative
//-------ENV------
 volatile uint8_t  env_state = 0; // off, attack, sustain, decay
 volatile uint8_t  env_attack = 0xff; // 5ms -> 1.2 s
 volatile uint8_t  env_pre_release = 0;
 volatile uint8_t  env_release = 0xff; // 5ms -> 1.2 s  
 volatile uint8_t  env_x = 0;
 volatile uint8_t  env_out = 0;
//-------LPF------
 volatile uint8_t  lpf_f = 255; // 0.8 fp value used in the lpf
 volatile uint8_t  lpf_base_f = 255; // 0.8 fp user value, used as base point to modulate with lfo
 volatile uint8_t  lpf_in = 0;
 volatile int16_t  lpf_delta = 0; // +/- 255 maximum, needs 16 bit signed
 volatile int16_t  lpf_out = 0;
 volatile int16_t  lpf_mult1 = 0;
 volatile uint8_t  al = 0; // lsb of lpf_delta
 volatile int8_t   ah = 0; // msb of lpf_delta
//-------SEQ------
 volatile uint8_t  seq_gate = 1; // on off sequencer
 volatile uint8_t  seq_note[16] = {0}; // 7->248 note, 0->7 rest, 248->255 ties
 volatile uint8_t  seq_timer = 0; // counter variable
 volatile uint8_t  seq_note_duration = 255; // 5ms -> 1.2s
 volatile uint8_t  seq_tempo = 100;  // 0.7 Hz -> 200 Hz
 volatile uint8_t  seq_step = 7; // 0->15
 volatile uint8_t  seq_step_counter = 0;
 volatile uint8_t  seq_pattern = 0; //0(fwd),1(bwd),2(pendulum),3(random)
 volatile uint8_t  seq_dir = 0; // direction of sequencer
 volatile uint8_t  seq_erase = 0; // variable used to erase the notes
 volatile uint8_t  seq_save = 0; // variable used to save the notes to eeprom
//-------LUT------
 const uint16_t note_lut[240] = // note from 16 Hz to 15KHz, in half semi-tone steps (?)
 {	// do do# re re# mi fa fa# sol sol# la la# si
	34,35,36,37,38,39,40,41,43,44,45,46,48,49,51,52,54,55,57,59,61,62,64,66,
    68,70,72,74,76,78,81,83,86,88,91,93,96,99,102,105,108,111,115,118,122,125,129,133,
	137,141,145,149,153,158,163,167,172,177,183,188,193,199,205,211,217,223,230,237,244,251,258,266,
    274,282,290,298,307,316,326,335,345,355,366,376,387,399,411,423,435,448,461,474,488,502,517,532,
	548,564,581,598,615,633,652,671,691,711,732,753,775,798,822,846,870,896,922,949,977,1006,1035,1066,
    1097,1129,1162,1196,1231,1267,1304,1343,1382,1423,1464,1507,1551,1597,1644,1692,1741,1793,1845,1900,1955,2013,2071,2132,	
	2194,2259,2325,2394,2463,2536,2609,2687,2765,2847,2929,3016,3103,3195,3288,3385,3483,3586,3690,3800,3910,4026,4142,4265,
    4389,4519,4650,4788,4926,5072,5219,5374,5530,5694,5859,6033,6207,6391,6576,6771,6967,7174,7381,7600,7820,8052,8285,8531,	
	8778,9039,9300,9576,9853,10146,10439,10749,11060,11389,11718,12066,12414,12783,13153,13544,13935,14349,14763,15202,15641,16106,16571,17064,
    17557,18079,18601,19154,19707,20293,20879,21499,22120,22778,23436,24132,24829,25567,26306,27088,27870,28698,29527,30405,31283,32213,33143,33143
 };
  const uint16_t lfo_lut[256] = // 0.1 Hz to 177 Hz 
 {
	33,35,37,39,41,43,45,47,51,55,59,63,67,71,75,79,
	85,91,97,103,109,115,121,127,135,143,151,159,167,175,183,191,
	201,211,221,231,241,251,261,271,283,295,307,319,331,343,355,367,
	381,395,409,423,437,451,465,479,495,511,527,543,559,575,591,607,
	625,643,661,679,697,715,733,751,771,791,811,831,851,871,891,911,
	933,955,977,999,1021,1043,1065,1087,1111,1135,1159,1183,1207,1231,1255,1279,
	1305,1331,1357,1383,1409,1435,1461,1487,1515,1543,1571,1599,1627,1655,1683,1711,
	1741,1771,1801,1831,1861,1891,1921,1951,1983,2015,2047,2079,2111,2143,2175,2207,
	2241,2275,2309,2343,2377,2411,2445,2479,2515,2551,2587,2623,2659,2695,2731,2767,
	2805,2843,2881,2919,2957,2995,3033,3071,3111,3151,3191,3231,3271,3311,3351,3391,
	3433,3475,3517,3559,3601,3643,3685,3727,3771,3815,3859,3903,3947,3991,4035,4079,
	4125,4171,4217,4263,4309,4355,4401,4447,4495,4543,4591,4639,4687,4735,4783,4831,
	4881,4931,4981,5031,5081,5131,5181,5231,5283,5335,5387,5439,5491,5543,5595,5647,
	5751,5855,5959,6063,6167,6271,6375,6479,6687,6895,7103,7311,7519,7727,7935,8143,
	8559,8975,9391,9807,10223,10639,11055,11471,12303,13135,13967,14799,15631,16463,17295,18127,
	19791,21455,23119,24783,26447,28111,29775,31439,34767,38095,41423,44751,48079,51407,54735,58063
 };
 
 const uint8_t log8_lut[256] = // log scale
 {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,
	3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,
	4,4,4,5,5,5,5,5,5,5,6,6,6,6,6,6,
	7,7,7,7,7,7,8,8,8,8,9,9,9,9,9,10,
	10,10,10,11,11,11,12,12,12,12,13,13,13,14,14,14,
	15,15,15,16,16,17,17,17,18,18,19,19,19,20,20,21,
	21,22,22,23,23,24,25,25,26,26,27,28,28,29,30,30,
	31,32,32,33,34,35,35,36,37,38,39,40,41,42,42,43,
	44,45,46,48,49,50,51,52,53,54,56,57,58,59,61,62,
	64,65,66,68,69,71,73,74,76,78,79,81,83,85,87,89,
	91,93,95,97,99,101,103,106,108,111,113,116,118,121,123,126,
	129,132,135,138,141,144,147,150,154,157,161,164,168,172,175,179,
	183,187,191,196,200,204,209,214,218,223,228,233,238,244,249,255
 };
 
 const uint32_t	square_lut[511] = // lut for mult a*b = [(a+b)^2 - (a-b)^2]/4
 {
	0,1,4,9,16,25,36,49,64,81,100,121,144,169,196,225,256,289,324,361,400,441,484,529,576,625,676,729,784,841,
	900,961,1024,1089,1156,1225,1296,1369,1444,1521,1600,1681,1764,1849,1936,2025,2116,2209,2304,2401,2500,2601,
	2704,2809,2916,3025,3136,3249,3364,3481,3600,3721,3844,3969,4096,4225,4356,4489,4624,4761,4900,5041,5184,5329,
	5476,5625,5776,5929,6084,6241,6400,6561,6724,6889,7056,7225,7396,7569,7744,7921,8100,8281,8464,8649,8836,9025,
	9216,9409,9604,9801,10000,10201,10404,10609,10816,11025,11236,11449,11664,11881,12100,12321,12544,12769,12996,
	13225,13456,13689,13924,14161,14400,14641,14884,15129,15376,15625,15876,16129,16384,16641,16900,17161,17424,
	17689,17956,18225,18496,18769,19044,19321,19600,19881,20164,20449,20736,21025,21316,21609,21904,22201,22500,
	22801,23104,23409,23716,24025,24336,24649,24964,25281,25600,25921,26244,26569,26896,27225,27556,27889,28224,
	28561,28900,29241,29584,29929,30276,30625,30976,31329,31684,32041,32400,32761,33124,33489,33856,34225,34596,
	34969,35344,35721,36100,36481,36864,37249,37636,38025,38416,38809,39204,39601,40000,40401,40804,41209,41616,
	42025,42436,42849,43264,43681,44100,44521,44944,45369,45796,46225,46656,47089,47524,47961,48400,48841,49284,
	49729,50176,50625,51076,51529,51984,52441,52900,53361,53824,54289,54756,55225,55696,56169,56644,57121,57600,
	58081,58564,59049,59536,60025,60516,61009,61504,62001,62500,63001,63504,64009,64516,65025,65536,66049,66564,
	67081,67600,68121,68644,69169,69696,70225,70756,71289,71824,72361,72900,73441,73984,74529,75076,75625,76176,
	76729,77284,77841,78400,78961,79524,80089,80656,81225,81796,82369,82944,83521,84100,84681,85264,85849,86436,
	87025,87616,88209,88804,89401,90000,90601,91204,91809,92416,93025,93636,94249,94864,95481,96100,96721,97344,
	97969,98596,99225,99856,100489,101124,101761,102400,103041,103684,104329,104976,105625,106276,106929,107584,
	108241,108900,109561,110224,110889,111556,112225,112896,113569,114244,114921,115600,116281,116964,117649,118336,
	119025,119716,120409,121104,121801,122500,123201,123904,124609,125316,126025,126736,127449,128164,128881,129600,
	130321,131044,131769,132496,133225,133956,134689,135424,136161,136900,137641,138384,139129,139876,140625,141376,
	142129,142884,143641,144400,145161,145924,146689,147456,148225,148996,149769,150544,151321,152100,152881,153664,
	154449,155236,156025,156816,157609,158404,159201,160000,160801,161604,162409,163216,164025,164836,165649,166464,
	167281,168100,168921,169744,170569,171396,172225,173056,173889,174724,175561,176400,177241,178084,178929,179776,
	180625,181476,182329,183184,184041,184900,185761,186624,187489,188356,189225,190096,190969,191844,192721,193600,
	194481,195364,196249,197136,198025,198916,199809,200704,201601,202500,203401,204304,205209,206116,207025,207936,
	208849,209764,210681,211600,212521,213444,214369,215296,216225,217156,218089,219024,219961,220900,221841,222784,
	223729,224676,225625,226576,227529,228484,229441,230400,231361,232324,233289,234256,235225,236196,237169,238144,
	239121,240100,241081,242064,243049,244036,245025,246016,247009,248004,249001,250000,251001,252004,253009,254016,
	255025,256036,257049,258064,259081,260100
 };
 
void Synth_Init(void)  // init pwm, timer2, timer0, interrupt, memory check
{	
	uint8_t x = 0;
	//pwm init
	APFCON1bits.CCP2SEL = 0; // pwm2 on rc3
    
	CCP2CON=0; // reset PWM2 control register

	PR2 = 255; // 31 khz @ 8 bit

    CCP2CON = 0b00111101;
    CCPR2L=0; // reset PWM2 duty cycle register L
    CCP2CONbits.DC2B = 0;

	CCPTMRS = 0; //select timer2 as time base for pwm
	TMR2IF=0; // timer2 interrupt flag reset
	T2CON= 0b00000100; // timer2 on prescaler 1:1
	while(!TMR2IF);
	TMR2IF=0; // timer2 interrupt flag reset
  
    PWM2_H_TRIS = 0;
        
 	//timer 0 5ms init
	OPTION_REGbits.TMR0CS = 0; // clock interno fosc/4
	OPTION_REGbits.PSA = 0; // prescaler on
	OPTION_REGbits.PS = 0b111; // prescaler rate 1:32
	TMR0 = 100;
	//interrupt init
	GIE = 1;
    PEIE = 1;    
	TMR0IE = 1;
	TMR2IE = 1; 

	/* check if there is a sequence in the eeprom memory 
	   by checking the status byte in memory if it is set load the seqeunce
	*/
	x = Read_EEPROM(MEMORY_STATUS);
	if(x==1)
	{
        Led_Blink(BLU,4);
		//------SYNTH-----
 		synth_volume = Read_EEPROM(16);

 		synth_pwm0_base = Read_EEPROM(17);
		synth_pwm0_base |= (Read_EEPROM(18)<<8);

		synth_pwm1_base = Read_EEPROM(19);
		synth_pwm1_base |= (Read_EEPROM(20)<<8);

		synth_osc_detune[0] = Read_EEPROM(21);
		synth_osc_detune[0] |= (Read_EEPROM(22)<<8);

		synth_osc_detune[1] = Read_EEPROM(23);
		synth_osc_detune[1] |= (Read_EEPROM(24)<<8);

		synth_osc_detune[2] = Read_EEPROM(25);
		synth_osc_detune[2] |= (Read_EEPROM(26)<<8);

		synth_detune = Read_EEPROM(27);
		synth_detune |= (Read_EEPROM(28)<<8); 		
 		//-------LFO------
 		lfo_wave = Read_EEPROM(29);

 		lfo_inc = Read_EEPROM(30);
		lfo_inc |= (Read_EEPROM(31)<<8);
 	
 		lfo_mod = Read_EEPROM(32);
 		lfo_dest = Read_EEPROM(33);
 		lfo_polarity = Read_EEPROM(34);
		//-------ENV------
 		env_attack = Read_EEPROM(35);
 		env_release = Read_EEPROM(36);
		//-------LPF------
 		lpf_base_f = Read_EEPROM(37);
		//-------SEQ------
		for(x=0;x<16;x++)
		{
			seq_note[x] = Read_EEPROM(x);
		}
 		seq_gate = Read_EEPROM(38);
 		seq_note_duration = Read_EEPROM(39);
 		seq_tempo = Read_EEPROM(40);
 		seq_step = Read_EEPROM(41);
 		seq_pattern = Read_EEPROM(42);
 		seq_dir = Read_EEPROM(43);
        synth_saw = Read_EEPROM(44);
 
		if(lfo_dest != LFO_PWM)
        {
           	synth_pwm0 = synth_pwm0_base;
           	synth_pwm1 = synth_pwm1_base;
        } 

        if(lfo_dest != LFO_LPF)
		{
			lpf_f = lpf_base_f;
		}	
	}
    else
    {
        Led_Blink(ROSSO,4);
    }
}

void Synth_Handler(void) // sequencer, envelope, update synth.
{    
	//-----SEQUENCER------ 
	if(seq_gate) 
	{
		seq_timer++;
		if(seq_timer == 1) // update synth only on the first "tick" of the timer
		{
            uint8_t nota_index = 0;
            if(seq_dir == 1) 
        	{
        		if(seq_step_counter > seq_step) // possible overflow if seq_step changes
        		{
        			seq_step_counter = seq_step;
        		}
                nota_index = seq_step - seq_step_counter; // bwd
        	}
        	else
        	{
        		nota_index = seq_step_counter; // fwd
        	}
            
			if(seq_note[nota_index] < 248 ) //  no tie
			{
				if(seq_note[nota_index] > 7) // nota
        		{
        			synth_osc_freq = (seq_note[nota_index]-8); // osc frequency        			      			
        			env_x = 0; // reset envelope_x
                    lfo_x = 0;
        			env_state = ENV_ATTACK;                    
                    synth_state = SYNTH_UPDATE_FREQ;
        		}
        		else //rest
        		{
        			env_state = ENV_OFF;
  					synth_state = SYNTH_OFF;
        		} 
			}
			else//ties
			{
				seq_timer = seq_tempo; // go to next step
			}
		}

		if(seq_timer >= seq_tempo ) 
		{
			seq_timer = 0;
			if(seq_pattern<3) // non random
			{
				seq_step_counter++; 
        		if(seq_step_counter > seq_step) 
        		{
        			seq_step_counter = 0;
        			if(seq_pattern == 2) 
        			{
        				seq_dir ^=1;   
                        seq_step_counter = 1;                        
					}
        		} 
			}
			else // pattern random
			{
				seq_step_counter = rand() % (seq_step+1);
			}
		}

		if( (seq_timer >= seq_note_duration) & (env_state != ENV_OFF) ) // start release
        {
       		env_state = ENV_RELEASE;  
            env_x = 0; 
            env_pre_release = env_out;       	     
        }
	}
	else // seq off
	{
		synth_state = SYNTH_OFF;
		env_state = ENV_OFF;
    	seq_timer = 0;         
        seq_step_counter = 0;
	}

	//-----ENVELOPE------- 
	if(env_state == ENV_OFF)
	{
		env_x = 0;
		env_out = 0; // envelope off
	}

	if(env_state == ENV_ATTACK)
	{
		if( (255 - env_attack) < env_x ) //check for overflow env_attack + env_x > 255
		{
			env_state = ENV_SUSTAIN;
			env_out = 255;
			env_x = 0;
		}
		else
		{
			env_x += env_attack;
			env_out = env_x;
		}       
	}

	if(env_state == ENV_RELEASE)
	{
        if( env_pre_release > env_release ) //check for overflow
		{
            if((env_pre_release - env_release) < env_x) //check for overflow
            {
                env_out = 0; // envelope off
                synth_state = SYNTH_OFF; 
                env_state = ENV_OFF;
            }
            else
            {
                env_x += env_release;
                env_out = env_pre_release - env_x;
            }				
		}  
        else
        {
            env_out = 0; // envelope off
            synth_state = SYNTH_OFF; 
            env_state = ENV_OFF;
        }
	}

	//-----LFO------ 
	if( (lfo_inc) & (synth_state != SYNTH_OFF) ) // lfo runs only if synth is not off
	{
		if(lfo_wave == 0) // triangle
		{
			if(lfo_x < 32768)
			{
				lfo_out = lfo_x>>8;
			}
			else
			{
				lfo_out = 255-(lfo_x>>8);
			}
		}
		
		if (lfo_wave == 1) // square
		{
			if(lfo_x < 32768)
			{
				lfo_out = 255;
			}
			else
			{
				lfo_out = 0;
			}
		}

		if(lfo_wave == 2) // saw
		{
			lfo_out = lfo_x>>8;
		}

		lfo_x += lfo_inc; // increment phase

		if(lfo_dest == LFO_PITCH)
		{ 
			synth_state = SYNTH_UPDATE_FREQ; 
		}

		if(lfo_dest == LFO_LPF)
		{
			// lfo_temp = (lfo_out * lfo_mod) / 256;
            uint8_t lfo_temp = (square_lut[lfo_out+lfo_mod]-square_lut[(lfo_out>lfo_mod)?(lfo_out-lfo_mod):(lfo_mod-lfo_out)])>>10;
            
            if(!lfo_polarity)
            {
            	if(255-lpf_base_f>lfo_temp) // lpf_base_f + lfo_temp < 255
            	{
            		lpf_f = lpf_base_f + lfo_temp;
            	}
            	else
            	{
            		lpf_f = 255;
            	}                	 
            }
            else
            {
            	if(lpf_base_f > lfo_temp)
            	{
            		lpf_f = lpf_base_f  - lfo_temp; 
            	}
            	else
            	{
            		lpf_f = 0;
            	}                	
            }                    
		}

		if(lfo_dest == LFO_PWM)
		{
			// lfo_temp = (lfo_out * lfo_mod);
            uint16_t lfo_temp = (square_lut[lfo_out+lfo_mod]-square_lut[(lfo_out>lfo_mod)?(lfo_out-lfo_mod):(lfo_mod-lfo_out)])>>2;
            
            if(!lfo_polarity)
            {
            	if( (0xffff-synth_pwm0_base) > lfo_temp ) 
            	{
            		synth_pwm0 = synth_pwm0_base + lfo_temp;
            	}
            	else
            	{
            		synth_pwm0 = 0xffff;
            	}   

            	if( (0xffff-synth_pwm1_base) > lfo_temp ) 
            	{
            		synth_pwm1 = synth_pwm1_base + lfo_temp;
            	}
            	else
            	{
            		synth_pwm1 = 0xffff;
            	}             	 
            }
            else
            {
            	if(synth_pwm0_base > lfo_temp)
            	{
            		synth_pwm0 = synth_pwm0_base - lfo_temp; 
            	}
            	else
            	{
            		synth_pwm0 = 0;
            	}   

            	if(synth_pwm1_base > lfo_temp)
            	{
            		synth_pwm1 = synth_pwm1_base - lfo_temp; 
            	}
            	else
            	{
            		synth_pwm1 = 0;
            	}             	
            }                    
		}
	}  

    //-----SYNTHESIZER---- 
	if(synth_state == SYNTH_OFF) 
	{
		synth_gate = 0;
		CCPR2L=0; 
		CCP2CONbits.DC2B = 0;
		synth_osc_x0 = 0;
        synth_osc_x1 = 0;
        synth_osc_x2 = 0;
        lfo_x = 0;
		lfo_out = 0;
	}
	
    //---FREQ UPDATE----
	if(synth_state == SYNTH_UPDATE_FREQ)
	{		
		synth_state = SYNTH_RUNNING;
		uint8_t x = 0;
		int16_t synth_base_freq[3]; // buffer so they change together
		for(x=0;x<3;x++)
        {
        	synth_base_freq[x] = synth_osc_freq + synth_osc_detune[x] + synth_detune;
            if(lfo_dest == LFO_PITCH)
            {
                uint8_t lfo_temp = (square_lut[lfo_out+lfo_mod] 
            				  - square_lut[(lfo_out>lfo_mod)?(lfo_out-lfo_mod):(lfo_mod-lfo_out)])>>10;
                
                if(!lfo_polarity)
                {
                	synth_base_freq[x] = synth_base_freq[x] + lfo_temp; 
                }
                else
                {
                	synth_base_freq[x] = synth_base_freq[x] - lfo_temp; 
                }                
            }
            
            if( synth_base_freq[x] > 239) synth_base_freq[x] = 239;
        	if( synth_base_freq[x] < 0)   synth_base_freq[x] = 0;            
        }
        synth_gate = 0;
        synth_osc_inc0 = note_lut[synth_base_freq[0]];
        synth_osc_inc1 = note_lut[synth_base_freq[1]];
        synth_osc_inc2 = note_lut[synth_base_freq[2]];
        synth_gate = 1;        
    }
}                

uint8_t Synth_Read_Step(void)
{
	return seq_step;
}

void Synth_Seq_Start(void) // start seq, stop seq 
{   
	seq_gate ^= 1;
    if(seq_gate)
    {
    	seq_step_counter = 0;
		seq_timer = 0;
        Led_Blink(ROSSO,4);
    }
    else
    {       
        Led_Blink(BLU,4);        
    }     
}

void Synth_Hold(uint8_t but, uint8_t led) // 16 note del seq, tempo and note duration
{
	ADC_Lock();

	switch(led)
	{
		case 0:
			Led_On(ROSSO);
			break;
		case 1:
			Led_On(BLU);
			break;
		case 2:
			Led_On(ROSSO);
			break;
		case 3:
			Led_On(BLU);
			break;	
		case 4:
			Led_On(ROSSO);
			break;		
	}

	if(but < 8) 
    {
    	if(adc[FLAG] & FLAG1)
    	{  
            seq_note[but*2] = adc[POT1];
    	}

    	if(adc[FLAG] & FLAG2)
    	{  
            seq_note[(but*2)+1] = 255-adc[POT2];
    	}
    }
    else 
    { 
    	if(adc[FLAG] & FLAG1) 
    	{  
    		seq_tempo = adc[POT1]; 
    		if(seq_tempo == 0) seq_tempo = 1;    		 			   		
    	}

    	if(adc[FLAG] & FLAG2) 
    	{  
    	    seq_note_duration = 255-adc[POT2];
    	}
    }
}

void Synth_Menu(uint8_t synth_menu) 
{
	ADC_Lock();
	switch(synth_menu)
	{
		case 0: // (square0) - (detune0) 
            Led_Blink(ROSSO,2);
			if(adc[FLAG] & FLAG1)
			{  				
                synth_pwm0_base = 256 *(adc[POT1]);
                if(lfo_dest != LFO_PWM)
                {
                	synth_pwm0 = synth_pwm0_base;
                }
 			}            
            if(adc[FLAG] & FLAG2) 
            {
            	uint8_t temp_adc2 = (255 - adc[POT2]); 
            	if(temp_adc2 < 128)
            	{
            		synth_osc_detune[0] = temp_adc2>>2;
            	}
            	else
            	{
            		synth_osc_detune[0] = -((temp_adc2-128)>>2);
            	}
            }
    		break;
    	case 1: // (square1) - (detune1)
 			Led_Blink(BLU,2);
			if(adc[FLAG] & FLAG1)
			{         
				synth_pwm1_base = 256 *(adc[POT1]);
                if(lfo_dest != LFO_PWM)
                {
                	synth_pwm1 = synth_pwm1_base;
                } 
            }            
            if(adc[FLAG] & FLAG2) 
            {
            	uint8_t temp_adc2 = (255 - adc[POT2]); 
            	if(temp_adc2 < 128)
            	{
            		synth_osc_detune[1] = temp_adc2>>2;
            	}
            	else
            	{
            		synth_osc_detune[1] = -((temp_adc2-128)>>2);
            	}
            }
			break;	
    	case 2: // (saw) - (detune2)   
			Led_Blink(ROSSO,2);
         	if(adc[FLAG] & FLAG1)
			{         
				synth_saw = adc[POT1]>>7;
            }
            if(adc[FLAG] & FLAG2) 
            {
            	uint8_t temp_adc2 = (255 - adc[POT2]); 
            	if(temp_adc2 < 128)
            	{
            		synth_osc_detune[2] = temp_adc2>>2;
            	}
            	else
            	{
            		synth_osc_detune[2] = -((temp_adc2-128)>>2);
            	}
            }
			break;
		case 3: // (attack) - (release)   
			Led_Blink(BLU,2);
			if(adc[FLAG] & FLAG1)
			{
                env_attack = log8_lut[255-adc[POT1]];
                if(env_attack == 0) env_attack = 1;                
            }
            if(adc[FLAG] & FLAG2)
			{
                env_release =  log8_lut[adc[POT2]];  
                if(env_release == 0) env_release = 1;
            }
			break;	
		case 4: // (volume) - (lpf)
			Led_Blink(ROSSO,2);
			if(adc[FLAG] & FLAG1)
			{
				synth_volume = log8_lut[adc[POT1]];
			}

			if(adc[FLAG] & FLAG2)
			{
                lpf_base_f = log8_lut[255-adc[POT2]];
				if(lfo_dest != LFO_LPF)
				{
					lpf_f = lpf_base_f;
				}	   
  			}
			break;	
		case 5: // (lfo freq) - (lfo mod amount and polarity) 
            Led_Blink(lfo_polarity+1,2);
			if(adc[FLAG] & FLAG1)
			{
                lfo_inc = lfo_lut[adc[POT1]];			 
			}
			if(adc[FLAG] & FLAG2)
			{
				uint8_t temp_adc2 = 255-adc[POT2];
				if(temp_adc2<128)
				{
					lfo_mod = log8_lut[temp_adc2*2];
					lfo_polarity = LFO_POS;
				}
				else
				{
					lfo_mod = log8_lut[(temp_adc2-128)*2];
					lfo_polarity = LFO_NEG;
				}
			}		
			break;	
		case 6: // (lfo_wave) - (lfo_dest)
			Led_Blink(lfo_wave+1,2);
			if(adc[FLAG] & FLAG1)
			{
				lfo_wave = (adc[POT1]*3)>>8;
			}

			if(adc[FLAG] & FLAG2)
			{
				lfo_dest = ((255-adc[POT2])*3)>>8;  // divide by 3
				if(lfo_dest != LFO_LPF)
				{
					lpf_f = lpf_base_f;
				}
				if (lfo_dest != LFO_PWM)
				{
					synth_pwm0 = synth_pwm0_base;
					synth_pwm1 = synth_pwm1_base;
				}			
			}
			break;	
		case 7: // (step) - (pattern)
            Led_Blink(seq_pattern+1,2);
 			if(adc[FLAG] & FLAG1)
			{
				seq_step = (adc[POT1]>>4); // 0->15
            }

			if(adc[FLAG] & FLAG2)
			{
				seq_pattern = (255-adc[POT2]) >> 6; // 0->3
				if(seq_pattern == 0) seq_dir = 0; // avanti
				if(seq_pattern == 1) seq_dir = 1; // indietro
				if(seq_pattern == 2) seq_dir = 0; // pendolo
				if(seq_pattern == 3) seq_dir = 0; // random
			}            	
			break;
		case 8: // (save) - ( erase )
			if(seq_save) seq_erase = 0;
            if(seq_erase) seq_save = 0;
            
            Led_Blink( (seq_save+seq_erase+1) ,2);

            if(adc[FLAG] & FLAG1) // save
			{
				if(adc[POT1]>250) 
				{
					if(seq_save == 0)
					{
						seq_save = 1;						
					}

					if(seq_save == 2)
					{
						seq_save = 3;
					}
					
				}
				if( adc[POT1]<5 )	
				{
					if(seq_save == 1)
					{
						seq_save = 2;
					}
				}

				if(seq_save == 3) // SAVE
				{
					seq_save = 0;
					// save to eeprom
					//------SYNTH-----
					Write_EEPROM(1,MEMORY_STATUS);
 					Write_EEPROM(synth_volume,16);
			
 					Write_EEPROM(synth_pwm0_base,17);
 					Write_EEPROM(synth_pwm0_base>>8,18);
								
					Write_EEPROM(synth_pwm1_base,19);
					Write_EEPROM(synth_pwm1_base>>8,20);
			
					Write_EEPROM(synth_osc_detune[0],21);
					Write_EEPROM(synth_osc_detune[0]>>8,22);			
					Write_EEPROM(synth_osc_detune[1],23);
					Write_EEPROM(synth_osc_detune[1]>>8,24);			
					Write_EEPROM(synth_osc_detune[2],25);
					Write_EEPROM(synth_osc_detune[2]>>8,26);
			
					Write_EEPROM(synth_detune,27);
					Write_EEPROM(synth_detune>>8,28); 					
 					//-------LFO------
 					Write_EEPROM(lfo_wave,29);
			
 					Write_EEPROM(lfo_inc,30);
 					Write_EEPROM(lfo_inc>>8,31);
 				
 					Write_EEPROM(lfo_mod,32);
 					Write_EEPROM(lfo_dest,33);
 					Write_EEPROM(lfo_polarity,34);			
					//-------ENV------
 					Write_EEPROM(env_attack,35);
 					Write_EEPROM(env_release,36);			
					//-------LPF------
 					Write_EEPROM(lpf_base_f,37);			
					//-------SEQ------
					for(seq_save=0;seq_save<16;seq_save++)
					{
						Write_EEPROM(seq_note[seq_save],seq_save);
					}
 					Write_EEPROM(seq_gate,38);
 					Write_EEPROM(seq_note_duration,39);
 					Write_EEPROM(seq_tempo,40);
 					Write_EEPROM(seq_step,41);
 					Write_EEPROM(seq_pattern,42);
 					Write_EEPROM(seq_dir,43);
                    Write_EEPROM(synth_saw,44);
					Write_EEPROM(1,MEMORY_STATUS);
					seq_save = 4;
				}
			}

			if(adc[FLAG] & FLAG2) // erase
			{
				if((255-adc[POT2])>250) 
				{
					if(seq_erase == 0)
					{
						seq_erase = 1;
					}

					if(seq_erase == 2)
					{
						seq_erase = 3;
					}
					
				}
				if( (255-adc[POT2])<5 )	
				{
					if(seq_erase == 1)
					{
						seq_erase = 2;
					}
				}

				if(seq_erase == 3) 
				{
                    seq_erase = 0;
					//------SYNTH-----
 					synth_volume = 240; 
 					synth_pwm0 = 32768; // dc = (synth_pwm0/65536)*100
 					synth_pwm1 = 32768; // dc = (synth_pwm0/65536)*100
 					synth_pwm0_base = 32768; 
 					synth_pwm1_base = 32768; 
 					synth_osc_detune[0] = 0; 
 					synth_osc_detune[1] = 0;
 					synth_osc_detune[2] = 0;
 					synth_detune = 0; // detune geneal in semitone
                    synth_saw = 0;
 				
 					lfo_wave = 0; // 0 triangle, 1 square, 2 saw, 3 random
 					lfo_inc = 0;  // lfo phase increment
 					lfo_mod = 0;  // 0.8 fixed point
 					lfo_dest = LFO_PITCH; // see synth.h
 					lfo_polarity = LFO_POS; // 0 positive, 1 negative
					
 					env_attack = 0xff; // 5ms -> 1.2 s
 					env_release = 0xff; // 5ms -> 1.2 s  
					
 					lpf_f = 255; // 0.8 fp value used in the lpf
 					lpf_base_f = 255; // 0.8 fp user value, used as base point to modulate with lfo
 				
 					seq_gate = 1; // on off sequencer
 					seq_timer = 0; // counter variable
 					seq_note_duration = 255; // 5ms -> 1.2s
 					seq_tempo = 100;  // 0.7 Hz -> 200 Hz
 					seq_step = 7; // 0->15
 					seq_step_counter = 0;
 					seq_pattern = 0; //0(fwd),1(bwd),2(pendulum),3(random)
 					seq_dir = 0; // direction of sequencer
 					for(seq_erase=0;seq_erase<16;seq_erase++)
					{
						seq_note[seq_erase] = 0;
					}
					Write_EEPROM(0,MEMORY_STATUS);
					seq_erase = 4;
				}					
			}
			break;	
	}
}

void Synth_Save_Erase_Clear(void)
{
	seq_erase = 0;
	seq_save = 0;
}

void Synth_Single(uint8_t but) // retrigger sequencer from selected step
{
	switch(but)
	{
		case 0: // retrigger 0
			seq_step_counter = 0;
			seq_timer = 0;
			Led_Blink(ROSSO,2);					
            break;
        case 1: // retrigger 1/4
			seq_step_counter = seq_step>>2;
			seq_timer = 0;
			Led_Blink(BLU,2);						
            break;
        case 2: // retrigger 1/2
			seq_step_counter = seq_step>>1;
			seq_timer = 0;
			Led_Blink(ROSSO,2);						
            break;
        case 3: // retrigger 3/4
			seq_step_counter = (seq_step*3)>>2;
			seq_timer = 0;
			Led_Blink(BLU,2);						
            break;         
	}
}

void Synth_Shift_Single(uint8_t but) // detune in semitones, tempo 
{    
	switch(but)
	{
        case 0: // detune --
			if(synth_detune > -120) synth_detune-=2;
			Led_Blink(ROSSO,4);					
            break;
        case 1: // detune ++
			if(synth_detune < 120) synth_detune+=2;
			Led_Blink(BLU,4);					
            break;   
        case 2: // tempo--
			if(seq_tempo < 255) seq_tempo++;
			Led_Blink(ROSSO,4);					
            break;  
        case 3: // tempo++			
            if(seq_tempo > 1) seq_tempo--;
			Led_Blink(BLU,4);					
            break;         
	}
}

void interrupt isr(void) 
{
    if(TMR2IF) 
	{   
		if(synth_gate)
		{
			// osc0
			synth_osc_out0 = (synth_osc_x0 < synth_pwm0) ? 255 : 0 ;
            // osc1
            synth_osc_out1 = (synth_osc_x1 < synth_pwm1) ? 255 : 0 ;
            // osc2
            synth_osc_out2 = (synth_saw) ? (255-(synth_osc_x2>>8)) : 0;
            // phase
            synth_osc_x0 += synth_osc_inc0;
            synth_osc_x1 += synth_osc_inc1;    
            synth_osc_x2 += synth_osc_inc2;  
            // mix and limit output dei osc       
            synth_16 = synth_osc_out0 + synth_osc_out1 + synth_osc_out2;
            lpf_in = (synth_16>255)?255:synth_16;
            // volume
            lpf_in = (square_lut[lpf_in + synth_volume]-square_lut[(lpf_in>synth_volume)?(lpf_in-synth_volume):(synth_volume-lpf_in)])>>10;
            // envelope
            lpf_in = (square_lut[lpf_in + env_out]-square_lut[(lpf_in>env_out)?(lpf_in-env_out):(env_out-lpf_in)])>>10;
            // lpf
            lpf_delta = lpf_in - lpf_out;
            al = lpf_delta;
            ah = lpf_delta>>8;
            lpf_mult1 = ( ((square_lut[al+lpf_f]-square_lut[(al>lpf_f)?(al-lpf_f):(lpf_f-al)])>>10)
                        + ((square_lut[ah+lpf_f]-square_lut[(ah>lpf_f)?(ah-lpf_f):(lpf_f-ah)])>>2));
            lpf_out += lpf_mult1;
             // limit lpf out
            synth_16 = (lpf_out>0)?lpf_out:0;
            synth_8 = (synth_16>255)?255:synth_16;
            // pwm out                             
            CCPR2L = synth_8;
        }		         
		TMR2IF=0; // Resetto il flag interrupt su timer 2
	}
    if(TMR0IF)
	{
		TMR0 = 100; // RESETTO VALORE TIMER
        flag_timer_5ms = 1;        
        TMR0IF=0; // Resetto il flag interrupt su timer 0
	}
}