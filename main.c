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
	}
}

int main(int argc, char *argv[])
{
	//Check CMD Arguments
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

	//Convert Level Char[] to int
	char *end;
	int level = (int)strtol(argv[3], &end, 10);
	if (level < -100 || level > 100)
	{
		printf("Level value is not between +100 and -100!");
		return 1;
	}

	//Read bitmap pixels:
	bitmap_error_t error;
	bitmap_pixel_hsv_t *pixels;
	int width, height;

	char *outputName = (char *)malloc(strlen(argv[1]) + 5 * sizeof(char));

	//Produce output name
	strncpy(outputName, argv[1], strlen(argv[1]) - 4);
	strcat(outputName, "_b_");
	strcat(outputName, argv[3]);
	strcat(outputName, ".bmp");

	error = bitmapReadPixels((char *)argv[1], (bitmap_pixel_t **)&pixels, &width, &height, BITMAP_COLOR_SPACE_HSV);

	assert(error == BITMAP_ERROR_SUCCESS); //!If False displays error message and aborts program
	printf("Read Bitmap successfully!");
	//Manipulate bitmap pixels:
	manipulate(pixels, width * height, level);

	//Write bitmap pixels:
	bitmap_parameters_t parameters = {
		.bottomUp = BITMAP_BOOL_TRUE,
		.widthPx = width,
		.heightPx = height,
		.colorDepth = BITMAP_COLOR_DEPTH_24,
		.compression = BITMAP_COMPRESSION_NONE,
		.dibHeaderFormat = BITMAP_DIB_HEADER_INFO,
		.colorSpace = BITMAP_COLOR_SPACE_HSV,
	};
	error = bitmapWritePixels(outputName, BITMAP_BOOL_TRUE, &parameters, (bitmap_pixel_t *)pixels);
	assert(error == BITMAP_ERROR_SUCCESS);
	printf("Bitmap has been written!");
	//Free the pixel array:
	free(pixels);

	return 0;
}
