#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

// 6 second "dits" --> with that my callsign is a 5 minute, 54 second transmission
#define CWT0OVFS 2929	// number of timer0 overflows per dit --> T0OVS = (ditlength/s * 1MHz/2048)

#define MORSEBYTES 8

typedef enum{
	TX_FREQ_LO,
	TX_FREQ_HI
} tTxFreq;

typedef enum{
	MEPT_TX_START,
	MEPT_TX_PROGRESS,
	MEPT_TX_DELAY
} tMeptState;

// timer0 overflow counter
volatile uint16_t t0ovfcnt;

// counters for keying functions
volatile uint8_t bitcnt;
volatile uint8_t bytecnt;
volatile uint8_t unitcnt;

volatile tTxFreq nextfreq;
volatile uint8_t txcomplete;

/* AD9850 frequency tuning words
 * two frequencies are defined for CCW (FSK-CW)
 * Note: to keep this firmware simple, these config
 * words already are in the correct bit order (LSB first)
 * use ad9850.py to calculate these words */
const uint8_t ad9850flo[5] = {0xc3, 0xe2, 0x23, 0x28, 0x0};
const uint8_t ad9850fhi[5] = {0x76, 0x12, 0x23, 0x28, 0x0};

// 1 unit = 1 bit
// use morsepack.py to generate these bytes
const uint8_t call[8] = {
	0x1d,0x45,0xd4,0x5d,
	0xdd,0xc7,0x5d,0xc4
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
	if(bytecnt < MORSEBYTES)
	{
		if(bitcnt < 7)
		{
			if((call[bytecnt]>>(7-bitcnt)) & 1) nextfreq = TX_FREQ_HI;
			else nextfreq = TX_FREQ_LO;
			bitcnt++;
		} else {
			if(call[bytecnt] & 1) nextfreq = TX_FREQ_HI;
			else nextfreq = TX_FREQ_LO;
			bitcnt = 0;
			bytecnt++;
		}
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

	currfreq = TX_FREQ_HI;
	nextfreq = TX_FREQ_LO;

	ad9850_setfreq(ad9850flo);

	sei();

	while(1)
	{
		switch(currstate)
		{
			case MEPT_TX_START:
				// reset all status variables
				txcomplete = 0;
				unitcnt = 0;
				bitcnt = 0;
				bytecnt = 0;

				currfreq = TX_FREQ_HI;
				nextfreq = TX_FREQ_LO;

				timer0_stop();
				// key up transmitter
				PORTD |= (1<<PD0);
				timer0_start();
				nextstate = MEPT_TX_PROGRESS;
				break;

			case MEPT_TX_PROGRESS:
				if(currfreq != nextfreq)
				{
					if(nextfreq == TX_FREQ_LO) ad9850_setfreq(ad9850flo);
					else if(nextfreq == TX_FREQ_HI) ad9850_setfreq(ad9850fhi);
					currfreq = nextfreq;
				}
				if(txcomplete){
					// key down transmitter
					PORTD &= ~(1<<PD0);
					nextstate = MEPT_TX_DELAY;
				} else nextstate = MEPT_TX_PROGRESS;
				break;

			case MEPT_TX_DELAY:
				if(unitcnt < 100) nextstate = MEPT_TX_DELAY;
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
