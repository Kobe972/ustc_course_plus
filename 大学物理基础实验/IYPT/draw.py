import curve
import warnings
import numpy as np
warnings.filterwarnings("ignore")
arr = input("Input data x:") 
x = [float(n) for n in arr.split()]
arr = input("Input data y:") 
y = [float(n) for n in arr.split()]
x_label = input("Input the label of axis x(enter for no label):")
y_label = input("Input the label of axis y(enter for no label):")
title = input("Input the title of the graph(enter for no title):")
curve.curve(np.array(x),np.array(y),x_label,y_label,title)
