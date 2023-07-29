#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

bool TCPReceiver::segment_received(const TCPSegment &seg) {
    DUMMY_CODE(seg);
    static size_t abs_seqno = 0;
    if(seg.header().syn)
    {
        if(_syn_flag) return false;
        _isn = seg.header().seqno.raw_value();
        _base = _isn + 1;
        _syn_flag=true;
    }
    else if(!_syn_flag)
    {
        return false;
    }
    else
    {
        if(seg.header().ack) return true;
        if(seg.header().fin) _fin_flag=true;
        abs_seqno = unwrap(WrappingInt32(seg.header().seqno.raw_value()), WrappingInt32(_isn), abs_seqno);
        if(abs_seqno + _isn + seg.length_in_sequence_space() <= _base || abs_seqno + _isn >= _base + _capacity) return false;
        _reassembler.push_substring(seg.payload().copy(),abs_seqno - 1,seg.header().fin);
        _base = seg.header().fin?_reassembler.head_index() + _isn + 2:_reassembler.head_index() + _isn + 1;
    }
    if(seg.header().fin&&seg.header().syn)
    {
        _fin_flag=true;
        _reassembler.push_substring(seg.payload().copy(),0,true);
        _base = _reassembler.head_index() + _isn + 2;
    }
    return true;
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    if(!_syn_flag) return {};
    return WrappingInt32{static_cast<uint32_t>(_base)};
}

size_t TCPReceiver::window_size() const {
    return _capacity - _reassembler.stream_out().buffer_size();
}
