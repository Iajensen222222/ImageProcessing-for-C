#ifndef PROCESSING_H
#define PROCESSING_H

// #include "imageio.h"

typedef struct image
{
  int ny;
  int nx;
  int nc;
  double data[];
} image;

image *rgb_to_bw(image *im_color);

image *pad_array(const image *im, const int pad);

double sum_filter(image *im, image *kernel, int py, int px, int pad);

image *convolve(const image *im, const image *kernel);

void normalize(image *im, const int im_min, const int im_max);

image *blur(const image *im_in);

image *sharpen(const image *im, const double strength);

#endif
