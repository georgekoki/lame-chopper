# Lame Chopper
#### Code by George 'Koki' Kokiadis
##### *https://github.com/georgekoki* 

Welcome to lame chopper. This was a fun little programming challenge i put myself through to teach myself python.
While the original algorithm was written in python, it was re-written in c++ because the python implementation was too damn slow.

But what is lame chopper?
Lame chopper takes any video you feed it, and turns it into a modern work of art.

The algorithm does the following:
1. Take the video you give it
2. Grab one frame from it
3. From that frame, grab every row of pixels and get their average colour
4. Construct a line from every average color of every row
5. Grab another frame
6. If the video is not over, go to 3.
7. Stitch every line together
8. Save it as an image file

It uses OpenCV 4.1.0 to extract image data and to save images. So if you plan on tweaking the code/compile it yourself you can
download it for free here: https://opencv.org/

If you have any questions, please send them to my email:
georgekoki1999@gmail.com