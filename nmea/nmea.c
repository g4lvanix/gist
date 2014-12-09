#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

const uint16_t meanEarthRadius = 6371;

typedef struct GPSFixStruct
{
	uint8_t utc_hours;		// UTC time provided by fix 
	uint8_t utc_minutes;
	uint8_t utc_seconds;
	char status;			// receiver status, A = active/valid fix, V = invalid fix
	float lat;
	uint8_t lat_deg;		// latitude information, degrees
	float lat_moa;			// latitude information, minutes of arc
	char lat_hemi; 			// latitude hemisphere: N or S 
	float lon;
	uint8_t lon_deg;		// longitude information, degrees
	float lon_moa;			// longitude information, minutes of arc 
	char lon_hemi;			// longitude hemisphere: E or W
	float speed_kn;			// speed in knots
	float speed_kmh;		// speed in km/h
	float track;			// track made good in degrees
	uint8_t utc_day;		// UTC date provided by fix 
	uint8_t utc_month;
	uint8_t utc_year;
	float mag_dev_angle;	// magnetic deviation angle at fix position
	char mag_dev_direction;	// magnetic deviation direction, E or W
} GPSFix;

void slices(char *str, int start, int end, char *buf)
{
	for(int i = start; i < end; i++)
	{
		*buf = str[i];
		buf++;
	}
}

uint8_t parse_GPRMC(char *nmeaString, GPSFix *fix)
{
	uint8_t nmeaStringIndex = 0;
	uint8_t nmeaDelimCount = 0;
	uint8_t nmeaDelimIndex[11] = {0};
	
	uint8_t nmeaEOM = 0;
	uint8_t nmeaChecksumCalc, nmeaChecksumRecv = 0;
	
	char temp[8];
	
	while (nmeaString[nmeaStringIndex] != '\n')
	{
		// find the indices of the comma delimiters in the NMEA sentence
		if (nmeaString[nmeaStringIndex] == ',')
		{
			nmeaDelimIndex[nmeaDelimCount] = nmeaStringIndex;
			nmeaDelimCount++;
		}
		
		// find index of the * delimiter at the end of the NMEA sentence 
		// end of message (EOM)
		if (nmeaString[nmeaStringIndex] == '*') 
		{
			nmeaEOM = nmeaStringIndex; 
		}
		
		nmeaStringIndex++;
	}
	
	// calculate the checksum of the NMEA sentence
	for (nmeaStringIndex = 1; nmeaStringIndex < nmeaEOM; nmeaStringIndex++)
	{
		nmeaChecksumCalc ^= nmeaString[nmeaStringIndex];
	}
	
	// retrieve the checksum of the NMEA sentence calculated by the GPS
	temp[0] = nmeaString[nmeaEOM+1];
	temp[1] = nmeaString[nmeaEOM+2];
	nmeaChecksumRecv = (uint8_t)strtol(temp, NULL, 16);
	
	// check wether the calculated and the received checksums match
	if (nmeaChecksumCalc != nmeaChecksumRecv) return 1;
	
	// retrieve UTC time information
	memset(temp, 0, 8);
	
	slices(nmeaString, nmeaDelimIndex[0]+1, nmeaDelimIndex[0]+3, temp);
	fix->utc_hours = atoi(temp); 
	
	memset(temp, 0, 8);
	
	slices(nmeaString, nmeaDelimIndex[0]+3, nmeaDelimIndex[0]+5, temp);
	fix->utc_minutes = atoi(temp); 
	
	memset(temp, 0, 8);
	
	slices(nmeaString, nmeaDelimIndex[0]+5, nmeaDelimIndex[1], temp);
	fix->utc_seconds = atoi(temp); 
	
	fix->status = nmeaString[nmeaDelimIndex[1]+1];


	// retrieve latitude information
	memset(temp, 0, 8);

	slices(nmeaString, nmeaDelimIndex[2]+1, nmeaDelimIndex[2]+3, temp);
	fix->lat_deg = atoi(temp);
	fix->lat = (float)atof(temp);
	
	
	memset(temp, 0, 8);
	
	slices(nmeaString, nmeaDelimIndex[2]+3, nmeaDelimIndex[3], temp);
	fix->lat_moa = (float)atof(temp);
	fix->lat += (float)atof(temp)/60.0;
	
	fix->lat_hemi = nmeaString[nmeaDelimIndex[3]+1];
	
	// retrieve longitude information
	memset(temp, 0, 8);
	
	slices(nmeaString, nmeaDelimIndex[4]+1, nmeaDelimIndex[4]+4, temp);
	fix->lon_deg = atoi(temp);
	fix->lon = (float)atof(temp);
	
	memset(temp, 0, 8);
	
	slices(nmeaString, nmeaDelimIndex[4]+4, nmeaDelimIndex[5], temp);
	fix->lon_moa = (float)atof(temp);
	fix->lon += (float)atof(temp)/60.0;
	
	fix->lon_hemi = nmeaString[nmeaDelimIndex[5]+1];
	
	// retrieve speed in knots
	memset(temp, 0, 8);
	
	slices(nmeaString, nmeaDelimIndex[6]+1, nmeaDelimIndex[7], temp);
	fix->speed_kn = (float)atof(temp);
	fix->speed_kmh = fix->speed_kn * 1.852;
	
	// retrieve true course
	memset(temp, 0, 8);
	
	slices(nmeaString, nmeaDelimIndex[7]+1, nmeaDelimIndex[8], temp);
	fix->track = (float)atof(temp);
	
	// retrieve UTC date information
	memset(temp, 0, 8);
	
	slices(nmeaString, nmeaDelimIndex[8]+1, nmeaDelimIndex[8]+3, temp);
	fix->utc_day = atoi(temp); 
	
	memset(temp, 0, 8);
	
	slices(nmeaString, nmeaDelimIndex[8]+3, nmeaDelimIndex[8]+5, temp);
	fix->utc_month = atoi(temp); 
	
	memset(temp, 0, 8);
	
	slices(nmeaString, nmeaDelimIndex[8]+5, nmeaDelimIndex[9], temp);
	fix->utc_year = atoi(temp);
	
	// retrieve magnetic deviation angle and direction
	memset(temp, 0, 8);
	
	slices(nmeaString, nmeaDelimIndex[9]+1, nmeaDelimIndex[10], temp);
	fix->mag_dev_angle = (float)atof(temp);
	
	fix->mag_dev_direction = nmeaString[nmeaDelimIndex[10]+1];
	
	return 0;
}

