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

void manipulate(bitmap_pixel_hsv_t *pixels, int count, int level)
{

	for (int i = 0; i < count; i++)
	{
		bitmap_pixel_hsv_t *pixel = &pixels[i];
		pixel->v = max(min(pixel->v + (255 / 100 * level), 255), 0);
		printf("%i \n", pixel->v);
	}
}

int main(int argc, char *argv[])
{ /*
	if (argc < 4)
	{
		printf("Not enough Arguments!");
		return 1;
	}
	if (argc > 4)
	{
		printf("Too much Arguments!");
		return 1;
	}*/

	char *end;
	int level = (int)strtol(argv[3], end, 10);

	float alpha = 0;
	/*	if (level < -100 || level > 100)
	{
		printf("Level value is not between +100 and -100!");
		return 1;
	}*/

	//Read bitmap pixels:
	bitmap_error_t error;
	bitmap_pixel_rgb_t *pixels_1;
	bitmap_pixel_rgb_t *pixels_2;
	bitmap_pixel_rgb_t *pixels_output;
	int width_1, height_1;
	int width_2, height_2;
	int width_output, height_output;
	/*
	char *outputName = (char *)malloc(strlen(argv[1]) + 5 * sizeof(char));

	strncpy(outputName, argv[1], strlen(argv[1]) - 4);
	strcat(outputName, "_b");
	strcat(outputName, argv[3]);
	strcat(outputName, ".bmp");*/

	error = bitmapReadPixels("sails.bmp", (bitmap_pixel_t **)&pixels_1, &width_1, &height_1, BITMAP_COLOR_SPACE_RGB);
	assert(error == BITMAP_ERROR_SUCCESS); //!If False displays error message and aborts program
	printf("Read Bitmap successfully!");

	error = bitmapReadPixels("serrano.bmp", (bitmap_pixel_t **)&pixels_2, &width_2, &height_2, BITMAP_COLOR_SPACE_RGB);
	assert(error == BITMAP_ERROR_SUCCESS); //!If False displays error message and aborts program
	printf("Read Bitmap successfully!");

	width_output = min(width_1, width_2);
	printf("Width: %i | %i | %i", width_1, width_2, width_output);
	height_output = min(height_1, height_2);
	pixels_output = (bitmap_pixel_rgb_t *)malloc(height_output * width_output * sizeof(bitmap_pixel_rgb_t));

	//Manipulate bitmap pixels:

	for (int e = 0; e < height_output; e++)
	{
		for (int i = 0; i < width_output; i++)
		{
			bitmap_pixel_rgb_t *pixel_1 = &pixels_1[i + (width_output * e)];
			bitmap_pixel_rgb_t *pixel_2 = &pixels_2[i + (width_output * e)];
			bitmap_pixel_rgb_t *pixel_output = &pixels_output[i + (width_output * e)];

			pixel_output->r = round(alpha * pixel_1->r + (1 - alpha) * pixel_2->r);
			pixel_output->g = round(alpha * pixel_1->g + (1 - alpha) * pixel_2->g);
			pixel_output->b = round(alpha * pixel_1->b + (1 - alpha) * pixel_2->b);
			//printf("R: %i | %i | %i \n", pixel_1->r, pixel_2->r, pixel_output->r);
			//printf("%i | %i \n", pixel_2->r, pixel_output->r);

			if (i + 1 == width_1 || i + 1 == width_2)
				break;
		}
		if (e + 1 == height_1 || e + 1 == height_2)
			break;
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
