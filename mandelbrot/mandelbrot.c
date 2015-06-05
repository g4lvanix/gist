#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define IM_WIDTH 	1000
#define IM_HEIGHT 	1000

// Represents the Gaussian/Complex Plane
// the values of the series z[n+1] = z^2 + c
// where Re(c) = x and Im(c) = y coordinate
// the third index switches between Re(z) and Im(z)
double gaussianPlane[IM_WIDTH][IM_HEIGHT][2] = {{{0.0}}};

// holds the image frame data
// third index switches between the previous and the current frame
// which is needed for generating a nice looking greyscale image
uint8_t imagePlane[IM_WIDTH][IM_HEIGHT][2] = {{{255}}};

// calculate the real and imaginary components of the next element in the 
// Mandelbrot set z[n+1] = z[n]^2 + c;
// where c = x+jy;
// x,y image coordinates (used for image generation and array indexing)
// xmid,ymid center coordinates of image in Gaussian Plane
// xwidth,ywidth 
void calcNextIteration(uint16_t x, uint16_t y, double xmid, double ymid, double xwidth, double ywidth)
{
	double xscaled,yscaled;	// coordinates in the Gaussian Plane
	double z2real, z2imag;	// Re(z^2) and Im(z^2)
	
	// scale the input image coordinates according to the size of the image plane
	// shift positive integer indices to the middle of the range
	// to obtain complex numbers in all quadrants
	xscaled = (double)(x - (uint16_t)IM_WIDTH/2);
	yscaled = (double)(y - (uint16_t)IM_HEIGHT/2);
	
	// normalize the shifted integer coordinates to range [-width/2,width/2]
	xscaled = ((xwidth*xscaled) / (double)IM_WIDTH);
	yscaled = ((ywidth*yscaled) / (double)IM_HEIGHT);
	// center xrange,yrange around xmid,ymid in Gaussian Plane
	xscaled += xmid;
	yscaled -= ymid;
	
	// calculate Re(z^2) and Im(z^2)
	z2real = (gaussianPlane[x][y][0]*gaussianPlane[x][y][0]) - (gaussianPlane[x][y][1]*gaussianPlane[x][y][1]);
	z2imag = 2*gaussianPlane[x][y][0]*gaussianPlane[x][y][1];
	// calculate z[n+1] = z^2 + c
	gaussianPlane[x][y][0] = z2real + xscaled;
	gaussianPlane[x][y][1] = z2imag + yscaled;
}

void runEval(uint16_t iterations, uint16_t totalPics, uint16_t currPic, double xl, double yl)
{
	double mag2;	// mag2 = mag(z)^2 = Re(z)^2 + Im(z)^2 
	
	double stepWidth = 255.0/(double)totalPics;
	
	while(iterations--){
		for (uint16_t m = 0; m < (uint16_t)IM_WIDTH; m++){
			for (uint16_t n = 0; n < (uint16_t)IM_HEIGHT; n++){
				
				calcNextIteration(m,n,0.0,0.0,xl,yl);
				//calcNextIteration(m,n,-0.1011,0.9563,xl,yl);
			
				// calculate the square of the magnitude of the current element
				mag2 = (gaussianPlane[m][n][0]*gaussianPlane[m][n][0]) + (gaussianPlane[m][n][1]*gaussianPlane[m][n][1]);
			
				if (mag2 < 4.0){
					//imagePlane[m][n][0] = 255 - (uint16_t)(stepWidth*currPic);
					imagePlane[m][n][0] = 0;
				} else {
					//imagePlane[m][n][0] = imagePlane[m][n][1];
					imagePlane[m][n][0] = 255;
				}
			
				imagePlane[m][n][1] = imagePlane[m][n][0];
			}	
		}
	}
}

// generates a binary PNM image from the imagePlane data
void drawImage(char *filename)
{
	FILE *fp;
	
	printf("Writing image file\n");
	fp = fopen(filename,"wb");
	
	// print pgm file header
	fprintf(fp, "P5\n%u %u\n255\n", (uint16_t)IM_WIDTH, (uint16_t)IM_HEIGHT);
	
	// write lines of the pgm file	
	for (uint16_t m = 0; m < (uint16_t)IM_WIDTH; m++){
		for (uint16_t n = 0; n < (uint16_t)IM_HEIGHT; n++){
			fwrite(imagePlane[n][m], sizeof(imagePlane[n][m][0]),1,fp);
		}
	}
		
	fclose(fp);
	printf("Done\n");	
}

int main(int argc, char *argv[])
{
	uint16_t pics,it;
	uint16_t n = 0;
		
	double flowStep;
	
	char filename[] = "img/mandelbrot_0000_0000_0000_0000_0000.pnm";
	
	pics = 50;
	it = 10;
	
	flowStep = 4/pics;
	
		for(uint16_t i = 0; i < pics; i++)
		{
			runEval(it, pics, i, flowStep*i,flowStep*i);	
			sprintf(filename, "img/mandelbrot_%04u_%04u_%04u_%04u_%04u.pnm",(uint16_t)IM_WIDTH,(uint16_t)IM_HEIGHT,pics,it,n);
			drawImage(filename);
			n++;
			
			for (uint16_t m = 0; m < (uint16_t)IM_WIDTH; m++){
				for (uint16_t n = 0; n < (uint16_t)IM_HEIGHT; n++){
					imagePlane[m][n][0] = 255;
					imagePlane[m][n][1] = 255;
				}
			}
		}
	
	return 0;
}
