#include "header.h"

/* Declare the global error flag */
extern int debug_flag_global;

void paint_canvas(
   int *reference_rgb_image_arr,
   int width,
   int height,
   int *initial_canvas_rgb_image_arr,
   int initial_canvas_rgb_image_width,
   int initial_canvas_rgb_image_height,
   int *initial_canvas_bumpity_image_arr,
   int initial_canvas_bumpity_image_width,
   int initial_canvas_bumpity_image_height,
   int *cell_width_arr,
   int *cell_height_arr,
   int *offset_j_arr,
   int *offset_i_arr,
   double *opacity_strength_arr,
   double *bumpity_strength_arr,
   double *error_threshold_arr,
   int *texture_ind_arr,
   int layer_nbr,
   texture_struct *texture_arr,
   int texture_nbr,
   int **pcanvas_rgb_image_arr,
   int **pcanvas_bumpity_image_arr
)

{
   int *canvas_rgb_image_arr;
   int layer_ind;
   int cell_width;
   int cell_height;
   int offset_j;
   int offset_i;
   int debug_flag= debug_flag_global;
   int err_flag;
   int *canvas_bumpity_image_arr;
   double error_threshold;
   double opacity_strength;
   double bumpity_strength;
   char scale_method[256];
   char scale_gaussian[256];
   int texture_ind;
   int *rgb_image_colors_arr;
   int rgb_image_colors_nbr;
   
   get_rgb_image_colors(
      reference_rgb_image_arr,
      width,
      height,
      &rgb_image_colors_arr,
      &rgb_image_colors_nbr
   );

   /*
   Allocate memory for canvas_rgb_image_arr
   */
   
   canvas_rgb_image_arr= (int *)calloc(width*height,3*sizeof(int));
   
   /*
   Initialize the canvas rgb image with the input initial canvas rgb image
   */
   
   strcpy(scale_method,"cubic");
   strcpy(scale_gaussian,"no");
   scale_rgb_image(
      initial_canvas_rgb_image_arr,
      initial_canvas_rgb_image_width,
      initial_canvas_rgb_image_height,
      canvas_rgb_image_arr,
      width,
      height,
      scale_method,
      scale_gaussian
   );
   
   if ( debug_flag == 1 ) {
      
      /*
      Print the canvas rgb image
      */
      
      err_flag= write_rgb_image(
         (char *)"canvas_rgb_image.png",
         canvas_rgb_image_arr,
         width,
         height
      );
      if ( err_flag == 1 ) {
         error_handler((char *)"paint_canvas");
      }
   }
   
   /*
   Allocate memory for canvas_bumpity_image_arr
   */
   
   canvas_bumpity_image_arr= (int *)calloc(width*height,sizeof(int));
   
   /*
   Initialize the canvas bumpity image with the input initial canvas bumpity image
   */
   
   strcpy(scale_method,"cubic");
   strcpy(scale_gaussian,"no");
   scale_image(
      initial_canvas_bumpity_image_arr,
      initial_canvas_bumpity_image_width,
      initial_canvas_bumpity_image_height,
      canvas_bumpity_image_arr,
      width,
      height,
      scale_method,
      scale_gaussian
   );
   
   if ( debug_flag == 1 ) {
      
      /*
      Print the canvas bumpity image
      */
      
      err_flag= write_image(
         (char *)"canvas_bumpity_image.png",
         canvas_bumpity_image_arr,
         width,
         height
      );
      if ( err_flag == 1 ) {
         error_handler((char *)"paint_canvas");
      }
   }
   
   /*
   Paint the canvas layer by layer
   */
   
   for ( layer_ind= 0 ;
      layer_ind< layer_nbr ;
      layer_ind++ ) {

      cell_width= cell_width_arr[layer_ind];
      cell_height= cell_height_arr[layer_ind];
      offset_j= offset_j_arr[layer_ind];
      offset_i= offset_i_arr[layer_ind];
      opacity_strength= opacity_strength_arr[layer_ind];
      bumpity_strength= bumpity_strength_arr[layer_ind];
      error_threshold= error_threshold_arr[layer_ind];
      texture_ind= texture_ind_arr[layer_ind];
      
      fprintf(stdout,"Painting canvas layer using layer_ind = %2d cell_width = %3d cell_height = %3d offset_j = %3d offset_i = %3d opacity_strength = %.2f bumpity_strength = %.2f error_threshold = %6.2f texture_ind = %2d...\n",layer_ind,cell_width,cell_height,offset_j,offset_i,opacity_strength,bumpity_strength,error_threshold,texture_ind);
      
      /*
      Paint the canvas layer
      */
      
      paint_canvas_layer(
         reference_rgb_image_arr,
         width,
         height,
         rgb_image_colors_arr,
         rgb_image_colors_nbr,
         cell_width,
         cell_height,
         offset_j,
         offset_i,
         opacity_strength,
         bumpity_strength,
         error_threshold,
         texture_arr,
         texture_nbr,
         texture_ind,
         canvas_rgb_image_arr,
         canvas_bumpity_image_arr
      );
      
      if ( debug_flag == 1 ) {
         
         /*
         Print the canvas image
         */
         
         err_flag= write_rgb_image(
            (char *)"canvas_rgb_image.png",
            canvas_rgb_image_arr,
            width,
            height
         );
         if ( err_flag == 1 ) {
            error_handler((char *)"paint_canvas");
         }
      }
      
      if ( debug_flag == 1 ) {
         
         /*
         Print the canvas bumpity image
         */
         
         err_flag= write_image(
            (char *)"canvas_bumpity_image.png",
            canvas_bumpity_image_arr,
            width,
            height
         );
         if ( err_flag == 1 ) {
            error_handler((char *)"paint_canvas");
         }
      }
      
      fprintf(stdout,"Painting canvas layer using layer_ind = %2d cell_width = %3d cell_height = %3d offset_j = %3d offset_i = %3d opacity_strength = %.2f bumpity_strength = %.2f error_threshold = %6.2f texture_ind = %2d... done.\n",layer_ind,cell_width,cell_height,offset_j,offset_i,opacity_strength,bumpity_strength,error_threshold,texture_ind);
   }
   
   (*pcanvas_rgb_image_arr)= canvas_rgb_image_arr;
   (*pcanvas_bumpity_image_arr)= canvas_bumpity_image_arr;
   
}
