// Chester Huynh and Jennifer Lin
// 601.220, Fall 2018
// project.c
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "ppmIO.h"
#include "imageManip.h"
#define PI 3.14159265358979323846

int main(int argc, char* argv[]) {
  if (argc < 3) {
    fprintf(stderr, "No input and output filename.\n");
    return 1; // not enough filenames given
  }

  if (argc < 4) {
    fprintf(stderr, "No operation name.\n");
    return 4; // input and output files were given but no operation given
  }

  FILE *fp = fopen(argv[1], "rb");
  if (fp == NULL) {
    fprintf(stderr, "Cannot open image.\n");
    return 2; // cannot open Image
  }
  Image * img = read_ppm(fp);
  if (img == NULL) {
    fprintf(stderr, "Read ppm failed.\n");
    return 3; // read function failed beceause ppm was incorrectly formatted
  }

  if (strcmp(argv[3], "swap") == 0) {
    swap(img); // rgb values of image get cycled
  } else if (strcmp(argv[3], "grayscale") == 0) {
    grayscale(img); // grayscale image produced
  } else if (strcmp(argv[3], "contrast") == 0) {
    double c_factor;
    if (argc < 5) {
      fprintf(stderr, "No contrast factor given.\n");
      return 5; // insufficient arguments for the contrast function
    }
    sscanf(argv[4],"%lf",&c_factor); // desired contrast factor
    contrast(img, c_factor); // contrast changed by inputted factor
  } else if (strcmp(argv[3], "zoom_in") == 0) {
    zoom_in(img); // returns a zoomed in image
  } else if (strcmp(argv[3], "zoom_out") == 0) {
    zoom_out(img); // returns a zoomed out image
  } else if (strcmp(argv[3], "occlude") == 0) {
    /* upper left column, upper left row
    bottom right column, and bottom right row boundaries
    */
    int c_up, r_up, c_b, r_b;
    if (argc < 8) {
      fprintf(stderr, "Not enough bounds given.\n");
      return 5; // not enough bounds given for occlude
    }
    // read in boundaries for section of image to occlude
    sscanf(argv[4],"%d", &c_up);
    sscanf(argv[5],"%d", &r_up);
    sscanf(argv[6],"%d", &c_b);
    sscanf(argv[7],"%d", &r_b);
    if (c_up > c_b || r_up > r_b) {
      fprintf(stderr, "Occlude bounds out of range.\n");
      return 6; // the bottom limits are less than upper limits
    }
    if (c_up < 0 || r_up < 0 || c_b >= img->cols || r_b >= img->rows) {
      fprintf(stderr, "Occlude bounds out of range.\n");
      return 6; // boundaries are too small or too big for image
    }
    occlude(img, c_up, r_up, c_b, r_b); // occludes section of image
  } else if (strcmp(argv[3], "blur") == 0) {
    double sigma; // sigma value to generate filter matrix
    if (argc < 5) {
      fprintf(stderr, "No sigma value given.\n");
      return 5; // not enough arguments for blur
    }
    sscanf(argv[4], "%lf", &sigma); // read in a sigma value
    blur(img, sigma); // convolves image with generated filter
  } else {
    fprintf(stderr, "Invalid operation name.\n");
    return 4; // no valid operation was given
  }

  FILE* write_to = fopen(argv[2], "wb");
  if (write_to == NULL) {
    fprintf(stderr, "Cannot open output file.\n");
    return 7; // cannot open Image
  }

  // number of pixels written into target file
  int num_pixels_written = write_ppm(write_to, img);
  printf("%d pixels written to successfully!\n", num_pixels_written);

  free(img->data);
  free(img);
  fclose(fp);
  fclose(write_to);

  return 0;
}
