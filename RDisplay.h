//////////////////////////////////////////////////////
//
// Application definitions
//
//////////////////////////////////////////////////////

#define MAX_TEMP 1000


#define DEBOUNCE_DELAY	100

#define bitset(var,bitno) ((var) |= 1 << (bitno))
#define bitclr(var,bitno) ((var) &= ~(1 << (bitno)))


//////////////////////////////////////////////////////////   
// Definitions of port pins
//////////////////////////////////////////////////////////

// Outputs
#define YEL_LED			RB7
#define YEL_LED_TRIS	TRISB7
#define GRN_LED			RB6
#define GRN_LED_TRIS	TRISB6

#define GRN_SELECT	RB2
#define RED_SELECT	RB3

#define GRN 0
#define RED 1

#define	PERIOD	1000000		// period in uS - one second here
#define	XTAL	4000000		// crystal frequency - 4MHz
#define	ICLK	(XTAL/4)	// crystal is divided by four
#define	SCALE	16		// prescale by 16 - check for overflow!

#define	PRELOAD	PERIOD*ICLK/SCALE/1000000

/////////////////////////////////////////////////////////////////
// All the protocol related defines are going hear
/////////////////////////////////////////////////////////////////

#define SOH	0xff

#define MY_ID	 75

#define CTRL_ACK	1
#define CTRL_NAK	0



#define ACK(x)	x & 0b10000000
#define NAK(x)	!ACK(x)

#define DATA(x)	x & 0b01000000
#define CTRL(x)	!DATA(x)

#define CONTROL(x) x & 0b00111111 // Returns control value

#define CTRL_0	1
#define CTRL_1	1
#define CTRL_2	1
#define CTRL_3	1

#define DATA_1	1
#define DATA_2	1
#define DATA_3	1


// 
// Local function declerations
//
void init_ports();
void init_ssp();
void init_display();
void init_timer0();

unsigned char Get_DOW_CRC(unsigned char *pBuffer, int nSize);
int  get_value(const char buffer[]);
static void interrupt my_isr(void);
void	process_buffer();
void	update_display();
void	send_packet();
void	set_color(unsigned char c);





