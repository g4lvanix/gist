#include <stdio.h>
#include <stdint.h>

int16_t cosLUT_1200HZ[36] = {
	32755, 32101, 30472, 27917, 24514, 20366, 15599, 10358, 4803, -898, 
	-6572, -12046, -17155, -21742, -25669, -28815, -31087, -32413, -32755, -32101, 
	-30472, -27917, -24514, -20366, -15599, -10358, -4803, 898, 6572, 12046, 
	17155, 21742, 25669, 28815, 31087, 32413, 
};

int16_t cosLUT_2200HZ[20] = {
	32755, 30874, 25971, 18526, 9267, -898, -10976, -19979, -27027, -31429, 
	-32755, -30874, -25971, -18526, -9267, 898, 10976, 19979, 27027, 31429, 
};

int main(int argc, char *argv[])
{
	FILE *f;
	int16_t buffer[22050] = {0};
	
	/*
	uint32_t message[10] = {
		0xCAFEBABE, 0xFFFEEFEF, 0xB00B1322,
		0x3FFF0000, 0x00000000, 0x00000000,
		0xFFF300F1, 0xF10BCDA0, 0x00000000,
		0xABCDEF01
	};
	*/
	
	uint8_t message = 0x7E;
	
	if (argc < 2)
	{
		printf("I take at least 2 arguments!\n");
		return 1;
	}
	
	f = fopen(argv[argc-1], "w");

	if (f == 0)
	{
		printf("File \'%s\' could not be opened!\n", argv[argc-1]);
		return 1;
	}

	uint16_t buffindex = 0;
	/*
	for (uint8_t i = 0; i < 8; i++)
	{
			for (uint8_t s = 0; s < 36; s++)
			{
				if ( (message>>i) & 0x01 )
				{
					buffer[buffindex+s] = cosLUT_2200HZ[s%20];
				}
				else
				{
					buffer[buffindex+s] = cosLUT_1200HZ[s];
				}
			}
			
			buffindex += 36;
	}
	*/

	for (uint16_t i = 0; i < 36; i++)
	{
		buffer[i] = cosLUT_1200HZ[i];
	}
	
	buffindex += 36;
	
	for (uint16_t i = 0; i < (6*36); i++)
	{
		buffer[buffindex+i] = cosLUT_2200HZ[i%20];
	}
	
	buffindex += 6*36;
	
	for (uint16_t i = 0; i < 36; i++)
	{
		buffer[buffindex+i] = cosLUT_1200HZ[i];
	}
	
	fwrite(buffer, sizeof(int16_t), 22050, f);
	
	fclose(f);
}
