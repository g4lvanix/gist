#ifndef NMEA_H
#define NMEA_H

#include <stdint.h>

typedef struct{
	uint16_t utc_year;
	uint8_t utc_month;
	uint8_t utc_day;
	uint8_t utc_hour;
	uint8_t utc_min;
	uint8_t utc_sec;
	
	double lat;
	double lon;
	double altmsl;
	double hdop;
	double pdop;
	double vdop;
	double gskts;
	double gskph;
	double magvar;
	double tangletrue;
	
	uint8_t nsats;
	
	enum{
		INVALID,
		GPS,
		DGPS,
		PPS,
		RTK,
		FLOATRTK,
		ESTIMATED,
		MANUAL,
		SIMULATION
	} fixqual;

	enum{
		NOFIX,
		FIX2D,
		FIX3D
	} fixtype;
} GPSData;

uint8_t nmea_checksum(char* s);
int8_t nmea_ascii2hex(char c);
int16_t nmea_strfind(char* s, char c);

uint8_t parseGGA(GPSData* gpsd, char* s);
uint8_t parseGSA(GPSData* gpsd, char* s);
uint8_t parseGSV(GPSData* gpsd, char* s);
uint8_t parseGLL(GPSData* gpsd, char* s);
uint8_t parseRMC(GPSData* gpsd, char* s);
uint8_t parseVTG(GPSData* gpsd, char* s);

#endif // NMEA_H
