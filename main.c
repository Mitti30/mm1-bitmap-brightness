#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "bitmap.h"

double min(double a, double b)
{
	return (a < b) ? a : b;
}

double max(double a, double b)
{
	return (a > b) ? a : b;
}

int main(int argc, char *argv[])
{
	//Check the CMD Arguments
	if (argc < 4)
	{
		printf("Not enough Arguments!");
		return 1;
	}
	if (argc > 4)
	{
		printf("Too much Arguments!");
		return 1;
	}

	//Convert char[] factor to double
	char *end;
	double alpha = strtod(argv[3], &end);
	;
	if (alpha < 0 || alpha > 1)
	{
		printf("Alphablending factor is not between 0 and 1!");
		return 1;
	}

	//Read bitmap pixels:
	bitmap_error_t error;
	bitmap_pixel_rgb_t *pixels_1;
	bitmap_pixel_rgb_t *pixels_2;
	bitmap_pixel_rgb_t *pixels_output;
	int width_1, height_1;
	int width_2, height_2;
	int width_output, height_output;

	error = bitmapReadPixels(argv[1], (bitmap_pixel_t **)&pixels_1, &width_1, &height_1, BITMAP_COLOR_SPACE_RGB);
	assert(error == BITMAP_ERROR_SUCCESS); //!If False displays error message and aborts program
	printf("Read Bitmap successfully!");

	error = bitmapReadPixels(argv[2], (bitmap_pixel_t **)&pixels_2, &width_2, &height_2, BITMAP_COLOR_SPACE_RGB);
	assert(error == BITMAP_ERROR_SUCCESS); //!If False displays error message and aborts program
	printf("Read Bitmap successfully!");

	//Image gets cropped to the resolution of the smaller input image
	width_output = min(width_1, width_2);
	height_output = min(height_1, height_2);
	pixels_output = (bitmap_pixel_rgb_t *)malloc(height_output * width_output * sizeof(bitmap_pixel_rgb_t));

	//Manipulate bitmap pixels:

	for (int e = 0; e < height_output; e++)
	{
		for (int i = 0; i < width_output; i++)
		{
			bitmap_pixel_rgb_t *pixel_1 = &pixels_1[i + (width_1 * e)];
			bitmap_pixel_rgb_t *pixel_2 = &pixels_2[i + (width_2 * e)];
			bitmap_pixel_rgb_t *pixel_output = &pixels_output[i + (width_output * e)];

			pixel_output->r = round(alpha * pixel_1->r + (1 - alpha) * pixel_2->r);
			pixel_output->g = round(alpha * pixel_1->g + (1 - alpha) * pixel_2->g);
			pixel_output->b = round(alpha * pixel_1->b + (1 - alpha) * pixel_2->b);
		}
	}

	//Write bitmap pixels:
	bitmap_parameters_t parameters = {
		.bottomUp = BITMAP_BOOL_TRUE,
		.widthPx = width_output,
		.heightPx = height_output,
		.colorDepth = BITMAP_COLOR_DEPTH_24,
		.compression = BITMAP_COMPRESSION_NONE,
		.dibHeaderFormat = BITMAP_DIB_HEADER_INFO,
		.colorSpace = BITMAP_COLOR_SPACE_RGB,
	};
	error = bitmapWritePixels("alpha.bmp", BITMAP_BOOL_TRUE, &parameters, (bitmap_pixel_t *)pixels_output);
	assert(error == BITMAP_ERROR_SUCCESS);
	printf("Bitmap has been written!");
	//Free the pixel array:
	free(pixels_1);
	free(pixels_2);
	free(pixels_output);

	return 0;
}
