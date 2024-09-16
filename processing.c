#include <stdlib.h>
#include <stdio.h>

#include "imageio.h"

//pass
image *rgb_to_bw(image *im_color){

  image *im = initialize_image(im_color->ny, im_color->nx, 1);

  //double *bw = malloc(sizeof(double)*(im->ny)*(im->nx));
  
  double R = 0.299;
  double G = 0.587;
  double B = 0.114;
  int count = 0;
  int pass = 0;

  for(int i = 0; i < im->ny; i++){
    for(int j = 0; j < im->nx; j++){
      im->data[count] = (R*im_color->data[pass])+(G*im_color->data[pass+1])+(B*im_color->data[pass+2]);
      count++;
      pass = pass + 3;
    }
  }
  
  return im;
}

//unknown
//working
image *pad_array(const image *im, const int pad){
  int ny = im->ny;
  int nx = im->nx;
  int nc = im->nc;
  int count = 0;

  /*
  double *data = (double *)malloc(sizeof(double)*(ny+2*pad)*(nx+2*pad)*(nc));

  //fill array with zeros
  for(int i = 0; i < ny+pad*2; i++){
    for(int j = 0; j < nx+pad*2; j++){
      data[count] = 0.0;
      count++;
    }
  }
  */

  image *im_out = initialize_image(ny+pad*2, nx+pad*2, nc);

  /*
  for(int i = 0; i < (ny+pad*2)*(nx+pad*2)*nc; i++){
    im_out->data[i] = data[i];
  }
  */
  
  //populate array
  count = 0;
  for(int i = pad; i < ny+pad; i++){
    for(int j = pad; j < nx+pad; j++){
      im_out->data[i*im_out->nx + j] = im->data[count];
      count++;
    }
  }

  /*
  free(data);
  data = NULL;
  */
  return im_out;
}

//will do inner filter algorithm
//working
double sum_filter(image *im, image *kernel, int py, int px, int pad){
  double out = 0.0;
  py = py - pad;
  px = px - pad;

  for(int i = 0; i < kernel->ny; i++){
    for(int j = 0; j < kernel->nx; j++){
      out += (kernel->data[i*(kernel->nx)+j])*(im->data[((i+py)*im->nx) + (j+px)]);
    }
  }

  
  return out;
}


//try
image *convolve(const image *im, const image *kernel){
  int pad = (int)((double)(kernel->ny - 1)/2);

  image *flip = initialize_image(kernel->ny, kernel->nx, kernel->nc);

  for(int i = 0; i < flip->ny; i++){
    for(int j = 0; j < flip->nx; j++){
      flip->data[(flip->nx)*(flip->ny) - (i*flip->ny + j + 1)] = kernel->data[i*kernel->ny + j];
    }
  }
  
  image *im_pad = pad_array(im, pad);
  image *im_out = initialize_image(im->ny, im->nx, im->nc);
  int count = 0;
  
  for(int i = pad; i < im_out->ny+pad; i++){
    for(int j = pad; j < im_out->nx+pad; j++){
      im_out->data[count] = sum_filter(im_pad, flip, i, j, pad);
      count++;
    }
  }

  free(im_pad);
  free(flip);

  im_pad = NULL;
  flip = NULL;
  
  return im_out;
}



//pass?
void normalize(image *im, const int im_min, const int im_max){
  /*
Python Code:
  im = np.array(im)

    for i in range(np.shape(im)[0]):
        for j in range(np.shape(im)[1]):
            if im_min > im[i,j]:
                im[i,j] = im_min
            if im_max < im[i,j]:
                im[i,j] = im_max
  */
  int count = 0;

  for(int i = 0; i < im->ny; i++){
    for(int j = 0; j < im->nx; j++){
      //do I need the k for loop?
      for(int k = 0; k < im->nc; k++){
	if(im->data[count] < (double)im_min){
	  im->data[count] = im_min;
	}
	if(im->data[count] > (double)im_max){
	  im->data[count] = im_max;
	}
	count++;
      }
    }
  }
  
}

//try
image *blur(const image *im_in){
  image *gausian = initialize_image(5, 5, 1);
  gausian->data[0] = 1;
  gausian->data[1] = 4;
  gausian->data[2] = 7;
  gausian->data[3] = 4;
  gausian->data[4] = 1;
  
  gausian->data[5] = 4;
  gausian->data[6] = 16;
  gausian->data[7] = 26;
  gausian->data[8] = 16;
  gausian->data[9] = 4;
  
  gausian->data[10] = 7;
  gausian->data[11] = 26;
  gausian->data[12] = 41;
  gausian->data[13] = 26;
  gausian->data[14] = 7;
  
  gausian->data[15] = 4;
  gausian->data[16] = 16;
  gausian->data[17] = 26;
  gausian->data[18] = 16;
  gausian->data[19] = 4;

  gausian->data[20] = 1;
  gausian->data[21] = 4;
  gausian->data[22] = 7;
  gausian->data[23] = 4;
  gausian->data[24] = 1;

  for(int i = 0; i < gausian->ny; i++){
    for(int j = 0; j < gausian->nx; j++){
      gausian->data[i*gausian->nx+j] = gausian->data[i*gausian->nx+j]/273;
    }
  }
  /*
  for(int i = 0; i < gausian->ny; i++){
    printf("[");
    for(int j = 0; j < gausian->nx; j++){
      printf(" %lf, ", gausian->data[i*gausian->nx + j]);
    }
    printf("]\n");
  }
  */

  image *im_out = convolve(im_in, gausian);

  normalize(im_out, 0, 255);


  free(gausian);
  gausian = NULL;
  
  return im_out;
}

//try
image *sharpen(const image *im, const double strength){

  image *laplacian = initialize_image(3, 3, 1);
  
  laplacian->data[0] = 0;
  laplacian->data[1] = -1;
  laplacian->data[2] = 0;
  
  laplacian->data[3] = -1;
  laplacian->data[4] = 4;  
  laplacian->data[5] = -1;
  
  laplacian->data[6] = 0;
  laplacian->data[7] = -1;
  laplacian->data[8] = 0;

  image *blr = blur(im);

  image *edge = convolve(blr, laplacian);

  image *im_out = initialize_image(im->ny, im->nx, 1);

  for(int i = 0; i < im_out->ny; i++){
    for(int j = 0; j < im_out->nx; j++){
      im_out->data[i*im->nx+j] = im->data[i*im->nx + j] + strength*edge->data[i*im->nx + j];
    }
  }

  normalize(im_out, 0, 255);

  //free up memory
  free(laplacian);
  free(blr);
  free(edge);

  laplacian = NULL;
  blr = NULL;
  edge = NULL;
  
  return im_out;
}
