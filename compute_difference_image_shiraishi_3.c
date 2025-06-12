#include "header.h"

void compute_difference_image_shiraishi_3(
 int *rgb_image_arr,
 int width,
 int height,
 int rgb_color[3],
 double **pdifference_image_arr
)

/*
difference_image_arr is a grayscale image with values between 0.0 and 1.0
If difference_image_arr[pixel] = 1.0,
pixel has same color as input color rgb_color
It's a bit counter-intuitive as we are talking about a difference image
*/

/*
This assumes the reference image has a very limited of colors,
hece, we can have a binary difference image...
pixel is white (has value 1.) if same color as input color
pixel is black (has value 0.) otherwise
*/

{ 

 double *difference_image_arr;
 int i;
 int j;
 int pixel;
 int diff2_int;
 double diff_dbl;
 int cind;
 double max_diff_dbl;
 double min_diff_dbl;
 double normalized_diff_dbl;

 /*
 Allocate memory for the difference image
 */

 difference_image_arr= (double *)calloc(width*height,sizeof(double));

 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;

       diff2_int= 0;
       for ( cind= 0 ; cind< 3 ; cind++ ) {
          diff2_int+= (rgb_image_arr[3*pixel+cind]-rgb_color[cind])*
                      (rgb_image_arr[3*pixel+cind]-rgb_color[cind]);
       }
       if (diff2_int == 0) {
          difference_image_arr[pixel]= 1.;
       }
    }
 }

 (*pdifference_image_arr)= difference_image_arr;

}
