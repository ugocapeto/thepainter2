#include "header.h"

void get_cell_dominant_color(
   int *rgb_image_arr,
   int width,
   int height,
   int *rgb_image_colors_arr,
   int rgb_image_colors_nbr,
   int top_left_corner_i,
   int top_left_corner_j,
   int cell_width,
   int cell_height,
   int rgb[3]
)

/*
top_left_corner_i,top_left_corner_j is the top left corner of the cell
*/

{
   int *color_counts;
   int k;
   int local_i, local_j;
   int pixel;
   int current_pixel_rgb[3];
   int max_count;
   int dominant_color_ind;
   int cind;
   int i, j;
   int count;

   // Allocate memory to store counts for each unique color.
   color_counts = (int *)calloc(rgb_image_colors_nbr, sizeof(int));

   // Iterate through each pixel in the specified cell
   for (local_i = 0; local_i < cell_height; local_i++) {
      i = top_left_corner_i + local_i;

      // Ensure the current row is within the image bounds
      if (i < 0 || i >= height) {
         continue;
      }

      for (local_j = 0; local_j < cell_width; local_j++) {
         j = top_left_corner_j + local_j;

         // Ensure the current column is within the image bounds
         if (j < 0 || j >= width) {
            continue;
         }

         pixel = i * width + j;

         // Extract the RGB color of the current pixel from the main image
         current_pixel_rgb[0] = rgb_image_arr[3 * pixel + 0];
         current_pixel_rgb[1] = rgb_image_arr[3 * pixel + 1];
         current_pixel_rgb[2] = rgb_image_arr[3 * pixel + 2];

         // Find this color in the rgb_image_colors_arr and increment its count
         for (k = 0; k < rgb_image_colors_nbr; k++) {
            if (rgb_image_colors_arr[k * 3 + 0] == current_pixel_rgb[0] &&
                rgb_image_colors_arr[k * 3 + 1] == current_pixel_rgb[1] &&
                rgb_image_colors_arr[k * 3 + 2] == current_pixel_rgb[2]) {
               color_counts[k]++;
               break; // Found the color, move to the next pixel
            }
         }
         if (!(k < rgb_image_colors_nbr)) {
            error_handler((char *)"get_cell_dominant_color");
         }
      }
   }

   max_count= 0;
   dominant_color_ind= -1;

   // Find the color with the maximum count
   for (k = 0; k < rgb_image_colors_nbr; k++) {
      if (color_counts[k] > max_count) {
         count = color_counts[k];
         if (count > max_count) {
            max_count= count;
            dominant_color_ind = k;
         }
      }
   }
   if (dominant_color_ind == -1) {
      error_handler((char *)"get_cell_dominant_color");
   }

   // Set the output rgb to the dominant color found
   for (cind = 0; cind < 3; cind++) {
      rgb[cind] = rgb_image_colors_arr[dominant_color_ind * 3 + cind];
   }

   // Free the allocated memory for counts
   free(color_counts);

}
