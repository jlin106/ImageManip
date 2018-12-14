// Chester Huynh and Jennifer Lin
// 601.220, Fall 2018
// imageManip.c
#include <stdlib.h>
#include <math.h>
#include "imageManip.h"
#include "ppmIO.h"
#define PI 3.14159265358979323846

/* Swaps the color channels of the image.
*/
void swap(Image * img){
  // determine the total number of pixels of the image
  int num_pixels = img->rows * img->cols;

  for (int i = 0; i < num_pixels; i++) {
    unsigned char red = (img->data)[i].r; // temp variable to hold red value
    (img->data)[i].r = (img->data)[i].g; // swap red -> green
    (img->data)[i].g = (img->data)[i].b; // swap green -> blue
    (img->data)[i].b = red;  // swap blue -> red
  }
}

/* Apply greyscale transformation to the image.
*/
void grayscale(Image * img){
  // determine total number of pixels of the image
  int num_pixels = img->rows * img->cols;

  for (int i = 0; i < num_pixels; i++) {
    // calculate grayscale value based on rgb values for a pixel
    int red = (int) (img->data)[i].r;
    int green = (int) (img->data)[i].g;
    int blue = (int) (img->data)[i].b;
    double intensity = 0.30*red + 0.59*green + 0.11*blue;

    //cast value back to unsigned char
    unsigned char g_value = (unsigned char) intensity;

    // assign value to each rbg channel
    (img->data)[i].r = (img->data)[i].g = (img->data)[i].b = g_value;
  }
}

/* Helper function to apply the contrast factor to a pixel rbg value.
   Returns the new contrast-applied unsigned char.
*/
unsigned char applyContrast(unsigned char char_value, double c_factor) {
  // convert pixel rbg value to a double evenly spanning [-0.5, 0.5]
  // and apply contrast factor
  double d_value = (double) char_value;
  d_value = ((d_value / 255) - 0.5) * c_factor;

  // convert to a range [0, 255] in order to use saturating math
  // when converting back to unsigned char
  d_value = ((d_value + 0.5) * 255);
  unsigned char u_char;
  if (d_value > 255) {
    u_char = (unsigned char) 255;
  } else if (d_value < 0) {
    u_char = (unsigned char) 0;
  } else {
    u_char = (unsigned char) d_value;
  }

  // return the new contrast-applied unsigned char
  return u_char;
}

/* Adjusting the contrast of the image.
*/
void contrast(Image * img, double c_factor) {
  // determine total number of pixels of the image
  int num_pixels = img->rows * img->cols;

  // separately apply the helper function to each rbg value, for each pixel
  for (int i = 0; i < num_pixels; i++) {
    (img->data)[i].r = applyContrast(((img->data)[i].r), c_factor);
    (img->data)[i].g = applyContrast(((img->data)[i].g), c_factor);
    (img->data)[i].b = applyContrast(((img->data)[i].b), c_factor);
  }
}

/* Apply zoom in to the image.
*/
void zoom_in(Image *img) {
  // malloc an image with double the number of original rows and cols
  Image * temp = (Image *) malloc(sizeof(Image));
  if (!temp) {
    return; //abort if malloc failed
  }
  temp->cols = 2 * img->cols;
  temp->rows = 2 * img->rows;
  temp->data = (Pixel *) malloc(sizeof(Pixel) * temp->rows * temp->cols);
  if (!(temp->data)) {
    return; //abort if malloc failed
  }

  // parsing through each pixel of the original image
  for (int r = 0; r < img->rows; r++) {
    for (int c = 0; c < img->cols; c++) {
      // indexes of the new image that will hold the same pixel value
      int n1 = 2*c + (r*2*temp->cols);
      int n2 = 2*c + 1 + (r*2*temp->cols);
      int n3 = 2*c + temp->cols + (r*2*temp->cols);
      int n4 = 2*c + temp->cols + 1 + (r*2*temp->cols);

      // set the pixel value at the indexes (stated above) on the new image
      // to the current pixel (based on for loop) of the original image
      (temp->data)[n1].r = (temp->data)[n2].r = (temp->data)[n3].r
        = (temp->data)[n4].r = (img->data)[c+(r*img->cols)].r; // r value
      (temp->data)[n1].g = (temp->data)[n2].g = (temp->data)[n3].g
        = (temp->data)[n4].g = (img->data)[c+(r*img->cols)].g; // g value
      (temp->data)[n1].b = (temp->data)[n2].b = (temp->data)[n3].b
        = (temp->data)[n4].b = (img->data)[c+(r*img->cols)].b; // b value
    }
  }

  // set the value of original image pointer to value of new image pointer
  img->data = (Pixel *) realloc(img->data, sizeof(Pixel) * temp->rows * temp->cols);
  if (!(img->data)) {
    return; //abort if malloc failed
  }
  img->rows = temp->rows;
  img->cols = temp->cols;
  for (int i = 0; i < temp->rows * temp->cols; i++) {
    (img->data)[i].r = (temp->data)[i].r;
    (img->data)[i].g = (temp->data)[i].g;
    (img->data)[i].b = (temp->data)[i].b;
  }

  free(temp->data);
  free(temp);
}

