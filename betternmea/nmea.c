#include "nmea.h"

uint8_t nmea_checksum(char* s)
{
	uint8_t k = 0;
	uint8_t csrecv = 0;
	if(*s == '$') s++;
	while(*s != '*') k ^= *s++;
	s++;
	csrecv = (nmea_ascii2hex(*s++)<<4);
	csrecv |= nmea_ascii2hex(*s);
	return k == csrecv ? 1:0;
}

int8_t nmea_ascii2hex(char c)
{
	if( (c >= '0') && (c <= '9') ) return (c-48);
	else if((c == 'A') || (c == 'a')) return 0x0A;
	else if((c == 'B') || (c == 'b')) return 0x0B;
	else if((c == 'C') || (c == 'c')) return 0x0C;
	else if((c == 'D') || (c == 'd')) return 0x0D;
	else if((c == 'E') || (c == 'e')) return 0x0E;
	else if((c == 'F') || (c == 'f')) return 0x0F;
	else return -1;
}

int16_t nmea_strfind(char* s,char c)
{
	int16_t i = 0;
	while(*(s+i))
	{
		if(*(s+i) == c) return i;
		i++;
	}
	return -1;
}

double nmea_str2double(char* s)
{
	
}

uint8_t parseGGA(GPSData* gpsd, char* s)
{
	if(!nmea_checksum(s)) return 0;



	return 1;		
}
