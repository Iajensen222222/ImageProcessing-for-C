#ifndef IMAGEIO_H
#define IMAGEIO_H

#include "processing.h"

image *initialize_image(int ny, int nx, int nc);

image *read_image(char *filename);

void write_image(char *filename, image *im);

#endif
