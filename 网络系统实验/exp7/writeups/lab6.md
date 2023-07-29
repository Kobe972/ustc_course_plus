Lab 6 Writeup
=============

PB20000156  
徐亦昶

# P1
发送：首先调用`network.host(...).sent_to(...)`实现数据的发送，再调用`simulate()`完成路由器内部的路由（调用了route函数）和路由器与外部的数据交换。在该函数结束后会检查数据是否正确到达。

接收：在`simulate()`进行路由器与外部的数据交换的过程中，调用了`exchange_frames`函数，这个函数又调用了`deliver`，`deliver`中使用`recv_frame`进行数据的接收。
# P2
我的实现中没有显式记录子网掩码。我使用string来标记每个节点的二进制字符串。
# P3
* 应该正常路由的时候没有路由。原因是在`route_one_datagram`中，忽略了前缀长度为0的情况。这种情况下，应该路由到查找失败的接口而不是直接退出。
* 输出中间结果显示正确路由，send_datagram也显示已发送数据，但是对面就是没有收到。为了解决这个问题，我在`_interface.send_datagram`后面输出了`_interface`已发送的数据包个数，发现并没有改变。再仔细检查这一部分代码，发现`AsyncNetworkInterface& _interface = interface(selected_item.interface_num);`这条语句中没有加引用，这就导致接口状态实际上并没有改变。
# P4
应该给出RadixTree的伪代码。