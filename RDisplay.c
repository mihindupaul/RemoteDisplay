/*
*********************************************************************
	PROGRAM	: Remote 7-seg Display system
	
	PROJECT	: PID based Temperature controller
	
	AUTHOR  : Mihindu Sanjeewa Paul
			  (mihindupaul@programmer.NET)
	
	DATE	: 2003-01-07
	
	COMPILER : HI-TECH PICC 8.01(PL3)
	
*********************************************************************
*/
#include <pic.h>
#include <stdio.h>
#include <hitech.h>
#include "always.h"
#include "rdisplay.h"
#include "max7219.h"
#include "serial.h"



// Global Variables
unsigned char	rx_buffer[12];					// Buffer for in/out packets
unsigned char	tx_buffer[12] = {MY_ID,~MY_ID};	// Buffer for output data
unsigned char	seconds	= 0;					// second count
static volatile unsigned char	buf_pointer = -1; // pointer for Rx buffer

/*
	Function	: Main()
	Calls		: init_ports()
				  init_ports();
			      init_display();
			      init_timer0();
				  serial_setup();
*/
void main()
{
	init_ports();
	//setup_max7219();
	//init_timer0();
	serial_setup();
	//set_color(RED); //	colour selection
	ei();			// enable all interrupts
	
	// Main loop goes hear
	while(TRUE){
	}
}

/*
	Function	: The Interrupt service routine(ISR)
	Calls		: getch()
				  process_buffer()	
*/

#pragma interrupt_level 1
static void interrupt my_isr(void)
{
	unsigned char rx_char;
	
	if(RCIF){					// Rx interrupt	
		
		rx_char = getch();
		YEL_LED = 1;
		switch(buf_pointer){
		case -1:				// if buffer is empty
			if(rx_char == SOH){
				buf_pointer = 0;
				YEL_LED = 1;
			}
			break;
		case 0:					// check the id
			if(rx_char != MY_ID) buf_pointer = -1;
			else rx_buffer[buf_pointer++] = rx_char;
			break;
		case 1:					// check id & idc reset not compared
			if(rx_buffer[0] == ~rx_char) buf_pointer = -1;
			else rx_buffer[buf_pointer++] = rx_char;
			break;
		case 11:				// when the CRC byte arrived(End of packet) 
			rx_buffer[buf_pointer] = rx_char;	// store the CRC
			process_buffer(); 
			buf_pointer = -1;					// reset buffer
			YEL_LED = 0;
			break;
		default:
			rx_buffer[buf_pointer++] = rx_char;	// store the value on buffer
		}
		
	}
	// Timer0 
	if(T0IF){
		if(seconds >= 10){
			seconds = 0;
		}
		else	seconds++;
		T0IF = 0;				// Clear timer flag
	}
	YEL_LED = 0;
}

/*
	I/O port Initialization 
*/
void init_ports()
{

	TRISA = 0x0;	// make A input
	PORTA = 0x10;

	PORTA = 0x00;
	TRISA = 0xff;	// all pins output

	TRISB = 0x0;	// make B output
	PORTB = 0x0;
	
}


void init_timer0()
{
	OPTION = 0b00000111;	// set option_reg(1:256 prescaler)
//	T0IE = 1;				// enable timer interrupt
}

/*
	Function 	: process_buffer()
	Details		: Processes the recived packet after the 13 byte arrival.
	Called by	: ISR
	calls		: update_dispaly()
				  Get_DOW_CRC()	
*/
void	process_buffer()
{
	if(Get_DOW_CRC(rx_buffer+2,9) == rx_buffer[11])		//	Correct reciving of packet
	{
		//	Decode mode selection
		switch(rx_buffer[2] & 0b00000110){
		case 0:
			dspx(DECODE_MODE,DM_CODEB_70);
			break;
		case 2:
			dspx(DECODE_MODE,DM_CODEB_30);
			break;
		case 4:
			dspx(DECODE_MODE,DM_NULL);
		}
		
		set_color((rx_buffer[2]& 0b0000001)?RED:GRN);	
		update_display();

	}
}

/*
	Function	: Update_display
	Details		: Update the all 8 7-seg with data packet data
				  sent by the Tx
	Called by	: process_buffer()
*/
void update_display()
{
	BYTE i;
	for(i=0;i < 8;i++) 
		dspx(i+1,rx_buffer[3+i]);
}

/*
	Function	: send_packet
	Details		: Send a reply packet to the Tx
*/
void send_packet()
{
	tx_buffer[2] = 0x2; // Set control value.
	// fill data buffer
	//
	//
	//
	tx_buffer[11] = Get_DOW_CRC(tx_buffer+2,9); // embed CRC
	putch(SOH);
	putchbuffer(tx_buffer,9);
}

/*
	Function	: Colour changer
*/
#pragma interrupt level 1
void set_color(unsigned char c)
{
	if(c == GRN){
		RED_SELECT = 1;
		GRN_SELECT = 0;
	}else
	{
		GRN_SELECT = 1;
		RED_SELECT = 0;
	}
}	
