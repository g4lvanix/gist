#include <stdio.h>
#include <stdint.h>

#include "rpi.h"
#include "rpi_gpio.h"

int main(void)
{
	volatile unsigned int* baseaddr;

	baseaddr = map_peripheral();

	gpio_conf_mode(baseaddr, 47, GPIO_MODE_OUT);
	gpio_conf_mode(baseaddr, 20, GPIO_MODE_IN);

	while(1)
	{
		if(gpio_read(baseaddr, 20) == 1) gpio_write(baseaddr, 47, 1);
		else gpio_write(baseaddr, 47, 0);
	}

	unmap_peripheral();

	return 0;
}
