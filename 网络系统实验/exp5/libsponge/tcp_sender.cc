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
    return _bytes_in_flight;
}

void TCPSender::fill_window() {
    // if `_sending_end` has been set, the sender shouldn't send any new bytes
    if (_fin_flag) {
        return;
    }
    while (_window_free_size > 0 && !_fin_flag) {
        TCPSegment segment;
        TCPHeader &header = segment.header();
        if (next_seqno_absolute() == 0) {
            header.syn = true;
            --_window_free_size;
        }
        header.seqno = next_seqno();
        Buffer &buffer = segment.payload();
        buffer = stream_in().read(min(_window_free_size, TCPConfig::MAX_PAYLOAD_SIZE));
        if (stream_in().eof() && _window_free_size > buffer.size()) {
            header.fin = true;
            _fin_flag = true;
        }

        size_t len = segment.length_in_sequence_space();
        _window_free_size -= len;
        if (len == 0) {
            return;
        }

        _segments_out.push(segment);
        _segments_unresponded.push(segment);
        _next_seqno += len;
        _bytes_in_flight += len;
        if (!_timer_running) {
            _timer_running = true;
        }
    }
}

//! \param ackno The remote receiver's ackno (acknowledgment number)
//! \param window_size The remote receiver's advertised window size
bool TCPSender::ack_received(const WrappingInt32 ackno, const uint16_t window_size) {
    uint64_t abs_ackno = unwrap(ackno, _isn, next_seqno_absolute());
    // impossible ackno (beyond next seqno) should be ignored
    if (abs_ackno > next_seqno_absolute()) {
        return false;
    }
    _window_size = window_size;
    if(_window_size) _window_free_size = abs_ackno + _window_size - next_seqno_absolute();
    else _window_free_size = abs_ackno + 1 - next_seqno_absolute();

    bool successfully_received = false;
    while (!_segments_unresponded.empty()) {
        TCPSegment segment = _segments_unresponded.front();
        size_t len = segment.length_in_sequence_space();
        uint64_t seqno = unwrap(segment.header().seqno, _isn, next_seqno_absolute());
        if (seqno + len > abs_ackno) {
            break;
        }
        _segments_unresponded.pop();
        _bytes_in_flight -= len;
        successfully_received = true;
    }
    fill_window();
    if (successfully_received) {
        _retransmission_timeout = _initial_retransmission_timeout;
        _consecutive_retransmission = 0;
        if (!_segments_unresponded.empty()) {
            _timer_running = true;
            _timer = 0;
        } else {
            _timer_running = false;
        }
    }
    return true;
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void TCPSender::tick(const size_t ms_since_last_tick) {
    if(!_timer_running) return;
    _timer += ms_since_last_tick;
    if (_timer < _retransmission_timeout) {
        return;
    }
    _segments_out.push(_segments_unresponded.front());
    if (_window_size) {
        _consecutive_retransmission += 1;
        _retransmission_timeout <<= 1;
    }
    _timer_running = true;
    _timer = 0;
}

unsigned int TCPSender::consecutive_retransmissions() const { return _consecutive_retransmission; }

void TCPSender::send_empty_segment() {
    TCPSegment segment;
    segment.header().seqno = next_seqno();
    _segments_out.push(segment);
}
