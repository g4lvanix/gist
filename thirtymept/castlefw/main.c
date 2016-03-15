#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

// 6 second "dits" --> with that my callsign is a 5 minute, 54 second transmission
#define CWT0OVFS 4482	// number of timer0 overflows per dit --> T0OVS = (ditlength/s * 1MHz/2048)

#define MORSELEN 17

typedef enum{
	FREQ_DIT_0,
	FREQ_DIT_1,
	FREQ_DIT_2,
	FREQ_DIT_3,
	FREQ_DIT_4,
	FREQ_DAH_0,
	FREQ_DAH_1,
	FREQ_DAH_2,
	FREQ_DAH_3,
	FREQ_DAH_4,
	FREQ_DAH_5
} tTxFreq;

typedef enum{
	MEPT_TX_START,
	MEPT_TX_PROGRESS,
	MEPT_TX_DELAY
} tMeptState;

// timer0 overflow counter
volatile uint16_t t0ovfcnt;

// counters for keying functions
volatile uint8_t symcnt;
volatile uint8_t unitcnt;
volatile uint8_t ditcnt;
volatile uint8_t dahcnt;

volatile tTxFreq nextfreq;
volatile uint8_t txcomplete;

// CASTLE 10  10-FSK tones for 10145000 Hz
// Word order: first 5 are dit freqs, last 5 are dah freqs
const uint8_t castleTones[10][5] = {
	{0x7d,0x67,0x63,0x28,0x0},
	{0x45,0x67,0x63,0x28,0x0},
	{0xe1,0x67,0x63,0x28,0x0},
	{0xd6,0x67,0x63,0x28,0x0},
	{0xa,0x67,0x63,0x28,0x0},
	{0xaf,0x67,0x63,0x28,0x0},
	{0x8,0xe7,0x63,0x28,0x0},
	{0x34,0xe7,0x63,0x28,0x0},
	{0xe2,0xe7,0x63,0x28,0x0},
	{0xc6,0xe7,0x63,0x28,0x0},
};

// 1 unit = 1 bit
// use morsepack.py to generate these bytes
// -.. .-.. .---- -.-- .
const int8_t call[21] = {
	1,0,0,-1,
	0,1,0,0,-1,
	0,1,1,1,1,-1,
	1,0,1,1,-1,
	0
};


void spi_init(void)
{
	DDRA |= (1<<PA0);	// AD9850 FQUD
	DDRB |= (1<<PB7) | (1<<PB6);
	USICR |= (1<<USIWM0) | (1<<USICS1) | (1<<USICLK);
}

uint8_t spi_transfer(uint8_t data)
{
	USIDR = data;
	USISR |= (1<<USIOIF);
	USICR |= (1<<USIWM0) | (1<<USICS1) | (1<<USICLK) | (1<<USITC);
	while(!(USISR & (1<<USIOIF))) USICR |= (1<<USITC);
	return USIDR;
}

void ad9850_setfreq(const uint8_t* conf)
{
	for(uint8_t i = 0; i < 5; i++) spi_transfer(conf[i]);
	PORTA |= (1<<PA0);
	PORTA &= ~(1<<PA0);
}

void key_init(void)
{
	DDRD |= (1<<PD0);
	PORTD &= ~(1<<PD0);
}

void keyfsk(void)
{
	if (symcnt < 21)
	{
		if(call[symcnt] == -1)
		{
			ditcnt = 0;
			dahcnt = 0;
			symcnt++;
		}

		if(call[symcnt] == 1)
		{
			nextfreq = 5 + dahcnt;
			dahcnt++;
		} else {
			nextfreq = ditcnt;
			ditcnt++;
		}

		symcnt++;
	} else {
		txcomplete = 1;
	}
}

void timer0_start(void)
{
	t0ovfcnt = 0;
	TCNT0 = 0;
	// enable timer0 interrupt
	TIMSK |= (1<<TOIE0);
	// start timer0, prescaler 8
	TCCR0B |= (1<<CS01);
}

void timer0_stop(void)
{
	TCCR0B &= ~(1<<CS01);
	TIMSK &= ~(1<<TOIE0);
}

int main(void)
{
	tMeptState currstate,nextstate;
	tTxFreq currfreq;

	spi_init();
	key_init();

	currstate = MEPT_TX_START;
	nextstate = MEPT_TX_START;

	currfreq = FREQ_DIT_0;
	nextfreq = FREQ_DIT_0;

	ad9850_setfreq(castleTones[FREQ_DIT_0]);

	sei();

	while(1)
	{
		switch(currstate)
		{
			case MEPT_TX_START:
				// reset all status variables
				txcomplete = 0;
				unitcnt = 0;
				symcnt = 0;
				ditcnt = 0;
				dahcnt = 0;

				currfreq = FREQ_DIT_0;
				nextfreq = FREQ_DIT_0;

				timer0_stop();
				// key up transmitter
				//PORTD |= (1<<PD0);
				timer0_start();
				nextstate = MEPT_TX_PROGRESS;
				break;

			case MEPT_TX_PROGRESS:
				if(currfreq != nextfreq)
				{

					ad9850_setfreq(castleTones[nextfreq]);
					// key up transmitter
					PORTD |= (1<<PD0);
					currfreq = nextfreq;
				}
				if(txcomplete){
					// key down transmitter
					PORTD &= ~(1<<PD0);
					nextstate = MEPT_TX_DELAY;
				} else nextstate = MEPT_TX_PROGRESS;
				break;

			case MEPT_TX_DELAY:
				if(unitcnt < 20) nextstate = MEPT_TX_DELAY;
				else nextstate = MEPT_TX_START;
				break;
		}
		currstate = nextstate;
	}
	return 0;
}

ISR(TIMER0_OVF_vect)
{
	if(t0ovfcnt < CWT0OVFS) t0ovfcnt++;
	else {
		t0ovfcnt = 0;
 		unitcnt++;
		keyfsk();
	}
}
