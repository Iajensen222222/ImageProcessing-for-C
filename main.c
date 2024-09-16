#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



#include "processing.h"
#include "imageio.h"

int main(int argc, char *argv[]){
  /*
  //testing
  image *test = initialize_image(3, 3, 1);
  
  for(int i = 0; i < test->ny; i++){
    for(int j = 0; j < test->nx; j++){
      test->data[i*test->nx+j] = i+j-1.;
    }
  }
  for(int i = 0; i < test->ny; i++){
    printf("[");
    for(int j = 0; j < test->nx; j++){
      printf(" %lf, ", test->data[i*test->nx + j]);
    }
    printf("]\n");
  }

  printf("\n");

  image *flip = initialize_image(test->ny, test->nx, test->nc);
  
  for(int i = 0; i < flip->ny; i++){
    for(int j = 0; j < flip->nx; j++){
      flip->data[(flip->nx)*(flip->ny) - (i*flip->ny + j + 1)] = test->data[i*test->ny + j];
    }
  }

  for(int i = 0; i < flip->ny; i++){
    printf("[");
    for(int j = 0; j < flip->nx; j++){
      printf(" %lf, ", flip->data[i*flip->nx + j]);
    }
    printf("]\n");
  }

  printf("\n");
  
  int pad = 1;

  image *big = pad_array(test, pad);
  
  
  for(int i = pad; i < big->ny; i++){
    for(int j = pad; j < big->nx; j++){
      big->data[i*big->nx+j] = 1.;
    }
  }
  
  
  for(int i = 0; i < big->ny; i++){
    printf("[");
    for(int j = 0; j < big->nx; j++){
      printf(" %lf, ", big->data[i*big->nx + j]);
    }
    printf("]\n");
  }

  printf("out = %lf\n", sum_filter(big, test, 2, 2, pad));
  */



  //start of function> Everything above should be crossed out
  
  if(argc < 4){
    printf("Usage:\n"    //0             1            2              3                 4
	   "    $ ./imageprocessing <proc_type> <input_image> <output_image> [strength (default=1)]\n");
    return 0;
  }



  if(strcmp(argv[1], "bw") == 0){
    //execute bw process
    image *im = read_image(argv[2]);
    image *bw = rgb_to_bw(im);
    normalize(bw, 0, 255);
    //image *im_bw = initialize_image(im->ny, im->nx, 1);
      
    write_image(argv[3], bw);
    

    printf("bw processing successful on image size %dx%d\n", bw->ny, bw->nx);
    printf("\n");
    printf("output written to: %s\n", argv[3]);

    //free up memory
    free(bw);
    bw = NULL;

    free(im);
    im = NULL;
    
    return 0;
  }
  
  if(strcmp(argv[1], "blur") == 0){
    //execute blur process
    image *im = read_image(argv[2]);
    image *bw = rgb_to_bw(im);
    image *out = blur(bw);
    
    //image *im_bw = initialize_image(im->ny, im->nx, 1);
      
    write_image(argv[3], out);

    printf("blur processing successful on image size %dx%d\n", out->ny, out->nx);
    printf("\n");
    printf("output written to: %s\n", argv[3]);

    //free up memory
    free(out);
    out = NULL;

    free(im);
    im = NULL;

    free(bw);
    bw = NULL;
    
    return 0;
  }
  
  if(strcmp(argv[1], "sharpen") == 0){
    //execute sharpen process
    image *im = read_image(argv[2]);
    image *bw = rgb_to_bw(im);

    int strength = 1;
    if(argc > 4){
      strength = atoi(argv[4]);
    }
    
    image *out = sharpen(bw, strength);
    
    //image *im_bw = initialize_image(im->ny, im->nx, 1);
      
    write_image(argv[3], out);

    printf("sharpen processing successful on image size %dx%d\n", out->ny, out->nx);
    printf("\n");
    printf("output written to: %s\n", argv[3]);

    //free memory
    free(out);
    out = NULL;

    free(im);
    im = NULL;

    free(bw);
    bw = NULL;
    
    return 0;
  }
  printf("processing type must be blur, sharpen, or bw\n");
  return 0;
}
