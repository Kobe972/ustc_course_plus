import cv2
import numpy as np
import glob
import os
exit() #防止误点击
def transform(filename):
    src=cv2.imread(filename)
    hsv = cv2.cvtColor(src, cv2.COLOR_BGR2HSV)
    hsv=cv2.medianBlur(hsv, 7)
    low_hsv = np.array([0,0,150])
    high_hsv = np.array([255,255,255])
    mask = cv2.inRange(hsv,lowerb=low_hsv,upperb=high_hsv)
    kernel = np.ones((3,3),np.uint8)
    mask = cv2.dilate(mask,kernel,iterations = 6)
    mask = cv2.erode(mask,kernel,iterations = 6) #iteration的值越高，腐蚀程度就越高 呈 正相关关系且只能是整数
    gray=cv2.bitwise_and(src,src,mask=mask)
    gray=cv2.Canny(gray,50,500)
    contours,_=cv2.findContours(gray,cv2.RETR_CCOMP,cv2.CHAIN_APPROX_SIMPLE)
    maxArea=0
    for index in range(0,len(contours)):
        if cv2.contourArea(contours[index])>cv2.contourArea(contours[maxArea]):
            maxArea = index
    polyContour=cv2.approxPolyDP(contours[maxArea],100,True)
    hull=cv2.convexHull(polyContour)
    length = len(hull)
    if length<4:
        return
    rightbottom=0
    leftbottom=1
    righttop=2
    lefttop=3
    for i in range(0,len(hull)):
        if hull[i][0][0]+hull[i][0][1]>hull[rightbottom][0][0]+hull[rightbottom][0][1]:
            rightbottom=i
            continue
        if hull[i][0][0]*2-hull[i][0][1]>hull[righttop][0][0]*2-hull[righttop][0][1]:
            righttop=i
            continue
        if -hull[i][0][0]+hull[i][0][1]*2>-hull[leftbottom][0][0]+hull[leftbottom][0][1]*2:
            leftbottom=i
            continue
        if -hull[i][0][0]-hull[i][0][1]>-hull[leftbottom][0][0]-hull[leftbottom][0][1]:
            lefttop=i
            continue
    srcPoints=np.array([hull[lefttop][0],hull[leftbottom][0],hull[righttop][0],hull[rightbottom][0]],dtype='float32')
    dstPoints=np.array([[0,0],[0,src.shape[0]],[src.shape[1],0],[src.shape[1],src.shape[0]]],dtype='float32')
    transMat=cv2.getPerspectiveTransform(srcPoints, dstPoints)
    processed=cv2.warpPerspective(src,transMat,(src.shape[1],src.shape[0]))
    cv2.imwrite(filename,processed)
paths=glob.glob(os.path.join('.\\2021.11.16\\','*.jpg'))
for path in paths:
    transform(path)


