# thepainter2

Simulates painting with oil. Input is a photograph. Output is (hopefully) a painterly rendering.

This is, I believe, an improved version of the [thepainter](https://github.com/ugocapeto/thepainter).

Written and tested on Ubuntu 24.04.

For thepainter2 to link, you need to get [common](https://github.com/ugocapeto/common). The "common" dir should be at the same level as the "thepainter2" dir in your tree structure (if you don't want to play with the Makefiles).

To compile "thepainter2", cd thepainter2, then:<br />
make MODE=debug<br />
or<br />
make MODE=release<br />

To link "thepainter2", cd main from thepainter2, then:<br />
make

To run "thepainter2", cd test from main and go to any of the dirs under there, then:<br />
../../thepainter2

The exec reads in a file called "thepainter2_input.txt" that contains all the input.

The input looks like this:

```
content_image.png<br />
../../texture/canvas002/rgb.png<br />
../../texture/canvas002/bumpity.png<br />
24<br />
384 256   0   0 0.8 0.8 10.0 0<br />
384 256 192   0 0.8 0.8 10.0 0<br />
384 256   0 128 0.8 0.8 10.0 0<br />
384 256 192 128 0.8 0.8 10.0 0<br />
192 128   0   0 0.8 0.8 20.0 1<br />
192 128  91   0 0.8 0.8 20.0 1<br />
192 128   0  64 0.8 0.8 20.0 1<br />
192 128  91  64 0.8 0.8 20.0 1<br />
 96  64   0   0 0.8 0.8 30.0 2<br />
 96  64  48   0 0.8 0.8 30.0 2<br />
 96  64   0  32 0.8 0.8 30.0 2<br />
 96  64  48  32 0.8 0.8 30.0 2<br />
 48  32   0   0 0.8 0.8 40.0 3<br />
 48  32  24   0 0.8 0.8 40.0 3<br />
 48  32   0  16 0.8 0.8 40.0 3<br />
 48  32  24  16 0.8 0.8 40.0 3<br />
 24  16   0   0 0.8 0.8 50.0 4<br />
 24  16  12   0 0.8 0.8 50.0 4<br />
 24  16   0   8 0.8 0.8 50.0 4<br />
 24  16  12   8 0.8 0.8 50.0 4<br />
 12   8   0   0 0.8 0.8 60.0 5<br />
 12   8   6 0 0.8 0.8 60.0 5<br />
 12   8   0 4 0.8 0.8 60.0 5<br />
 12   8   6 4 0.8 0.8 60.0 5<br />
6<br />
../../texture/brush256/opacity.png<br />
../../texture/brush256/bumpity.png<br />
../../texture/brush128/opacity.png<br />
../../texture/brush128/bumpity.png<br />
../../texture/brush064/opacity.png<br />
../../texture/brush064/bumpity.png<br />
../../texture/brush032/opacity.png<br />
../../texture/brush032/bumpity.png<br />
../../texture/brush016/opacity.png<br />
../../texture/brush016/bumpity.png<br />
../../texture/brush008/opacity.png<br />
../../texture/brush008/bumpity.png<br />
canvas_rgb_image.png<br />
canvas_bumpity_image.png<br />
```

In this case, we are gonna paint 24 layers using 6 different brushes.
The difference between the brushes is the size of the brush. For speed, for a given layer, you really want to use a brush that is about the same size as the layer's cell size. Doesn't have to be but it makes things way faster.

Let's look at how a layer is given, for example, this one:<br />
192 128<br />91<br />64 0.8 0.8 20.0 1<br />
First and second number represent the cell width and height. Recall that brush strokes are applied using a uniform grid. For each grid cell, one brush stroke is gonna be applied if if the median color in the current canvas differs too much from the reference image. In theory, the grid cell height should be about the same as the brush width.
Third and fourth number represent the offset in the horizontal (width) and vertical (height) direction. Typically, you would want to use four layers instead of just one for better coverage. That's why there are four layers per brush.<br />
Fifth and sixth number are the opacity and bumpity strenght just like in "thepainter".<br />
Seventh number is the threshold for deciding if the current canvas color is close enough. The larger the number, the looser the painting. Again, same as "thepainter".<br />
Eighth number is the index into the array of brushes.

After the layers come the brushes.
Each brush is defined by its opacity texture and its bumpity texture.
For example, here is one brush definition:<br />
../../texture/brush128/opacity.png<br />
../../texture/brush128/bumpity.png<br />
This brush has a width of 128 pixels and it should be used with a grid cell height equal to 128. Again, matching the sizes between grid cells and brushes is important to get speed. If they don't match, scaling (in particular, downscaling) might get cpu intensive because of having to apply Gaussian blur.

The reference image is "content_image.png". **Unlike "thepainter" where the reference image could be pretty much anything in terms of number of colors, here, it is assumed the reference image uses a limited palette, for example, 16 colors or 32 colors.**

In Gimp, to reduce the number of colors, you click on Image->Mode->Indexed...:

![image](https://github.com/user-attachments/assets/ed226273-85f0-4628-b02a-27d8dd0781e2)

The exec outputs 2 images: "canvas_rgb_image.png" and "canvas_bumpity_image.png". You can use the bumpity map as a bump map to show the texture of the paint.

In Gimp, to use a bump map, you click on Filters->Map->Bump Map...:

![image](https://github.com/user-attachments/assets/1bda6fd6-ca6c-43e5-89fe-a4b16283dda9)

Click on the "?" next to "Aux Input" and then double-click on the bumpity image.

Here is an example. It's in the test dir.

Reference image (16 colors):

![image](https://github.com/user-attachments/assets/ecbc2580-b99e-439a-8263-0a94c38a6d0e)

Output from thepainter2 (at 50% magnification):

![image](https://github.com/user-attachments/assets/0cdc0d6f-6568-4032-b82a-338def139e85)

At 100% maginification:

![image](https://github.com/user-attachments/assets/2a063cf2-bd27-4aae-8de8-176e1c545a13)

In theory, the only time you may need to edit the input file is if you use reference images that are drastically different from the ones in the test dir (in terms of size). In the test dir, I have images that are 1280 and 1920 pixels along the largest dimension. Basically, larger images require larger brushes.


