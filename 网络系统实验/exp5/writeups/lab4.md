Lab 4 Writeup
PB20000156  
徐亦昶
# 收到报文后_sender和_receiver的处理
## _sender
* 收到syn报文且还没有建立连接时，调用connect()，_sender发送syn建立连接。
* 在SYN_SENT阶段收到ack后，_sender要发送空包。
* 收到ack后，如果不是在LISTEN状态，则通知_sender。
* 如果数据包序列号异常或者收到rst，_sender都要发送空包。
* 在已建立连接的情况下，如果满足上述发送空包的条件，发送空包。
* _sender发送ack包（在代码实现中，发送ack包是在发送空包的函数中实现的）。
* _sender调用fill_window发送缓冲区中剩下的数据包。
## _receiver
* _receiver调用segment_received接收数据包，返回seqno的合法性。
# 结束TCP连接中主动关闭情况下，本端是否需要等待一段时间再关闭？
取决于本端是否有未读取的数据。如果本端的接收队列中有未读取的数据，那么本端会直接发送RST包给对端，然后立即关闭连接，不需要等待。如果本端的接收队列中没有未读取的数据，那么本端会发送FIN包给对端，然后进入FIN_WAIT_1状态，等待对端的ACK和FIN包，最后进入TIME_WAIT状态，等待一段时间后关闭连接。这个等待时间是为了保证对端能够收到最后一个ACK包，以及防止旧连接的报文段干扰新连接。
# active()函数的功能
判断连接是否还活跃。当连接不可用后active()会返回false，否则返回true。
# 遇到的问题及其解决过程
1. t_active_close在close Action后，希望发空包但没有发。主要问题在于不知道close到底操作了什么。我检测了一遍tcp_connection.hh中的函数，感觉最可疑的是end_input_stream，于是在这个函数结尾发送了空包，问题解决。
2. t_ack_rst中，发送了一个自己的数据包后提示少发了一个空包。查找资料得知当数据包的seqno不合理时，需要发送空包。于是在接收模块中增加了对seqno的检查，在它不在窗口范围内时设置flag以被tcp_connection检测到。
3. t_ack_rst中，对方在本端LISTEN阶段发送了ACK包，提示本端_sender状态应该是'waiting for stream to begin (no SYN sent)'，但实际上是'stream started but nothing acknowledged'。我在tcp_state中找到了关于各个状态的判断标准，发现是我在segment_received末尾调用了fill_window，于是在处于LISTEN状态时提前使用return退出。
4. t_listen中tick后提示状态不正确，因为_sender错误地发送了新包。经过仔细推断，发现在tcp_connection的tick函数中不应该调用_sender.fill_window()。
5. t_listen后面发送seqno=0的包以后，_reciever错误地认为其合法。于是做了一点修改，当已经建立连接且收到seqno=0的数据包后，同样发送空包（之前仅当数据包有数据时发送空包）。
6. 建立连接后无法发送数据，通过在每个函数调用时打印错误信息，发现出错点在remaining_outbound_capacity()计算错误，我返回的是_sender.stream_in().buffer_size()，实际上是_sender.stream_in().remaining_capacity()，这会导致上层误认为缓冲区已满。
7. 部分测试点clean_shutdown后_sender.stream_in()在某个时刻又新增了数据，导致eof返回0。我直接重写了_sender的重要函数，在_bytes_in_flight等地方做了优化（发送、接收包的时候维护而不是每次都遍历缓冲区），最后只剩五个点出现fail。
8. 尝试减少这些测试点的数据量，发现这时候就全部过了，于是猜想是序列号没有处理好导致溢出后出错，所以在receiver里面尽可能多地用了wrap，进行这样的修改以后测试点全部通过。
# 改进之处
增加实验3的测试样例，比如连续发送空包时，seqno应该是不变的。实验3的_receiver应该新增一个接口，返回上一个接受的数据包是否超出窗口限制。应该模拟实验4过程中发送的所有请求，保证实验3测试样例通过后在实验4中无需修改实验3的代码。