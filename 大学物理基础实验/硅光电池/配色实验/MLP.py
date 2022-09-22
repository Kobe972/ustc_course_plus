import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
def curve(data1,data2,x_label='x',y_label='y',title='Untitled',color='r-'):
    l=min(data1)
    r=max(data1)
    x_data = np.linspace(l, r, min(int(150*(r-l)),1000), dtype=np.float32)[:, np.newaxis]
    x = np.array(data1)[:, np.newaxis]
    y_data = np.array(data2)[:, np.newaxis]
    xs = tf.placeholder(tf.float32, [None, 1])
    ys = tf.placeholder(tf.float32, [None, 1])
    # 隐层1
    Weights1 = tf.Variable(tf.random_normal([1, 5]))
    biases1 = tf.Variable(tf.zeros([1, 5]) + 0.1)
    Wx_plus_b1 = tf.matmul(xs, Weights1) + biases1
    l1 = tf.nn.sigmoid(Wx_plus_b1)
    # 隐层2
    Weights2 = tf.Variable(tf.random_normal([5, 10]))
    biases2 = tf.Variable(tf.zeros([1, 10]) + 0.1)
    Wx_plus_b2 = tf.matmul(l1, Weights2) + biases2
    l2 = tf.nn.sigmoid(Wx_plus_b2)
    # 输出层
    Weights3 = tf.Variable(tf.random_normal([10, 1]))
    biases3 = tf.Variable(tf.zeros([1, 1]) + 0.1)
    prediction = tf.matmul(l2, Weights3) + biases3
    loss = tf.reduce_mean(tf.reduce_sum(tf.square(ys - prediction), reduction_indices=[1]))
    train_step = tf.train.AdamOptimizer(0.1).minimize(loss)
    # 初始化所有变量
    init = tf.global_variables_initializer()
    # 激活会话
    with tf.Session() as sess:
        sess.run(init)
        # 绘制原始x-y散点图。
        
        plt.rcParams['font.sans-serif'] = ['SimHei']
        plt.rcParams['axes.unicode_minus'] = False
        plt.xlabel(x_label)
        plt.ylabel(y_label)
        plt.title(title, fontsize=24)
        ax = fig.add_subplot(1, 1, 1)
        ax.scatter(x, y_data*10)
        plt.ion()
        plt.show()
        # 迭代次数 = 10000
        for i in range(10000):
            # 训练
            sess.run(train_step, feed_dict={xs: x, ys: y_data})
            # 每50步绘图并打印输出。
            if i % 50 == 0:
                # 可视化模型输出的结果。
                try:
                    ax.lines.remove(lines[0])
                    ax.lines.remove(lines2[0])
                except Exception:
                    pass
                prediction_value = sess.run(prediction, feed_dict={xs: x_data})
                # 绘制模型预测值。
                x1=x_data[-1]
                x2=x_data[-10]
                y1=prediction_value[-1]
                y2=prediction_value[-10]
                b=(x1*y2-x2*y1)/(y2-y1)
                b=min(max(x_data),b)
                b=max(min(x_data),b)
                lines2=ax.plot([x1,b],[y1*10,0],color,lw=1)
                lines = ax.plot(x_data, prediction_value*10, color, lw=3)
                plt.pause(1)
                # 打印损失
                result=sess.run(loss, feed_dict={xs: x, ys: y_data})
                print(result)
                if(result<=0.0057):
                    print('U0=',b[0])
                    print('\n\n')
                    break

fig = plt.figure()
x=[1.7594,1.7912,1.8126,1.8284,1.8442,1.8627,1.8707,1.8833,1.8951,1.9067,1.9186,1.9293,1.9390,1.9489,1.9567,1.9693,1.9778,1.9884,1.9957,2.004]
y=[0.5,1,1.5,2.0,2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.5,7.0,7.5,8.0,8.5,9.0,9.5,10.0]
plt.ylim(0,100)
curve(x,y,"U(V)","I(mA)","")
x=[2.212,2.258,2.291,2.314,2.337,2.357,2.375,2.393,2.411,2.427,2.441,2.454,2.469,2.483,2.498,2.511,2.522,2.533,2.546,2.554]
curve(x,y,"U(V)","I(mA)","",'g-')
x=[2.534,2.567,2.588,2.604,2.621,2.635,2.647,2.661,2.673,2.683,2.696,2.705,2.715,2.728,2.738,2.747,2.756,2.765,2.775,2.783]
curve(x,y,"U(V)","I(mA)","",'b-')