/* Helper function to find the average rbg values of four pixels.
   Returns the new pixel with the averaged channel values.
*/
Pixel * average(Pixel p1, Pixel p2, Pixel p3, Pixel p4) {
  // malloc a Pixel that will be the average of the four pixels
  Pixel * p_avg = malloc(sizeof(Pixel));
  if (!p_avg) {
    return NULL; //abort if malloc failed
  }

  // calculate the average rbg values, set as new pixel rbg value
  p_avg->r = (p1.r + p2.r + p3.r + p4.r) / 4;
  p_avg->g = (p1.g + p2.g + p3.g + p4.g) / 4;
  p_avg->b = (p1.b + p2.b + p3.b + p4.b) / 4;

  // return the new pixel
  return p_avg;
}

/* Apply zoom out to the image.
*/
void zoom_out(Image *img) {
  // malloc an image with half the number of original rows and cols
  Image * temp = (Image *) malloc(sizeof(Image));
  if (!temp) {
    return; //abort if malloc failed
  }
  temp->cols = img->cols / 2;
  temp->rows = img->rows / 2;
  temp->data = (Pixel *) malloc(sizeof(Pixel) * temp->rows * temp->cols);
  if (!(temp->data)) {
    return; //abort if malloc failed
  }

  // parsing through each pixel of the NEW image
  for (int r = 0; r < temp->rows; r++) {
    for (int c = 0; c < temp->cols; c++) {
      // indexes of the pixels in the original image that will be
      // averaged together
      int n1 = 2*c + (r*4*temp->cols);
      int n2 = 2*c + 1 + (r*4*temp->cols);
      int n3 = 2*c + 2*temp->cols + (r*4*temp->cols);
      int n4 = 2*c + 2*temp->cols + 1 + (r*4*temp->cols);

      // use the helper function to find the average of the pixels defined
      // by the index values above
      Pixel * p_avg = average((img->data)[n1], (img->data)[n2],
                        (img->data)[n3], (img->data)[n4]);

      // set the new image pixel value to the calculated average
      (temp->data)[c + (r*temp->cols)].r = p_avg->r;
      (temp->data)[c + (r*temp->cols)].g = p_avg->g;
      (temp->data)[c + (r*temp->cols)].b = p_avg->b;
      free(p_avg);
    }
  }

  // set the value of original image pointer to value of new image pointer
  img->data = (Pixel *) realloc(img->data, sizeof(Pixel) * temp->rows * temp->cols);
  if (!(img->data)) {
    return; //abort if malloc failed
  }
  img->rows = temp->rows;
  img->cols = temp->cols;
  for (int i = 0; i < temp->rows * temp->cols; i++) {
    (img->data)[i].r = (temp->data)[i].r;
    (img->data)[i].g = (temp->data)[i].g;
    (img->data)[i].b = (temp->data)[i].b;
  }

  free(temp->data);
  free(temp);
}

/* Occlude a section of the image defined by given topleft and bottomright pixel.
*/
void occlude(Image *img, int c_up, int r_up, int c_b, int r_b) {
  // set pixels bounded by the constraints to rbg value of 0
  for (int r = r_up; r < r_b + 1; r++) {
    for (int c = c_up; c < c_b + 1; c++) {
      (img->data)[(r*img->cols) + c].r = 0;
      (img->data)[(r*img->cols) + c].g = 0;
      (img->data)[(r*img->cols) + c].b = 0;
    }
  }
}

/* Helper function to square a value.
   Return value squared.
*/
double sq(double val) {
  return val*val;
}

