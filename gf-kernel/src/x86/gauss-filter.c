/*
 * Copyright(C) 2014 Matheus M. Queiroz <matheus.miranda.queiroz@gmail.com>, 
 *                   Pedro H. Penna <pedrohenriquepenna@gmail.com>
 * 
 * x86/gaussian-filter.c - gauss_filter() implementation.
 */

#include <global.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Gaussian filter.
 */
void gauss_filter(unsigned char *img, int imgsize, double *mask, int masksize)
{
	int i, j;
	int half;
	double pixel;
	int imgI, imgJ, maskI, maskJ;
	
	#define MASK(i, j) \
		mask[(i)*masksize + (j)]
	
	#define IMG(i, j) \
		img[(i)*imgsize + (j)]
	
	i = 0; j = 0;
	half = imgsize >> 1;
	
	#pragma omp parallel default(shared) private(imgI,imgJ,maskI,maskJ,pixel,i,j)
	{
		#pragma omp for
		for (imgI = 0; imgI < imgsize; imgI++)
		{			
			for (imgJ = 0; imgJ < imgsize; imgJ++)
			{
				pixel = 0.0;
				for (maskI = 0; maskI < masksize; maskI++)
				{	
					for (maskJ = 0; maskJ < masksize; maskJ++)
					{
						i = (imgI - half < 0) ? imgsize-1 - maskI : imgI - half;
						j = (imgJ - half < 0) ? imgsize-1 - maskJ : imgJ - half;

						pixel += IMG(i, j)*MASK(maskI, maskJ);
					}
				}
				   
				IMG(imgI, imgJ) = (pixel > 255) ? 255 : (int)pixel;
			}
		}
	}
}
