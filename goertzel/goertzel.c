#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define M_PI 3.1415926

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

  magnitude = sqrtf(real*real + imag*imag);
  return magnitude;
}

int main(void)
{
	int16_t buf[19200];
	float floatbuf[19200];
	int16_t outbuf[100];
	int chunkcnt = 0;

	FILE *infile;
	FILE *outfile;
	infile = fopen("in.raw","r");
	outfile = fopen("out.raw","w");

	// read file in 100ms chunks --> 19200 samples
	if(!feof(infile)){
		fread(buf,sizeof(int16_t),19200,infile);

		for(int i = 0; i < 19200;i++){
				floatbuf[i] = (float)(buf[i]/65535.0);
		}

		outbuf[chunkcnt++] = (int16_t)(goertzel_mag(19200,77500,192000,floatbuf) * 65535.0);
	}

	fwrite(outbuf,sizeof(int16_t),100,outfile);

	return 0;
}
