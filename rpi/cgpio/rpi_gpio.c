#include "rpi_gpio.h"

void gpio_conf_mode(volatile unsigned int* base, uint32_t pin, GPIOMode mode)
{
	uint32_t cr;
	volatile unsigned int* gpfsel;

	gpfsel = (volatile unsigned int*)(base + (pin/10));
	
	// read GPIO config register
	cr = *gpfsel;

	cr &= ~(7<<((pin%10)*3));
	cr |= ((uint32_t)mode<<((pin%10)*3));

	// write GPIO config register
	*gpfsel = cr;
}

void gpio_set(volatile unsigned int* base, uint32_t pin)
{
	*(base + (pin>>5) + 7) = (1<<(pin%32));
}

void gpio_clear(volatile unsigned int* base, uint32_t pin)
{
	*(base + (pin>>5) + 10) = (1<<(pin%32));
}

uint32_t gpio_write(volatile unsigned int* base, uint32_t pin, uint32_t level)
{
	if(level == 1) gpio_set(base,pin);
	else if(level == 0) gpio_clear(base,pin);
	else return -1;
	return 0;
}

uint32_t gpio_read(volatile unsigned int* base, uint32_t pin)
{
	volatile uint32_t temp;
	temp = *(base + (pin>>5) + 13);
	temp = temp>>(pin%32);
	temp &= 1;
	return temp;
}
