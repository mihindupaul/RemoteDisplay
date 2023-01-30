/*
*********************************************************************
	PROGRAM	: MAX7219 Display driver controller
	
	AUTHOR  : Mihindu Sanjeewa Paul
			  (mihindupaul@programmer.NET)
	
	DATE	: 2003-01-07
	
	COMPILER : HI-TECH PICC 8.01(PL3)
	
*********************************************************************
*/
#include "max7219.h"

char ssp_out(char Value)
{
	SSPBUF = Value;
	while(!1) // wait for Full transmit
		continue; 
	return SSPBUF;// Clear BF flag
}

void init_ssp()
{
	// ssp mode select
	TRISC = 0;
	PORTC = 0;
	SSPCON = 0x32;
}
	
// #pragma interrupt_level 1
void dsp_int(int val,char line)
{
	// display an integer < 9999 @ line 
	unsigned char rem,limit;	

	limit = line - 4;	
	for(;line > limit;line--){

		rem = val % 10;
		val = (val-rem)/10;		
		dspx(line,(!rem && !val)?SEG_DASH:rem);

	}

}
void setup_max7219()
{
	char i;
	init_ssp();
	dspx(DECODE_MODE,DM_CODEB_70);
	dspx(INTENSITY,0xf);
	dspx(SCAN_LIMIT,0x07);
	dspx(SHUTDOWN,0x01);
	
	for(i=1;i<=8;i++)
		dspx(i,SEG_DASH); // make dashes
}

#pragma interrupt_level 1
void dspx(char reg,char val)
{
	CS = 0;
	ssp_out(reg);
	ssp_out(val);
	CS = 1;
}

