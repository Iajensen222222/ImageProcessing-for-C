#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "processing.h"

image *initialize_image(int ny, int nx, int nc){
  image *im = malloc(sizeof(*im) + sizeof(double)*ny*nx*nc);
  im->ny=ny;
  im->nx=nx;
  im->nc=nc;
  
  for(int i = 0; i < ny*nx*nc; i++){
    im->data[i] = 0.0;
  }
  
  return im;
}

image *read_image(char *filename){
  //read in
  int nx, ny, nc;
  
  unsigned char *img_in = stbi_load(filename, &nx, &ny, &nc, 0);

  //initialize
  image *im = initialize_image(ny, nx, nc);

  //populate
  for(int i = 0; i < ny*nx*nc; i++){
    im->data[i] = (double)img_in[i];
  }
  
  //free up memory
  stbi_image_free(img_in);
  img_in = NULL;
  
  return im;
}

void write_image(char *filename, image *im){
  
  unsigned char *img_out = (unsigned char *)malloc((unsigned long)(im->nx)*(im->ny)*(im->nc)*sizeof(unsigned char));
  
  for(int i = 0; i < (im->ny)*(im->nx)*(im->nc); i++){
    img_out[i] = (unsigned char)im->data[i];
  }
  
  stbi_write_jpg(filename, im->nx, im->ny, im->nc, img_out, 100);

  free(img_out);
  img_out = NULL;
}
