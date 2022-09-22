import cv2
import numpy as np
import glob
import os

exit() #防止误点击
#文件名必须严格按字典序排列（注意字符串长度不同导致的字典序非预期）
base='.\\2021.11.16\\'
paths=glob.glob(os.path.join(base,'*.jpg'))
i=1
for path in paths:
    os.rename(path,base+str(i)+'.jpg')
    i=i+1


