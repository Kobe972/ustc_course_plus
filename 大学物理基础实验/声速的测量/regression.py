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

x=np.array(list(range(1,9)))
y=np.array([22.70,21.56,18.25,16.02,14.02,12.21,10.30,8.76])
plt.xlim(0.5,9.5)
plt.ylim(8.5,23)
linear_regression(x, y,"position","L(cm)",'')

