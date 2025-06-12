#include "header.h"

/*
Define the comparison function to sort the colors
*/

static int get_cell_median_color_compare(
   const void *p1,
   const void *p2
)

{

   int var1;
   int var2;

   var1= *(int *)p1;
   var2= *(int *)p2;

   /*
   Sort w/r to increasing value
   */

   if ( var1 < var2 )
      return -1;
   if ( var1 > var2 )
      return +1;

   /*
   Sort w/r to decreasing value
   */

   /*
   if ( var1 > var2 )
      return -1;
   if ( var1 < var2 )
      return +1;
   */

   return 0;

}


void get_cell_median_color(
   int *rgb_image_arr,
   int width,
   int height,
   int cell_top_left_corner_i,
   int cell_top_left_corner_j,
   int cell_width,
   int cell_height,
   int rgb[3]
)

/*
cell_top_left_corner_i,cell_top_left_corner_j is the top left corner of the grid cell
*/

{

   int *var_arr;
   int cind;
   int r_idx; // Renamed from grid_i (loop variable for rows)
   int c_idx; // Renamed from grid_j (loop variable for columns)
   int i;
   int j;
   int pixel;
   int var;
   int var_arr_ind;
   int var_arr_ind2;
   int num_elements;

   if (cell_height <= 0 || cell_width <= 0) {
      rgb[0] = 0; rgb[1] = 0; rgb[2] = 0;
      return;
   }

   /*
   Process each color channel independently
   */

   for ( cind= 0 ; cind< 3 ; cind++ ) {

      num_elements = cell_height * cell_width;
      var_arr= (int *)calloc(num_elements,sizeof(int));
      if (var_arr == NULL) {
         error_handler((char *)"get_cell_median_color: Failed to allocate var_arr");
         // Set to a default color before returning to ensure rgb is initialized for this channel
         // and then for subsequent channels if the loop continues (though it might be better to set all and exit)
         rgb[0] = 0; rgb[1] = 0; rgb[2] = 0;
         // It might be better to return immediately if one channel fails, or ensure all rgb components are set.
         // For consistency with dominant_color, setting all and returning.
         return;
      }

      for ( r_idx= 0 ; r_idx< cell_height ; r_idx++ ) {
         i= cell_top_left_corner_i+r_idx;
         if ( !(i < height) )
            continue;
         for ( c_idx= 0 ; c_idx< cell_width ; c_idx++ ) {
            j= cell_top_left_corner_j+c_idx;
            if ( !(j < width) )
               continue;
            pixel= i*width+j;
            var= rgb_image_arr[3*pixel+cind];
            var_arr_ind= r_idx*cell_width+c_idx;
            var_arr[var_arr_ind]= var;
         } 
      }

      /*
      Sort var_arr w/r to increasing value
      */

      /*
      The old fashioned way
      */

      /*
      for ( var_arr_ind= 0 ;
            var_arr_ind< grid*grid ;
            var_arr_ind++ ) {
         for ( var_arr_ind2= var_arr_ind+1 ;
               var_arr_ind2< grid*grid ;
               var_arr_ind2++ ) {
            if ( var_arr[var_arr_ind2] < var_arr[var_arr_ind] ) {
               var= var_arr[var_arr_ind];
               var_arr[var_arr_ind]= var_arr[var_arr_ind2];
               var_arr[var_arr_ind2]= var;
            }
         }
      }
      */

      /*
      Using qsort
      */

      qsort(
         var_arr,
         num_elements,
         sizeof(int),
         get_cell_median_color_compare
      );

      /*
      Check that var_arr was properly sorted
      */

      for ( var_arr_ind= 0 ;
            var_arr_ind< num_elements-1 ;
            var_arr_ind++ ) {
         var_arr_ind2= var_arr_ind+1;
         if ( var_arr[var_arr_ind2] < var_arr[var_arr_ind] ) {
            error_handler((char *)"get_cell_median_color");
         }
      }

      /*
      Get the median
      */

      rgb[cind]= var_arr[num_elements/2];

      free(var_arr);
   }

}
