import cv2
import numpy as np
import glob
import os

hull=[]
def on_EVENT_LBUTTONDOWN(event, x, y, flags, param):
    if event == cv2.EVENT_LBUTTONDOWN:
        hull.append([x,y])

def transform(filename):
    cv2.namedWindow("ppt")
    cv2.setMouseCallback("ppt", on_EVENT_LBUTTONDOWN)
    src=cv2.imread(filename)
    src = cv2.resize(src,None,fx=0.25,fy=0.25)
    cv2.imshow('ppt',src)
    cv2.waitKey(0)
    while len(hull)<4:
        pass
    rightbottom=0
    leftbottom=1
    righttop=2
    lefttop=3
    for i in range(0,len(hull)):
        if hull[i][0]+hull[i][1]>hull[rightbottom][0]+hull[rightbottom][1]:
            rightbottom=i
            continue
        if hull[i][0]*2-hull[i][1]>hull[righttop][0]*2-hull[righttop][1]:
            righttop=i
            continue
        if -hull[i][0]+hull[i][1]*2>-hull[leftbottom][0]+hull[leftbottom][1]*2:
            leftbottom=i
            continue
        if -hull[i][0]-hull[i][1]>-hull[leftbottom][0]-hull[leftbottom][1]:
            lefttop=i
            continue
    srcPoints=np.array([hull[lefttop],hull[leftbottom],hull[righttop],hull[rightbottom]],dtype='float32')
    dstPoints=np.array([[0,0],[0,src.shape[0]],[src.shape[1],0],[src.shape[1],src.shape[0]]],dtype='float32')
    transMat=cv2.getPerspectiveTransform(srcPoints, dstPoints)
    processed=cv2.warpPerspective(src,transMat,(src.shape[1],src.shape[0]))
    processed = cv2.resize(processed,None,fx=4,fy=4)
    cv2.imwrite(filename,processed)
    hull.clear()
    
paths=glob.glob(os.path.join('.\\remaining\\','*.jpg'))
for path in paths:
    transform(path)


