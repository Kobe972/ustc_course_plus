#include "network_interface.hh"

#include "arp_message.hh"
#include "ethernet_frame.hh"

#include <iostream>

// Dummy implementation of a network interface
// Translates from {IP datagram, next hop address} to link-layer frame, and from link-layer frame to IP datagram

// For Lab 5, please replace with a real implementation that passes the
// automated checks run by `make check_lab5`.

// You will need to add private members to the class declaration in `network_interface.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! \param[in] ethernet_address Ethernet (what ARP calls "hardware") address of the interface
//! \param[in] ip_address IP (what ARP calls "protocol") address of the interface
NetworkInterface::NetworkInterface(const EthernetAddress &ethernet_address, const Address &ip_address)
    : _ethernet_address(ethernet_address), _ip_address(ip_address) {
    cerr << "DEBUG: Network interface has Ethernet address " << to_string(_ethernet_address) << " and IP address "
         << ip_address.ip() << "\n";
}

//! \param[in] dgram the IPv4 datagram to be sent
//! \param[in] next_hop the IP address of the interface to send it to (typically a router or default gateway, but may also be another host if directly connected to the same network as the destination)
//! (Note: the Address type can be converted to a uint32_t (raw 32-bit IP address) with the Address::ipv4_numeric() method.)
void NetworkInterface::send_datagram(const InternetDatagram &dgram, const Address &next_hop) {
    // convert IP address of next hop to raw 32-bit representation (used in ARP header)
    const uint32_t next_hop_ip = next_hop.ipv4_numeric();

    DUMMY_CODE(dgram, next_hop, next_hop_ip);
    EthernetFrame frame;
    frame.header().type = EthernetHeader::TYPE_IPv4;
    frame.header().src = _ethernet_address;
    frame.payload() = BufferList{dgram.serialize().concatenate()};
    if(_arp_table.count(next_hop_ip) && _timer <= _ttl[next_hop_ip])
    {
        frame.header().dst = _arp_table[next_hop_ip];
        _frames_out.push(frame);
    }
    else
    {
        ARPMessage msg;
        msg.opcode = ARPMessage::OPCODE_REQUEST;
        msg.sender_ethernet_address = _ethernet_address;
        msg.sender_ip_address = _ip_address.ipv4_numeric();
        msg.target_ip_address = next_hop_ip;
        msg.target_ethernet_address = {0, 0, 0, 0, 0, 0};
        EthernetFrame arp_frame;
        arp_frame.header().type = EthernetHeader::TYPE_ARP;
        arp_frame.header().src = _ethernet_address;
        arp_frame.header().dst = ETHERNET_BROADCAST;
        arp_frame.payload() = move(msg.serialize());
        if(!_ip_time_table.count(next_hop_ip))
        {
            _frames_out.push(arp_frame);
            _ip_time_table[next_hop_ip] = _timer;
        }
        _outstanding_frames.push(OutstandingFrame{frame,next_hop_ip});
    }
}

//! \param[in] frame the incoming Ethernet frame
optional<InternetDatagram> NetworkInterface::recv_frame(const EthernetFrame &frame) {
    DUMMY_CODE(frame);
    if(frame.header().dst != _ethernet_address && frame.header().dst != ETHERNET_BROADCAST)
    {
        return {};
    }
    if(frame.header().type == EthernetHeader::TYPE_IPv4)
    {
        InternetDatagram result;
        if(result.parse(frame.payload())==ParseResult::NoError)
        {
            return result;
        }
    }
    if(frame.header().type == EthernetHeader::TYPE_ARP)
    {
        ARPMessage msg;
        if(msg.parse(frame.payload())==ParseResult::NoError)
        {
            uint32_t ip = msg.sender_ip_address;
            _arp_table[ip] = msg.sender_ethernet_address;
            _ttl[ip] = _timer + 30 * 1000;
            if(_ip_time_table.count(ip)) _ip_time_table.erase(ip);
            if(msg.opcode == ARPMessage::OPCODE_REQUEST && msg.target_ip_address == _ip_address.ipv4_numeric())
            {
                ARPMessage reply;
                reply.opcode = ARPMessage::OPCODE_REPLY;
                reply.sender_ethernet_address = _ethernet_address;
                reply.sender_ip_address = _ip_address.ipv4_numeric();
                reply.target_ethernet_address = msg.sender_ethernet_address;
                reply.target_ip_address = msg.sender_ip_address;
                EthernetFrame arp_frame;
                arp_frame.header().type = EthernetHeader::TYPE_ARP;
                arp_frame.header().src = _ethernet_address;
                arp_frame.header().dst = msg.sender_ethernet_address;
                arp_frame.payload() = move(reply.serialize());
                _frames_out.push(arp_frame);
            }
            queue<OutstandingFrame> backup;
            while (!_outstanding_frames.empty())
            {
                OutstandingFrame outstanding_frame = _outstanding_frames.front();
                if(_arp_table.count(outstanding_frame.next_hop_ip)&&_timer <= _ttl[outstanding_frame.next_hop_ip])
                {
                    outstanding_frame.frame.header().dst = _arp_table[outstanding_frame.next_hop_ip];
                    _outstanding_frames.pop();
                    _frames_out.push(std::move(outstanding_frame.frame));
                }
                else
                {
                    _outstanding_frames.pop();
                    backup.push(std::move(outstanding_frame));
                }
            }
            _outstanding_frames = std::move(backup);
        }
    }
    return {};
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void NetworkInterface::tick(const size_t ms_since_last_tick) {
    DUMMY_CODE(ms_since_last_tick);
    _timer += ms_since_last_tick;
    std::queue<uint32_t> timeout_ips;
    map<uint32_t, size_t>::iterator ite;
    for(ite = _ip_time_table.begin(); ite != _ip_time_table.end(); ite++)
    {
        if(_timer - ite->second >= 5000) timeout_ips.push(ite->first);
    }
    while(!timeout_ips.empty())
    {
        _ip_time_table.erase(timeout_ips.front());
        timeout_ips.pop();
    }
}