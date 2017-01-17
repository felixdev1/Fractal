/* 
1) Increasing the number of iterations decreases the speed linearly (as expected), e.g. 3200 iterations takes 10 times longer than 320.
 Also increasing the size has a quadratic impact on performance, this is expected as my algorithm always uses a square bounding box. 
 A combination of changing the two would invoke a multiplicative impact according the previous rules, e.g. tripling the iterations invokes a 3 times performance decrease, doubling the size invokes a 4 times performance decrease: together a 12 times performance decrease.
 
2) Given a large enough size, there would not be enough space for the output due to the sheer amount of pixels.
 The computational time takes over a few minutes if you produce a higher resolution or want a more accurate representation of the regions near singularities within the fractal.

*/

#include <stdio.h>
#include <stdlib.h>

#define SIZE 512

/* prototype declaration of plotting function, defined below */
void plot(int image[SIZE][SIZE], FILE* img, int max_iterations);


int main(int argc, char *argv[])
{
	
	/* num_iter[][] serves the same purpose as the variable image[][] in the skeleton code. 
	using it in the for loops is equivalent to counting the iterations */
	
	int num_iter[SIZE][SIZE]; 
	FILE *img;
	
	/* variable declarations as needed */

	img=fopen("julia_script.ppm","w");
	fprintf(img, "P6\n%d %d 255\n", SIZE, SIZE);
	
	/* loop to calculate fractal set */	
	/* pixelratio allows the user to choose the ratio of pixel mapping, i.e. setting pixelratio to 1 gives 1 to 1 pixel mapping.
	   a higher pixel mapping ratio will effectively zoom out out of the fractal, allowing the user to see the surrounding region. */
	
	int i, j; 
	int max_iterations = atoi(argv[1]); 
	int pixelratio=atoi(argv[4]);
	const double cre = atof(argv[2]); const double cim = atof(argv[3]);
	double z[2] = {0,0}; double zout[2] = {0,0};
	
	for(i = 0; i < SIZE; i++)
	{
		for(j = 0; j < SIZE; j++)
		{
			z[0] =  (i*((pixelratio*2.0)/(double)SIZE))-pixelratio;
			z[1] =  (j*((pixelratio*2.0)/(double)SIZE))-pixelratio;			
			
			for(num_iter[i][j] = 0; num_iter[i][j] < max_iterations; num_iter[i][j]++)
			{
				/* Complex computation */
				zout[0] = ((z[0]*z[0])-(z[1]*z[1])) + cre; //real part
				zout[1] = ((z[0]*z[1])+(z[1]*z[0])) + cim; //imaginary part
				
				/* Update values of z */
				z[0] = zout[0];
				z[1] = zout[1];
				
				if (((z[0]*z[0])+(z[1]*z[1])) > 4)
				{
					num_iter[i][j]++;
					break;
				}
			}
		}
	}	

	/* create image */
	
	plot(num_iter, img, max_iterations);
	fclose(img);
}

/* plotting function */

void plot(int image[SIZE][SIZE], FILE* img, int max_iterations)
{
	int i,j;
	unsigned char line[3*SIZE];
	for(i=0;i<SIZE;i++)
	{
		for(j=0;j<SIZE;j++)
		{
			if (image[i][j]<=63)
			{
				line[3*j]=255;
				line[3*j+1]=line[3*j+2]=255-4*image[i][j];
			}
			else
			{
				line[3*j]=255;
				line[3*j+1]=image[i][j]-63;
				line[3*j+2]=0;
			}
			if (image[i][j]==max_iterations) line[3*j]=line[3*j+1]=line[3*j+2]=255;
		}
		fwrite(line,1,3*SIZE,img);
	}
}
