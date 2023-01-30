//
// Max7219 related functions
//
#include <pic.h>

#define INT_ZERO {0x10,0x10,0x10,0x10}

#define LINE_1	4
#define LINE_2	8


//	Registers
#define NO_OP	0x0

#define D0	0x1
#define D1	0x2
#define D2	0x3
#define D3	0x4
#define D4	0x5
#define D5	0x6
#define D6	0x7
#define D7	0x8

#define DECODE_MODE		0x9
#define INTENSITY		0xA
#define	SCAN_LIMIT		0xB
#define SHUTDOWN		0xC
#define DISPLAY_TEST	0xF

//	Decode modes
#define DM_NULL			0x0
#define DM_CODEB_0		0x1
#define DM_CODEB_30		0xF
#define DM_CODEB_70		0xFF

//scan limit
#define SL_0	0x0
#define SL_1	0x1
#define SL_2	0x2
#define SL_3	0x3
#define SL_4	0x4
#define SL_5	0x5
#define SL_6	0x6
#define SL_7	0x7

//	Characters(code B)
#define SEG_0	0x0
#define SEG_1	0x1
#define SEG_2	0x2
#define SEG_3	0x3
#define SEG_4	0x4
#define SEG_5	0x5
#define SEG_6	0x6
#define SEG_7	0x7
#define SEG_8	0x8
#define SEG_9	0x9

#define SEG_E		0xB
#define SEG_H		0xC
#define SEG_L		0xD
#define SEG_P		0xE
#define SEG_BLANK	0xF
#define SEG_DASH	0xA

//	Characters	(no decode mode)

//	list of other supported characters on 7-segments



#define MAKE_DECIMAL(digit)	dspx(digit)


// Change this value for Chip select pin

static volatile bit CS		@ (unsigned)&PORTC*8+2;

extern char ssp_out(char Value);
extern void dspx(char reg,char val);
extern void dsp_int(int val,char line);
extern void setup_max7219();
