// Chester Huynh and Jennifer Lin
// 601.220, Fall 2018
// ppm_io.c
#include <stdlib.h>
#include <assert.h>
#include "ppmIO.h"

/* Read a PPM-formatted image from a file (assumes fp != NULL).
 * Returns the address of the heap-allocated Image struct it
 * creates and populates with the Image data.
 */

 Image * read_ppm(FILE *fp) {

    // gets P6 tag
   char p6[3];
   fscanf(fp, " %s", p6);
   int data = fgetc(fp); // read in '\n' at end of P6 tag

   // throw away all comment lines
   data = fgetc(fp);
   while (data == '#') {
     do {
       data = fgetc(fp);
     } while (data != '\n');
     data = fgetc(fp);
   }
   ungetc(data, fp);

   // store resolution of image
   int max_color_val;
   Image * img = (Image *) malloc(sizeof(Image));
   if (!img) {
     return NULL; //abort if malloc failed
   }
   fscanf(fp, " %d", &(img->cols)); // stores num of cols
   fscanf(fp, " %d", &(img->rows)); // stores num of rows
   fscanf(fp, " %d", &max_color_val); // store max rbg value
   data = fgetc(fp); // to get '\n'

   // read in all pixels into our array of pixels called data
   img->data = (Pixel *) malloc(sizeof(Pixel) * img->rows * img->cols);
   if (!(img->data)) {
     return NULL; //abort if malloc failed
   }
   int num_pixels_read = fread(img->data, sizeof(Pixel), img->rows * img->cols, fp);

   // check to see if the size of the array read in
   // corresponds to the resolution read in earlier
   if (num_pixels_read != img->rows * img->cols) {
     fprintf(stderr, "Uh oh. Pixel data failed to read properly!\n");
     return NULL;
   }

   return img;

 }


/* Write a PPM-formatted image to a file (assumes fp != NULL),
 * and return the number of pixels successfully written.
 */
int write_ppm(FILE *fp, const Image *im) {

  // write PPM file header, in the following format
  // P6
  // cols rows
  // 255
  fprintf(fp, "P6\n%d %d\n255\n", im->cols, im->rows);

  // now write the pixel array
  int num_pixels_written = fwrite(im->data, sizeof(Pixel), im->rows * im->cols, fp);

  if (num_pixels_written != im->rows * im->cols) {
    fprintf(stderr, "Uh oh. Pixel data failed to write properly!\n");
    return 0; // no pixels were written in file
  }

  return num_pixels_written;
}
