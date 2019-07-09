#	Lame cutter v1 by George Kokiadis
#	https://github.com/georgekoki
#	georgekoki1999@gmail.com
#
#	Chop videos into works of art
#
#	This file contains all the source code of the app.
#	Required external libs: OpenCV 4.1.0
#	Please consult the readme

import cv2
import numpy as np
import sys

# Reads a frame dist frames away from the current one
def readNextFrame(dist, cap):
    print("Reading next frame...")
    for x in range(dist):
        success, image = vidcap.read()
    if success != True:
        raise ValueError
    print("Done.")
    return image

# Calculates a vertical line for the image
def getLine(img):
    print("Calculating Line...")
    arr = np.ndarray(shape = (img.shape[0], 1, 3))
    for line in range(img.shape[0]):
            arr[line] = pixelMedian(line, img)
    print("Done.")
    return arr

# Gets the pixel median of a line from the video
def pixelMedian(line, img):
    R = 0
    G = 0
    B = 0
    for pixel in range(img.shape[1]):
        R = R + img[line][pixel][0]
        G = G + img[line][pixel][1]
        B = B + img[line][pixel][2]
    R = R / img.shape[1]
    G = G / img.shape[1]
    B = B / img.shape[1]

    return np.array([R, G, B])

# Main executable

step = 1
vidcap = cv2.VideoCapture("00152.MTS")
x = 1
print(">> Frame %d" %x)
try:
    line = getLine(readNextFrame(step, vidcap))
except ValueError:
    print("Video Contains less frames than step provided")
    exit
line_count = line.shape[0]
final_list = [line]
while True:
    final_list.append(line)
    print(">> Frame %d" %x)
    try:
            line = getLine(readNextFrame(step, vidcap))
    except ValueError:
        print("Reached Video end")
        break
    x = x + 1

final_array = np.ndarray(shape = (x, line_count, 1, 3))

for i in range(x):
    final_array[i] = final_list[i]
cv2.imwrite("frame.jpg", np.swapaxes(final_array, 0, 2)[0])