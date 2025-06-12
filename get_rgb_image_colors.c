#include "header.h"

void get_rgb_image_colors(
   int *reference_rgb_image_arr,
   int width,
   int height,
   int **prgb_image_colors_arr,
   int *prgb_image_colors_nbr
)

{

   int *rgb_image_colors_arr;
   int rgb_image_colors_nbr;
   int i, j, pixel, cind;
   int current_rgb[3];
   int k;
   int found;

   rgb_image_colors_arr = NULL;
   rgb_image_colors_nbr = 0;

   for ( i= 0 ; i< height ; i++ ) {
      for ( j= 0 ; j< width ; j++ ) {
         pixel= i*width+j;

         // Extract the RGB color of the current pixel
         current_rgb[0] = reference_rgb_image_arr[3 * pixel + 0];
         current_rgb[1] = reference_rgb_image_arr[3 * pixel + 1];
         current_rgb[2] = reference_rgb_image_arr[3 * pixel + 2];

         // Check if this color has already been stored
         found = 0;
         for (k = 0; k < rgb_image_colors_nbr; k++) {
            if (rgb_image_colors_arr[k * 3 + 0] == current_rgb[0] &&
                  rgb_image_colors_arr[k * 3 + 1] == current_rgb[1] &&
                  rgb_image_colors_arr[k * 3 + 2] == current_rgb[2]) {
                  found = 1;
                  break;
            }
         }

         // If the color is new, add it to our list of unique colors
         if (!found) {
            if (rgb_image_colors_nbr == 0) {
               rgb_image_colors_arr= (int *)calloc((rgb_image_colors_nbr+1),3*sizeof(int));
            }
            else {
               rgb_image_colors_arr= (int *)realloc(rgb_image_colors_arr,(rgb_image_colors_nbr+1)*3*sizeof(int));
            }
            // Add the new unique color to the array
            rgb_image_colors_arr[rgb_image_colors_nbr * 3 + 0] = current_rgb[0];
            rgb_image_colors_arr[rgb_image_colors_nbr * 3 + 1] = current_rgb[1];
            rgb_image_colors_arr[rgb_image_colors_nbr * 3 + 2] = current_rgb[2];
            rgb_image_colors_nbr++;
         }
      }
   }

   (*prgb_image_colors_arr)= rgb_image_colors_arr;
   (*prgb_image_colors_nbr)= rgb_image_colors_nbr;
}
