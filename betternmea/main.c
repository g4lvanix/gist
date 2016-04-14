#include <stdio.h>
#include <stdint.h>

#include "nmea.h"


int main(void)
{
	char gpgll[41] = "$GPGLL,4916.45,N,12311.12,W,225444,A,*1D";	

	int8_t n;
	n = ((nmea_ascii2hex(gpgll[38]))<<4) | nmea_ascii2hex(gpgll[39]);
	
	printf("%u\n",n);
	printf("Function returned: %u\n",nmea_checksum(gpgll));
	
	return 0;
}
