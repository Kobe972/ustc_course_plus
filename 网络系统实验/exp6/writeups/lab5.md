Lab 5 Writeup
徐亦昶  
PB20000156
# p1
在`tap.sh`中，通过`ip tuntap`命令创建虚拟网卡。这些网卡都被命名为`tapN`（其中N为整数）。每个网卡都绑定到了一个独特的MAC地址，并在IP层级分配了一个唯一的IP地址。

当虚拟机发出一个网络包时，包首先会经过操作系统内核的网络协议栈，其中包括 TCP/IP 层和网络接口层。在网络接口层，网络包首先被封装成一个以太网帧，该帧包含源和目标MAC地址。在这里，源MAC地址为本地MAC地址，而目标MAC地址为接收方MAC地址，通常是虚拟机的 MAC 地址。

这个以太网帧会被发送到虚拟机的虚拟网卡`tapN`。在接收到网络包时，tap设备会把这个以太网帧从内核态传输到用户态，然后将以太网帧的负载（即IP层以下的报文）传递给手写程序进行处理。

在手写的协议栈中，可以使用`read()`系统调用从tap设备读取数据包。读取的数据包的格式与标准以太网帧相同。然后可以对其进行解析，并处理它所包含的网络数据。此后，可以使用`write()`系统调用将处理后的数据写回到tap设备中，以便继续进行传输。数据将沿着虚拟网络卡`tapN`被发送到网络接口层，再由网络协议栈处理并转发到目的地。
# p2
遇到的问题：ARP测试点2出现`pending mappings last five seconds`，查询ppt发现同一个ARP请求5秒内不会重复发送。因此遇到了本实验的难点：处理ARP请求的重复发送问题。

首先在.hh文件中创建新变量_ip_time_table，它是一个map，记录了待重发ARP包目标IP和上一次发送时间的对应关系。

在tick()中，每次更新计时器都要检查一下_ip_time_table，删除超时的项。在send_datagram中，每次需要发ARP请求前，先检查_ip_time_table，如果有项说明还没超时，否则发送该请求。
# p3
FullStackSocket是一个自己实现的类，用于从网络中获取数据。它基于底层的系统调用实现了一个包含了多个协议层的完整的TCP/IP协议栈。

FullStackSocket与CS144TCPSocket的区别在于，CS144TCPSocket只实现了TCP协议，而FullStackSocket实现了整个TCP/IP协议栈。在获取网络数据时，FullStackSocket会处理链路层、网络层、传输层和应用层协议，并将这些层次的信息全部暴露给调用者。这意味着，通过FullStackSocket获取的数据不仅包含了TCP协议的数据，还包括了以太网头、IP头和TCP头等信息。简单地说，FullStackSocket实现了链路层。在代码实现中，可以看到CS144TCPSocket的connect()函数返回的是TCPOverIPv4SpongeSocket类，而另外一个返回的是TCPOverIPv4OverEthernetSpongeSocket类，相比前者多了“Ethernet”。

总的来说，FullStackSocket提供了更大的灵活性和更细粒度的控制，而CS144TCPSocket则提供了更高的易用性和更少的复杂度。
# p4
暂无改进之处。