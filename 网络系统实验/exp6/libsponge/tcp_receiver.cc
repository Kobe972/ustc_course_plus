#include "tcp_receiver.hh"
#include "tcp_state.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

bool TCPReceiver::segment_received(const TCPSegment &seg) {

    if(seg.header().syn)
    {
        if(_syn_flag) return false;
        _isn = seg.header().seqno;
        _ackno = 1;
        _last_seqno = 0;
        _syn_flag=true;
        if (seg.length_in_sequence_space() == 1) {
            return true;
        }
    }
    else if(!_syn_flag)
    {
        return false;
    }

    if (stream_out().input_ended()) {
        if (seg.length_in_sequence_space() == 0 && unwrap(seg.header().seqno, _isn, _last_seqno) == _ackno)
            return true;
        return false;
    }
    uint64_t seqno = unwrap(seg.header().seqno, _isn, _last_seqno);
    if (!seg.header().syn) {
        size_t len = seg.length_in_sequence_space();
        if(seqno + len <= _ackno || seqno >= _ackno + window_size())
        {
            if(len == 0)
            {
                if((seqno < _ackno || seqno >= _ackno + window_size()) && window_size()) return false;
                else
                {
                    _last_seqno = seqno;
                    return true;
                }
            }
            else return false;
        }
    }
    _last_seqno = seqno;
    size_t tmp = stream_out().bytes_written();
    if(seg.header().syn) _reassembler.push_substring(seg.payload().copy(), seqno, seg.header().fin);
    else _reassembler.push_substring(seg.payload().copy(), seqno - 1, seg.header().fin);
    _ackno += (stream_out().bytes_written() - tmp);
    if (stream_out().input_ended()) {
        _ackno++;
    }

    return true;
}

optional<WrappingInt32> TCPReceiver::ackno() const { 
    if(!_syn_flag) return {};
    return wrap(_ackno, _isn);
 }

size_t TCPReceiver::window_size() const { return _capacity - stream_out().buffer_size(); }
