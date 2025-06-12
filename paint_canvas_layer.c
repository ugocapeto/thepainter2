#include "header.h"

/* Define a structure to randomize the order the grid cells are gonna be processed */
typedef struct {
   int i;
   int j;
} CellCoordinate;

/* Declare the global error flag */
extern int debug_flag_global;

void paint_canvas_layer(
 int *reference_rgb_image_arr,
 int width,
 int height,
 int *rgb_image_colors_arr,
 int rgb_image_colors_nbr,
 int cell_width,
 int cell_height,
 int offset_j,
 int offset_i,
 double opacity_strength,
 double bumpity_strength,
 double error_threshold,
 texture_struct *texture_arr,
 int texture_nbr,
 int texture_ind,
 int *canvas_rgb_image_arr,
 int *canvas_bumpity_image_arr
)

{

 brush_stroke_struct brush_stroke_data;
 int brush_stroke_nbr;
 int i;
 int j;
 int local_pixel_nbr;
 int local_width;
 int local_height;
 int *local_rgb_image_arr;
 int local_pixel;
 int local_i;
 int local_j;
 int pixel;
 int cind;
 int brush_stroke_color[3];
 int debug_flag= debug_flag_global;
 int err_flag;
 int local_origin_x;
 int local_origin_y;
 texture_struct texture_data;
 double diff_dbl;
 int reference_rgb[3];
 int canvas_rgb[3];
 int diff2;
 double xc;
 double yc;
 double theta;
 double rectw;
 double rectl;
 int *local_rgb_image_arr2;
 int cell_top_left_corner_i;
 int cell_top_left_corner_j;
 int test_flag= 0;
 int cell_nbr_i;
 int cell_nbr_j;
 int pixel_nbr_i;
 int pixel_nbr_j;
 CellCoordinate *cell_coords = NULL;
 int num_total_cells = 0;
 int cell_idx = 0;

 /*
 Initialize the number of brush strokes
 */

 brush_stroke_nbr= 0;

 /*
 Count the number of cells and populate their coordinates
 */

 for (i = offset_i; i < height; i += cell_height) {
    for (j = offset_j; j < width; j += cell_width) {
       num_total_cells++;
    }
 }

 cell_coords = (CellCoordinate *)malloc(num_total_cells * sizeof(CellCoordinate));

 cell_idx = 0;
 for (i = offset_i; i < height; i += cell_height) {
    for (j = offset_j; j < width; j += cell_width) {
       cell_coords[cell_idx].i = i;
       cell_coords[cell_idx].j = j;
       cell_idx++;
    }
 }

 /*
 Shuffle the cell_coords array
 */

 /*
 This implements the Fisher-Yates shuffle algorithm (also known as the Knuth shuffle),
 which is a correct and common way to generate a random permutation of a finite sequence.
 The algorithm works by iterating through the array from the last element down to the second.
 In each step, it swaps the current element with an element chosen randomly
 from the portion of the array from the first element up to and including the current element.
 This ensures that every permutation is equally likely.
 */

 for (int k_shuffle_idx = num_total_cells - 1; k_shuffle_idx > 0; k_shuffle_idx--) {
    int rand_idx = rand() % (k_shuffle_idx + 1);
    CellCoordinate temp = cell_coords[k_shuffle_idx];
    cell_coords[k_shuffle_idx] = cell_coords[rand_idx];
    cell_coords[rand_idx] = temp;
 }

 /*
 Let's visit each cell in the shuffled order
 */

 for (int k_shuffle_idx = 0; k_shuffle_idx < num_total_cells; k_shuffle_idx++) {
    cell_top_left_corner_i = cell_coords[k_shuffle_idx].i;
    cell_top_left_corner_j = cell_coords[k_shuffle_idx].j;

    /*
    Let's consider a cell defined by
    cell_top_left_corner_i
    cell_top_left_corner_j
    cell_width
    cell_height
    */

    /*
    Get the dominant color in the cell
    looking at the reference image
    */

    get_cell_dominant_color(
     reference_rgb_image_arr,
     width,
     height,
     rgb_image_colors_arr,
     rgb_image_colors_nbr,
     cell_top_left_corner_i,
     cell_top_left_corner_j,
     cell_width,
     cell_height,
     reference_rgb
    );

    if (test_flag) {
       for (cind= 0 ; cind< 3 ; cind++ ) {
          reference_rgb[cind]= 255;
       }
    }

    /*
    Get the median color in the cell
    looking at the canvas image
    */

    get_cell_median_color(
     canvas_rgb_image_arr,
     width,
     height,
     cell_top_left_corner_i,
     cell_top_left_corner_j,
     cell_height,
     cell_width,
     canvas_rgb
    );

    /*
    If the median color on the canvas is close (within error_threshold)
    to the dominant color in the reference image, do nothing
    */

    diff2= 0;
    for ( cind= 0 ; cind< 3 ; cind++ ) {
       diff2+= (canvas_rgb[cind]-reference_rgb[cind])*
               (canvas_rgb[cind]-reference_rgb[cind]);
    }
    diff_dbl= sqrt((double)diff2);

    if ( diff_dbl <= error_threshold ) {

       /*
       Do nothing!
       */

       continue;
    }

    /*
    If here,
    a brush stroke needs to be applied for that cell
    */

    /*
    Define the local reference image
    */

    local_width= cell_width;
    local_height= cell_height;

    local_rgb_image_arr= (int *)calloc(local_width*local_height,3*sizeof(int));

    local_pixel= 0;
    local_pixel_nbr= 0;
    for ( local_i= 0 ; local_i< cell_height ; local_i++ ) {
       i= cell_top_left_corner_i+local_i;
       for ( local_j= 0 ; local_j< cell_width ; local_j++ ) {
          j= cell_top_left_corner_j+local_j;
          pixel= i*width+j;
          if ( !(i >= 0) ) {
             local_pixel++;
             continue;
          }
          if ( !(i < height) ) {
             local_pixel++;
             continue;
          }
          if ( !(j >= 0) ) {
             local_pixel++;
             continue;
          }
          if ( !(j < width) ) {
             local_pixel++;
             continue;
          }
          for ( cind= 0 ; cind< 3 ; cind++ )
           local_rgb_image_arr[3*local_pixel+cind]= reference_rgb_image_arr[3*pixel+cind];
          local_pixel++;
          local_pixel_nbr++;
       }
    }
    if ( !(local_pixel == local_width*local_height) ) {
       error_handler((char *)"paint_canvas_layer");
    }

    /*
    Do not consider a brush stroke for that cell
    if the local reference image is not fully within the reference image
    */

    if ( !(local_pixel_nbr == local_width*local_height) ) {

       /*
       Free local_rgb_image_arr
       */

       free(local_rgb_image_arr);

       continue;
    }

    if ( debug_flag == 1 ) {

       /*
       Print the local image
       */

       err_flag= write_rgb_image(
        (char *)"local_rgb_image.png",
        local_rgb_image_arr,
        local_width,
        local_height
       );
       if ( err_flag == 1 ) {
          error_handler((char *)"paint_canvas_layer");
       }
    }

    if ( debug_flag == 1 ) {

       /*
       Print the local image replacing actual color with dominant color
       */

       local_rgb_image_arr2= (int *)calloc(local_width*local_height,3*sizeof(int));
       for ( i= 0 ; i< local_height ; i++ ) {
          for ( j= 0 ; j< local_width ; j++ ) {
             pixel= i*local_width+j;
             for ( cind= 0 ; cind< 3 ; cind++ )
              local_rgb_image_arr2[3*pixel+cind]= reference_rgb[cind];
          }
       }

       err_flag= write_rgb_image(
        (char *)"local_rgb_image2.png",
        local_rgb_image_arr2,
        local_width,
        local_height
       );
       if ( err_flag == 1 ) {
          error_handler((char *)"paint_canvas_layer");
       }

       free(local_rgb_image_arr2);
    }

    /*
    Get the brush stroke color
    */

    for ( cind= 0 ; cind< 3 ; cind++ )
     brush_stroke_color[cind]= reference_rgb[cind];

    /*
    Define the origin of the local reference image, that is,
    its top-left corner
    */

    local_origin_x= cell_top_left_corner_j;
    local_origin_y= cell_top_left_corner_i;

    /*
    Make the brush stroke
    */

    make_brush_stroke(
     local_rgb_image_arr,
     local_width,
     local_height,
     local_origin_x,
     local_origin_y,
     brush_stroke_color,
     &xc,
     &yc,
     &theta,
     &rectw,
     &rectl,
     &err_flag
    );

    /*
    Free local_rgb_image_arr
    */

    free(local_rgb_image_arr);

    if ( err_flag == 1 ) {

       /*
       We were not able to compute the equivalent rectangle
       */

       continue;
    }

    brush_stroke_data.dummy_int= 0;
    brush_stroke_data.origin_x= local_origin_x;
    brush_stroke_data.origin_y= local_origin_y;
    brush_stroke_data.xc= xc;
    brush_stroke_data.yc= yc;
    brush_stroke_data.theta= theta;
    brush_stroke_data.rectw= rectw;
    brush_stroke_data.rectl= rectl;
    for ( cind= 0 ; cind< 3 ; cind++ )
     brush_stroke_data.color[cind]= brush_stroke_color[cind];

    /*
    Get brush texture corresponding to texture_ind
    */

    texture_data= texture_arr[texture_ind];

    /*
    Paint the brush stroke on canvas
    */

    paint_brush_stroke(
     reference_rgb_image_arr,
     width,
     height,
     brush_stroke_data,
     texture_data,
     opacity_strength,
     bumpity_strength,
     canvas_rgb_image_arr,
     canvas_bumpity_image_arr
    );

    /*
    Icrement the number of brush strokes
    */

    brush_stroke_nbr++;
 }

 fprintf(stdout,"Number of cells = %d\n",
  num_total_cells);
 fprintf(stdout,"Number of brush strokes created = %d\n",
  brush_stroke_nbr);

 /*
 Free allocated memory for cell coordinates
 */

 if (cell_coords != NULL) {
    free(cell_coords);
 }

}
