#include "header.h"

void compute_equivalent_rectangle(
 double *image_arr,
 int width,
 int height,
 double *pxc,
 double *pyc,
 double *ptheta,
 double *prectw,
 double *prectl,
 int *perr_flag
)

/*
image_arr is a width*height grayscale image with values between 0.0 and 1.0
*/

/*
(xc,yc) is the center of the equivalent rectangle
w/r to origin of the image which is the top left
theta is the angle betwen the longer edge and the x-axis
w is the width of the equivalent rectangle
l is the length of the equivalent rectangle
*/

{

 int l;
 int m;
 double moment;
 double M00;
 double M10;
 double M01;
 double M11;
 double M20;
 double M02;
 double xc;
 double yc;
 double a;
 double b;
 double c;
 double theta;
 double temp_dbl;
 double rectw;
 double rectl;
 double pi= acos(-1.0);
 int err_flag;

 xc= 0.0;
 yc= 0.0;
 theta= 0.0;
 rectw= 0.0;
 rectl= 0.0;
 err_flag= 0;

 /*
 Compute M00
 */

 l= 0;
 m= 0;

 compute_image_moment(
  image_arr,
  width,
  height,
  l,
  m,
  &moment
 );

 M00= moment;

 /*
 Compute M10
 */

 l= 1;
 m= 0;

 compute_image_moment(
  image_arr,
  width,
  height,
  l,
  m,
  &moment
 );

 M10= moment;

 /*
 Compute M01
 */

 l= 0;
 m= 1;

 compute_image_moment(
  image_arr,
  width,
  height,
  l,
  m,
  &moment
 );

 M01= moment;

 /*
 Compute M11
 */

 l= 1;
 m= 1;

 compute_image_moment(
  image_arr,
  width,
  height,
  l,
  m,
  &moment
 );

 M11= moment;

 /*
 Compute M20
 */

 l= 2;
 m= 0;

 compute_image_moment(
  image_arr,
  width,
  height,
  l,
  m,
  &moment
 );

 M20= moment;

 /*
 Compute M02
 */

 l= 0;
 m= 2;

 compute_image_moment(
  image_arr,
  width,
  height,
  l,
  m,
  &moment
 );

 M02= moment;

 /*
 Compute center of equivalent rectangle (xc,yc)
 */

 xc= M10/M00;
 yc= M01/M00;

 /*
 Compute a
 */

 a= M20/M00 - xc*xc;

 /*
 Compute b
 */

 b= 2*(M11/M00 - xc*yc);

 /*
 Compute c
 */

 c= M02/M00 - yc*yc;

 /*
 Compute theta (angle between longer edge and the x-axis)
 */

 /*
 In the paper,
 the authors say that the angle is always w/r to the x-axis but that's not correct.
 The angle may be w/r to the y-axis, probably when the angle is smaller than w/r to the x-axis.
 */

 /*
 Note that 
 atan() returns an angle between -po/2 and +pi/2
 */

 /*
 To check if theta is computed correctly,
 run the test cases test001, test002, test003, test004
 setting test_flag= 1 in paint_brush_stroke.c
 and setting debug_flag_global= 1 in main.c
 */

 if ( (a-c) == 0.0 ) {
    theta= 0.0; 
 }
 else {
    theta= atan(b / (a-c));
    theta/= 2.0;
 }

 if ( (a-c) < 0.0 ) {

    /*
    Go from angle between longer edge and y-axis to
            angle between longer edge and x-axis
    by adding pi/2
    */

    theta+= pi/2.0;
 }

 /*
 Adjust the angle so that it's always between -pi/2 and pi/2
 */

 if (theta<-pi/2) {
    theta+= pi;
 }
 if (theta>+pi/2) {
    theta-= pi;
 }

 /*
 It seems to me that
 theta should vary between -pi/2 and +pi/2, that is, -1.57 and +1.57
 */

 /*
 if (!(theta>-pi/2&&theta<+pi/2)) {
    error_handler((char *)"compute_equivalent_rectlangle");
 }
 */

 /*
 Compute rectw (rectangle width)
 */

 temp_dbl= b*b + (a-c)*(a-c);
 temp_dbl= sqrt(temp_dbl); 
 rectw= 6.0*(a+c-temp_dbl);
 if ( !(rectw >= 0.0) ) {
    /*
    error_handler((char *)"compute_equivalent_rectangle");
    */
    err_flag= 1;
    goto END;
 }
 rectw= sqrt(rectw);

 /*
 Compute rectl (rectangle length)
 */

 temp_dbl= b*b + (a-c)*(a-c);
 temp_dbl= sqrt(temp_dbl);
 rectl= 6.0*(a+c+temp_dbl);
 if ( !(rectl >= 0.0) ) {
    /*
    error_handler((char *)"compute_equivalent_rectangle");
    */
    err_flag= 1;
    goto END;
 }
 rectl= sqrt(rectl);

 END:

 (*pxc)= xc;
 (*pyc)= yc;
 (*ptheta)= theta;
 (*prectw)= rectw;
 (*prectl)= rectl;
 (*perr_flag)= err_flag;

}
