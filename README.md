Intermediate Programming Course Project
Collaboration between: Jennifer Lin and Chester Huynh

Image Manipulator 
------------------
A command-line-based user interface image manipulator. 
The program will be able to read PPM image files from disk, perform one of a variety of image manipulation tasks, 
and then write the result back to disk as a new PPM image file. 

The operations supported by the program include:
  swap - swap color channels (i.e. what was red becomes blue, what was blue becomes green, etc.)
  grayscale - convert to grayscale (i.e. a full-color image becomes shades of gray)
  contrast - increase/decrease contrast
  zoom_in - replicate each pixel 4 times so that image appears to be zoomed in
  zoom_out - reduce each block of 4 pixels to one "average" pixel so that image appears to be zoomed out
  occlude - black out (or "occlude") a specified rectangular region of the image
  blur - blur the image (smooth and soften its edges) by a specified amount

For example, at the command prompt, a user of your program might type 
./project nika.ppm nika-zoom_in.ppm zoom_in
to take as input a PPM file named nika.ppm and create an output file named nika-zoom_in.ppm which contains a 
blacked-out version of the nika image. 

Or, if the desire is to occlude the portion of the nika image with top left corner (x=60, y=25) and bottom right 
corner is (x=600, y=565), then the user could type 
./project nika.ppm nika-occlude.ppm occlude 60 25 600 565
to generate the output file named nika-occlude.ppm.

Code Files
----------
- Makefile
- ppmIO.h, ppmIO.c - Read in and write out the PPM image files
- ImageManip.h, ImageManip.c - Various image manipulation operations
- project.c - Command Line interactions in main method
