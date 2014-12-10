# Takes the command line arguments from the user in the following order:
#	- Image file on which we want to run the Graph Laplacian
#	- Label File for the given image
#	- Graph Lab Output file for running the output


import cv2
import numpy as np
import sys
import Image

filename = sys.argv[1] 
labelfile = sys.argv[2]
graphoutput = sys.argv[3]

im = Image.open(filename)
size = im.size
u, v = size
width = u
height = v
print width 
print height
# width, height = 1490, 747

def create_blank(width, height, rgb_color=(0, 0, 0)):
    """Create new image(numpy array) filled with certain color in RGB"""
    # Create black blank image
    image = np.zeros((height, width, 3), np.uint8)

    # Since OpenCV uses BGR, convert the color first
    color = tuple(reversed(rgb_color))
    # Fill image with color
    image[:] = color

    return image

# Create new blank 300x300 red image

# width, height = 1490, 747

white = (255, 255, 255)

image = np.zeros((height,width,3), np.uint8);

# cv2.rectangle(image, (100, 100), (200, 200), (255, 0, 0), 3)

# f = open(

array = []

with open(labelfile) as f:
	for line in f:
		line = line.rstrip('\n')
		values = line.split(',')
		array.append(values)

lookup = []
with open(graphoutput) as f1:
	for line1 in f1:
		line1 = line1.rstrip('\n')
		lookup.append(line1)

# print array
# print lookup

for i in range (0, height-1):
	for j in range (0, width-1):
		s = array[i][j]
		s1 = int(s) - 1
		#print s1
			 
		colortype = int(lookup[s1])
		
		if colortype == 1:
			# is green in saved image
			# changed to red
			image[i, j] = [255, 0, 0]
		elif colortype == 2:
			# is black in saved image
			image[i, j] = [200, 200, 0]
		elif colortype == 3:
			# is red in saved image
			image[i, j] = [0, 255, 0]
		elif colortype == 4:
			# is blue in saved image
			image[i, j] = [50, 150, 250]
		#else:
			# is black in saved image
		#	image[i, j] = [0, 0, 255]

cv2.imwrite('test.jpg', image)



def create_blank(width, height, rgb_color=(0, 0, 0)):
    """Create new image(numpy array) filled with certain color in RGB"""
    # Create black blank image
    image = np.zeros((height, width, 3), np.uint8)

    # Since OpenCV uses BGR, convert the color first
    color = tuple(reversed(rgb_color))
    # Fill image with color
    image[:] = color

    return image

# Create new blank 300x300 red image

# width, height = 1490, 747

white = (255, 255, 255)

#image = cv2.imread("res.jpg")

image1 = cv2.imread(filename)
image2 = cv2.imread('test.jpg')

image = cv2.addWeighted(image1, 0.7 , image2, 0.3, 0)

cv2.imwrite('res.jpg', image)

