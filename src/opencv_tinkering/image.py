import numpy as np
import cv2

#Load a image in color

img = cv2.imread('rio2.jpg')


cv2.imshow('image', img)
cv2.waitKey(0)
cv2.destroyAllWindows()

px = img[100, 100]
print px

cv2.namedWindow('image', cv2.WINDOW_NORMAL)
cv2.imshow('image', img)
cv2.waitKey(0)
cv2.destroyAllWindows()


