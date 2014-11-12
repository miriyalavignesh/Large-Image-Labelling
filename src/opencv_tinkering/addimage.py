import cv2
import numpy as np

width, height = 1490, 747

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

width, height = 1490, 747

white = (255, 255, 255)

image = cv2.imread("res.jpg")

image1 = cv2.imread('rio2.jpg')
image2 = cv2.imread('test.jpg')

image = cv2.addWeighted(image1, 0.7 , image2, 0.3, 0)

cv2.imwrite('res.jpg', image)


