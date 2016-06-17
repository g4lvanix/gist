#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define CHUNKSIZE 9600
#define MINS 1

float goertzel_mag(int numSamples, int TARGET_FREQUENCY, int SAMPLING_RATE, float* data)
{
	int     k,i;
  	float   floatnumSamples;
  	float   omega,sine,cosine,coeff,q0,q1,q2,magnitude,real,imag;

  	float   scalingFactor = numSamples / 2.0;

  	floatnumSamples = (float) numSamples;
	k = (int) (0.5 + ((floatnumSamples * TARGET_FREQUENCY) / SAMPLING_RATE));
  	omega = (2.0 * M_PI * k) / floatnumSamples;
  	sine = sin(omega);
  	cosine = cos(omega);
  	coeff = 2.0 * cosine;
  	q0=0;
  	q1=0;
  	q2=0;

	 for(i=0; i<numSamples; i++)
	 {
	 	q0 = coeff * q1 - q2 + data[i];
		q2 = q1;
	    q1 = q0;
	 }

	 // calculate the real and imaginary results
	 // scaling appropriately
	 real = (q1 - q2 * cosine) / scalingFactor;
	 imag = (q2 * sine) / scalingFactor;

	 magnitude = sqrt(real*real + imag*imag);	 
	 return magnitude;
}

int main(void)
{
	int16_t buf[CHUNKSIZE];
	float floatbuf[CHUNKSIZE];
	uint16_t outbuf[MINS*1200];
	int chunkcnt = 0;

	FILE *infile;
	FILE *outfile;
	infile = fopen("mono.raw","r");
	outfile = fopen("demod.raw","w");

	// read file in 50ms chunks --> 9600 samples
	while(chunkcnt < MINS*1200){
		fread(buf,sizeof(int16_t),CHUNKSIZE,infile);

		for(int i = 0; i < CHUNKSIZE; i++){
				floatbuf[i] = (float)buf[i]/65535.0;
		}

		outbuf[chunkcnt++] = (uint16_t)(goertzel_mag(CHUNKSIZE,77500,192000,floatbuf) * 65535.0);
	}

	fwrite(outbuf,sizeof(uint16_t),MINS*1200,outfile);

	fclose(infile);
	fclose(outfile);

	return 0;
}
