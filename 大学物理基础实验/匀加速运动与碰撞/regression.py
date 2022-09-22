import matplotlib.pyplot as plt
import numpy as np
import math
import math
plt.rcParams['font.family'] = ['sans-serif']
plt.rcParams['font.sans-serif'] = ['SimHei']
def linear_regression(x, y,x_label,y_label,title):
  N = len(x)
  sumx = sum(x)
  sumy = sum(y)
  sumx2 = sum(x ** 2)
  sumxy = sum(x * y)
  sum1=sum([(x[i]-sumx/N)*(y[i]-sumy/N) for i in range(0,N)])
  sum21=sum([(x[i]-sumx/N)**2 for i in range(0,N)])
  sum22=sum([(y[i]-sumy/N)**2 for i in range(0,N)])
  sum2=math.sqrt(sum21)*math.sqrt(sum22)
  print("相关系数："+str(sum1/sum2))
  A = np.mat([[N, sumx], [sumx, sumx2]])
  b = np.array([sumy, sumxy])
  a0,a1=np.linalg.solve(A, b)
  _X1 = [min(x), max(x)]
  _Y1 = [a0 + a1 * x for x in _X1]
  #显示图像
  plt.plot( x, y, 'ro', linewidth=2,label="原始数据")
  plt.plot(_X1, _Y1, 'b',linewidth=2,label='拟合直线'+"y="+str(a1)+"x+"+str(a0),color='C0')
  plt.xlabel(x_label)
  plt.ylabel(y_label)
  plt.title(title, fontsize=24)
  plt.legend()
  plt.show()

x=np.array([20.0,30.0,40.0,50.0,60.0])/100*2
t1=np.array([32.08,27.84,24.98,22.57,20.98])/1000
t2=np.array([32.44,27.60,24.79,22.66,21.03])/1000
t3=np.array([32.48,27.53,24.75,22.70,21.04])/1000
t=(t1+t2+t3)/3
y=(1.05/100/t)**2
plt.xlim(0.39,1.21)
plt.ylim(0.1,0.25)
linear_regression(x, y,"2s(m)","v^2",'')

