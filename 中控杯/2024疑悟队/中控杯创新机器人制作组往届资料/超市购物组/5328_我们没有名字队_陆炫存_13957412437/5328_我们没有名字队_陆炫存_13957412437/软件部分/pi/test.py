import cv2
import numpy as np
from skimage.measure import label
def check(img):
    img_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

    ## Gen lower mask (0-5) and upper mask (175-180) of RED
    mask1 = cv2.inRange(img_hsv, (0,43,100), (10,255,255))
    mask2 = cv2.inRange(img_hsv, (156,43,100), (180,255,255))

    ## Merge the mask and crop the red regions
    maskred = cv2.bitwise_or(mask1, mask2 )
    maskblue = cv2.inRange(img_hsv, (100,43,100), (124,255,255))
    def lcc(mask):
        labeled,num=label(mask,background=0,return_num=True)
        lcc=0
        for i in range(1,num+1):
            lcc=max(lcc,np.sum(labeled==i))
        return lcc
    print(lcc(maskred),lcc(maskblue))
    return lcc(maskred)>3000,lcc(maskblue)>3000
#check(cv2.imread('up.jpg'))