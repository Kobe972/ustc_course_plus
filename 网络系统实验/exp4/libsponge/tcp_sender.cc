#include "tcp_sender.hh"

#include "tcp_config.hh"

#include <random>
#include <iostream>

// Dummy implementation of a TCP sender

// For Lab 3, please replace with a real implementation that passes the
// automated checks run by `make check_lab3`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! \param[in] capacity the capacity of the outgoing byte stream
//! \param[in] retx_timeout the initial amount of time to wait before retransmitting the oldest outstanding segment
//! \param[in] fixed_isn the Initial Sequence Number to use, if set (otherwise uses a random ISN)
TCPSender::TCPSender(const size_t capacity, const uint16_t retx_timeout, const std::optional<WrappingInt32> fixed_isn)
    : _isn(fixed_isn.value_or(WrappingInt32{random_device()()}))
    , _initial_retransmission_timeout{retx_timeout},
    _retransmission_timeout{retx_timeout},
    _stream(capacity) {}

uint64_t TCPSender::bytes_in_flight() const { 
    uint64_t ret = 0;
    for(auto item:_segments_unresponded)
    {
        ret += item.length_in_sequence_space();
    }
    return ret;
}

void TCPSender::fill_window() {
    if(_stream.input_ended() && _window_size >= 1 && !_fin_flag)
    {
        TCPHeader header;
        header.seqno = _isn + _next_seqno;
        header.win = _window_size;
        header.fin = true;
        Buffer payload(_stream.peek_output(_stream.buffer_size()));
        _stream.pop_output(_stream.buffer_size()); //remember to pop
        TCPSegment segment;
        segment.header() = header;
        segment.payload() = payload;
        _segments_out.push(segment);
        _segments_unresponded.push_back(segment);
        _next_seqno += 1;
        _fin_flag = _stream.eof();
        _timer_running = true;
        return;
    }
    else if(_stream.eof())
    {
        cout<<"return"<<endl;
        return;
    }
    if(!_syn_flag) //send SYN
    {
        TCPHeader header;
        header.seqno = _isn;
        header.syn = true;
        header.win = _window_size;
        Buffer payload{};
        TCPSegment segment;
        segment.header() = header;
        segment.payload() = payload;
        _segments_out.push(segment);
        _segments_unresponded.push_back(segment);
        _next_seqno = 1;
        _syn_flag = true;
        _timer_running = true;
    }
    else
    {
        while(1)
        {
            if(_stream.buffer_empty() || _window_size == 0) break;
            TCPHeader header;
            header.seqno = _isn + _next_seqno;
            header.win = _window_size;
            size_t len = _stream.buffer_size();
            if(len > TCPConfig::MAX_PAYLOAD_SIZE) len = TCPConfig::MAX_PAYLOAD_SIZE;
            if(len > _window_size) len = _window_size;
            Buffer payload(_stream.peek_output(len));
            _stream.pop_output(len);
            TCPSegment segment;
            segment.header() = header;
            segment.payload() = payload;
            _segments_out.push(segment);
            _segments_unresponded.push_back(segment);
            _next_seqno += segment.length_in_sequence_space();
            _window_size -= segment.length_in_sequence_space();
            _timer_running = true;
        }
    }
}

//! \param ackno The remote receiver's ackno (acknowledgment number)
//! \param window_size The remote receiver's advertised window size
//! \returns `false` if the ackno appears invalid (acknowledges something the TCPSender hasn't sent yet)
bool TCPSender::ack_received(const WrappingInt32 ackno, const uint16_t window_size) {
    DUMMY_CODE(ackno, window_size);
    static uint64_t last_received = 0;
    _window_size = window_size;
    uint64_t abs_askno = unwrap(ackno, _isn, last_received);
    last_received = abs_askno;
    if(abs_askno == 1) _syn_flag = true;
    vector<TCPSegment>::iterator it;
    bool legal = false;
    bool successfully_received = false;
    for(it = _segments_unresponded.begin();it != _segments_unresponded.end();)
    {
        if(!legal && (*it).header().seqno.raw_value() + (*it).payload().size() >= abs_askno + _isn.raw_value() - 1) legal = true;
        if((*it).header().seqno.raw_value() < abs_askno + _isn.raw_value())
        {
            it = _segments_unresponded.erase(it);
            successfully_received = true;
        }
        else it++;
    }
    fill_window();
    if(successfully_received)
    {
        _timer = 0;
        _retransmission_timeout = _initial_retransmission_timeout;
        _consecutive_retransmission = 0;
    }
    if(_segments_unresponded.empty()) _timer_running = false;
    return legal;
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void TCPSender::tick(const size_t ms_since_last_tick) {
    cout<<TCPConfig::TIMEOUT_DFLT<<" "<<ms_since_last_tick<<endl;
    DUMMY_CODE(ms_since_last_tick);
    if(!_timer_running) return;
    _timer += ms_since_last_tick;
    if(_timer >= _retransmission_timeout && !_segments_unresponded.empty())
    {
        TCPSegment resend = _segments_unresponded[0];
        uint32_t lowest_seqno = _segments_unresponded[0].header().seqno.raw_value();
        for(auto segment:_segments_unresponded)
        {
            if(segment.header().seqno.raw_value() < lowest_seqno)
            {
                lowest_seqno = segment.header().seqno.raw_value();
                resend = segment;
            }
        }
        _segments_out.push(resend);
        if(_window_size > 0)
        {
            _consecutive_retransmission++;
            _retransmission_timeout *= 2;
        }
        _timer = 0;
    }
}

unsigned int TCPSender::consecutive_retransmissions() const { return _consecutive_retransmission; }

void TCPSender::send_empty_segment() {
    if(_window_size == 0) return;
    TCPHeader header;
    header.seqno = _isn + _next_seqno;
    header.win = _window_size;
    Buffer payload("");
    TCPSegment segment;
    segment.header() = header;
    segment.payload() = payload;
    _segments_out.push(segment);
    _segments_unresponded.push_back(segment);
    _next_seqno += 1;
    _window_size -= 1;
}
