// Chester Huynh and Jennifer Lin
// 601.220, Fall 2018
// imageManip.h
#ifndef IMAGEMANIP_H
#define IMAGEMANIP_H

#include <stdlib.h>
#include <math.h>
#include "ppmIO.h"
#define PI 3.14159265358979323846

/* Swaps the color channels of the image.
*/
void swap(Image *img);

/* Apply greyscale transformation to the image.
*/
void grayscale(Image * img);

/* Helper function to apply the contrast factor to a pixel rbg value.
   Returns the new contrast-applied unsigned char.
*/
unsigned char applyContrast(unsigned char char_value, double c_factor);

/* Adjusting the contrast of the image.
*/
void contrast(Image * img, double c_factor);

/* Apply zoom in to the image.
*/
void zoom_in(Image *img);

/* Helper function to find the average rbg values of four pixels.
   Returns the new pixel with the averaged channel values.
*/
Pixel * average(Pixel p1, Pixel p2, Pixel p3, Pixel p4);

/* Apply zoom out to the image.
*/
void zoom_out(Image *img);

/* Occlude a section of the image defined by given topleft and bottomright pixel.
*/
void occlude(Image *img, int c_up, int r_up, int c_b, int r_b);

/* Helper function to square a value.
   Return value squared.
*/
double sq(double val);

/* Helper function to generate the gaussian filter matrix.
   Returns a double pointer to the nxn matrix.
*/
double** gen_filter_matrix(int dim, int mid, double sigma);

/* Helper function to apply the gaussian filter matrix to a specified pixel.
   Return the pixel (the normalized sum).
*/
Pixel * filter_pixel(Image *img, int index, int mid, int dim, double** matrix);

/* Blur the image by applying a sigma-defined gaussian matrix to
   all pixels in a image.
*/
void blur(Image *img, double sigma);


#endif