void printFix(GPSFix* fix)
{
	printf("################### START #######################\n");
	
	printf("Fix UTC time: %02i:%02i:%02i\n", fix->utc_hours, fix->utc_minutes, fix->utc_seconds);
	printf("Fix UTC date: %02i-%02i-%02i\n", fix->utc_day, fix->utc_month, fix->utc_year);
	printf("Receiver status: %c ", fix->status);
	
	if (fix->status == 'A') printf("(valid fix)\n");
	else printf("(invalid fix)\n");
	
	printf("Latitude: %c %i° %f' %f\n", fix->lat_hemi, fix->lat_deg, fix->lat_moa, fix->lat);
	printf("Longitude: %c %i° %f' %f\n", fix->lon_hemi, fix->lon_deg, fix->lon_moa, fix->lon);
	printf("Speed: %f kn %f km/h\n", fix->speed_kn, fix->speed_kmh);
	printf("Course: %f\n", fix->track);
	printf("Magnetic deviation: %c %f\n\n", fix->mag_dev_direction, fix->mag_dev_angle);
	
	printf("#################### END ########################\n\n");
};

int main(int argc, char *argv[])
{
	GPSFix gpsFixData;

	if (argc < 2)
	{
		printf("I take at least 2 arguments!");
		return 0;
	}
	
	printf("%i %s %s", argc, argv[0], argv[1]);

	if (parse_GPRMC(argv[1], &gpsFixData))
	{
		printf("Invalid checksum!");
		return 0;
	}
	else
	{
		printFix(&gpsFixData);
	}
	
	// $GPRMC,133208,A,5045.4631,N,00848.3764,E,0.0,295.9,200914,0.6,E,A*19\r\n\0
	
	/*
	char *sentence[4] = 
	{
		"$GPRMC,220516,A,5133.82,N,00042.24,W,173.8,231.8,130694,004.2,W*70\r\n",
		"$GPRMC,131826,A,5045.4705,N,00848.3718,E,0.0,116.3,200914,0.6,E,A*12\r\n",
		"$GPRMC,132216,A,5045.4538,N,00848.4099,E,0.0,295.9,200914,0.6,E,A*1F\r\n",
		"$GPRMC,133208,A,5045.4631,N,00848.3764,E,0.0,295.9,200914,0.6,E,A*19\r\n"
	};
	
	for(int i = 0; i < 4; i++)
	{
		if (parse_GPRMC(sentence[i], &gpsFixData))
		{
			printf("Invalid checksum!\n");
		}
		else
		{
			printf("################### START #######################\n");
			printf("Decode of NMEA recommended minimum sentence C:\n");
			printf("%s\n", sentence[i]);
			printFix(&gpsFixData);
			printf("#################### END ########################\n");
		}
	}
	*/
	return 0;
}
