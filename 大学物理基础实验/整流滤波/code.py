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
        ax.scatter(x*2000, y_data)
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
                except Exception:
                    pass
                prediction_value = sess.run(prediction, feed_dict={xs: x_data})
                # 绘制模型预测值。
                lines = ax.plot(x_data*2000, prediction_value, color, lw=3)
                plt.pause(1)
                # 打印损失
                print(sess.run(loss, feed_dict={xs: x, ys: y_data}))

fig = plt.figure()
x=[0.01,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1]
y=[0.4198,0.3106,0.2204,0.1672,0.1339,0.1113,0.0951,0.0830,0.0737,0.0662,0.0601]
plt.ylim(0.05,0.45)
curve(x,y,"R(Ω)","Ku","")

