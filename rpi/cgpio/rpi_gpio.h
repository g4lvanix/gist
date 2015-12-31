#ifndef RPI_GPIO_H
#define RPI_GPIO_H

#include <stdint.h>

typedef enum{
	GPIO_MODE_IN,
	GPIO_MODE_OUT,
	GPIO_MODE_AF5,
	GPIO_MODE_AF4,
	GPIO_MODE_AF0,
	GPIO_MODE_AF1,
	GPIO_MODE_AF2,
	GPIO_MODE_AF3
} GPIOMode;


void gpio_conf_mode(volatile unsigned int* base, uint32_t pin, GPIOMode mode);
void gpio_set(volatile unsigned int* base, uint32_t pin);
void gpio_clear(volatile unsigned int* base, uint32_t pin);
uint32_t gpio_write(volatile unsigned int* base, uint32_t pin, uint32_t level);
uint32_t gpio_read(volatile unsigned int* base, uint32_t pin);


#endif /* RPI_GPIO_H  */
