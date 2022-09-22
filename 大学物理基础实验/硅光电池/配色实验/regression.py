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
  plt.plot(_X1, _Y1, 'b',linewidth=2,label='拟合直线'+"y="+str(a1)+"x"+str(a0),color='C0')
  plt.xlabel(x_label)
  plt.ylabel(y_label)
  plt.title(title, fontsize=24)
  plt.legend()
  plt.show()

y=np.array([0.0024,0.0051,0.0081,0.0111,0.0141,0.0171,0.0198,0.0230,0.0261,0.0290,0.0317,0.0347,0.0376,0.0408,0.0434,0.0465,0.0493,0.0522,0.0550,0.0580])-0.0002
x=np.array([0.5,1,1.5,2.0,2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.5,7.0,7.5,8.0,8.5,9.0,9.5,10.0])*10
plt.xlim(0,100)
plt.ylim(0.0020,0.0600)
linear_regression(x, y,"I(mA)","L(V)",'')

