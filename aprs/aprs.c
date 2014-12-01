#include <stdio.h>
#include <stdint.h>

int16_t sineLUT_1200HZ[18] = {
	0, 11207, 21062, 28377, 32269, 32269, 28377, 21062, 11207, 0, 
	-11207, -21062, -28377, -32269, -32269, -28377, -21062, -11207, 
};

int16_t sineLUT_2200HZ[10] = {
	0, 19260, 31163, 31163, 19260, 0, -19260, -31163, -31163, -19260, 
};

int main(void)
{

	FILE *f;
	
	f = fopen("audio.raw", "w");

	int16_t buffer[22050] = {0};
	
	for(uint16_t i = 0; i < 18*10; i++)
	{
		buffer[i] = sineLUT_1200HZ[i%18];
	}
	
	for(uint16_t i = 0; i < 10*10; i++)
	{
		buffer[i+180] = sineLUT_2200HZ[i%10];
	}
	
	fwrite(buffer, sizeof(int16_t), 22050, f);
	
	fclose(f);
}