/* Helper function to generate the gaussian filter matrix.
   Returns a double pointer to the nxn matrix.
*/
double** gen_filter_matrix(int dim, int mid, double sigma) {
  // malloc 2D matrix using pointers to arrays of pointers
  double **matrix = (double **)malloc(sizeof(double *) * dim);
  if (!matrix) {
    return NULL; //abort if malloc failed
  }
  for (int i = 0; i < dim; i++) {
    matrix[i] = (double *)malloc(sizeof(double) * dim);
    if (!matrix[i]) {
      return NULL; //abort if malloc failed
    }
  }

  // calculate the gaussian distribution values for the matrix,
  // calculating only the first quadrant and setting radial values equal
  for (int i = 0; i <= mid; i++) {
    for (int j = 0; j <= mid; j++) {
      double g = (1.0 / (2.0 * PI * sq(sigma))) * exp( -(sq(mid - i) +
        sq(mid - j)) / (2 * sq(sigma)));
      matrix[i][j] = matrix[dim-i-1][j] = matrix[i][dim-j-1] =
        matrix[dim-i-1][dim-j-1] = g;
    }
  }

  //return the matrix
  return matrix;
}

/* Helper function to apply the gaussian filter matrix to a specified pixel.
   Return the pixel (the normalized sum).
*/
Pixel * filter_pixel(Image *img, int index, int mid, int dim, double** matrix) {
  // malloc a Pixel, will be the normalized sum of pixel values
  Pixel * p_filtered = malloc(sizeof(Pixel));
  if (!p_filtered) {
    return NULL; //abort if malloc failed
  }

  // determine the row and column of the specific pixel in the image
  int r = index / img->cols;
  int c = index % img->cols;

  // determine the start and end row index for the gaussian matrix based
  // on the index of the given pixel
  int r_start = 0;
  if ((r-mid) < 0) {
    r_start = mid - r;
  }
  int r_end = dim - 1;
  if ((r+mid) > img->rows - 1) {
    r_end = (img->rows - 1 - r) + mid;
  }

  // determine the start and end column index for the gaussian matrix based
  // on the index of the given pixel
  int c_start = 0;
  if ((c-mid) < 0) {
    c_start = mid - c;
  }
  int c_end = dim - 1;
  if ((c+mid) > img->cols - 1) {
    c_end = (img->cols - 1 - c) + mid;
  }

  double w_red = 0;
  double w_green = 0;
  double w_blue = 0;
  double f_value = 0;

  // calculate the starting pixel for the original image based on
  // the start and end of the gaussian matrix
  int k = index - ((mid-r_start)*(img->cols)) - (mid-c_start);

  // parse through the index of the gaussian matrix and multiply with
  // underlying image pixel
  for (int r = r_start; r <= r_end; r++) {
    for (int c = c_start; c <= c_end; c++) {
      f_value += matrix[r][c];
      w_red += (matrix[r][c]*(double)(img->data)[k].r);
      w_green += (matrix[r][c]*(double)(img->data)[k].g);
      w_blue += (matrix[r][c]*(double)(img->data)[k].b);
      k++;
    }
    k = k - (c_end - c_start) + (img->cols) - 1;
  }

  // calculate the norm for each pixel rbg value
  double norm_r = w_red / f_value;
  double norm_g = w_green / f_value;
  double norm_b = w_blue / f_value;

  // set new pixel to the normalized sum values
  p_filtered->r = (unsigned char) norm_r;
  p_filtered->g = (unsigned char) norm_g;
  p_filtered->b = (unsigned char) norm_b;

  // return the new pixel
  return p_filtered;
}

/* Blur the image by applying a sigma-defined gaussian matrix to
   all pixels in a image.
*/
void blur(Image *img, double sigma) {
  // determine the total number of pixels of the image
  int num_pixels = img->rows * img->cols;

  // calculate the dimension and midpoint of the gaussian matrix based on sigma
  int dim = (int) (sigma * 10.0);
  if ((dim % 2) == 0) {
    dim += 1;
  }
  int mid = dim/2;

  // generate the filter matrix using helper function
  double **matrix = gen_filter_matrix(dim, mid, sigma);

  // apply the matrix to each pixel using helper function, set image pixel
  // value to the new calculated value.

  for (int i = 0; i < num_pixels; i++) {
    Pixel * ith_pixel = filter_pixel(img, i, mid, dim, matrix);
    (img->data)[i].r = ith_pixel->r;
    (img->data)[i].g = ith_pixel->g;
    (img->data)[i].b = ith_pixel->b;
    free(ith_pixel);
  }
  for (int i = 0; i < dim; i++) {
    free(matrix[i]);
  }
  free(matrix);
}
