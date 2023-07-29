#include "tcp_connection.hh"

#include <iostream>

// Dummy implementation of a TCP connection

// For Lab 4, please replace with a real implementation that passes the
// automated checks run by `make check`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

size_t TCPConnection::remaining_outbound_capacity() const { return _sender.stream_in().remaining_capacity(); }

size_t TCPConnection::bytes_in_flight() const { return _sender.bytes_in_flight(); }

size_t TCPConnection::unassembled_bytes() const { return _receiver.unassembled_bytes(); }

size_t TCPConnection::time_since_last_segment_received() const { return _timer; }

void TCPConnection::segment_received(const TCPSegment &seg) { 
    if (!_active)
        return;

    _timer = 0;
    if (TCPState::state_summary(_sender) == TCPSenderStateSummary::SYN_SENT) {
        if (seg.header().ack && seg.header().ackno == _sender.next_seqno() && seg.payload().size() > 0)
            return;
    }
    bool send_empty = false;
    if (_sender.next_seqno_absolute() > 0 && seg.header().ack) {
        if (!_sender.ack_received(seg.header().ackno, seg.header().win))
            send_empty = true;
    }

    if (!_receiver.segment_received(seg)) 
        send_empty = true;

    if (seg.header().syn && _sender.next_seqno_absolute() == 0) {
        connect();
        return;
    }

    if (seg.header().rst) {
        if (TCPState::state_summary(_sender) == TCPSenderStateSummary::SYN_SENT && !seg.header().ack) {
            return;
        }
        unclean_shutdown(false);
        return;
    }

    if (seg.length_in_sequence_space() > 0)
        send_empty = true; //send ack.

    if (send_empty) {
        if (_receiver.ackno().has_value() && _sender.segments_out().empty()) {
            _sender.send_empty_segment();
        }
    }
    send_all();
    clean_shutdown();
 }

bool TCPConnection::active() const { return _active; }

size_t TCPConnection::write(const string &data) {
    size_t ret = _sender.stream_in().write(data);
    _sender.fill_window();
    send_all();
    clean_shutdown();
    return ret;
}

//! \param[in] ms_since_last_tick number of milliseconds since the last call to this method
void TCPConnection::tick(const size_t ms_since_last_tick) { 
    if (!_active) {
        return;
    }
    _timer += ms_since_last_tick;
    _sender.tick(ms_since_last_tick);
    if (_sender.consecutive_retransmissions() > TCPConfig::MAX_RETX_ATTEMPTS) {
        unclean_shutdown(true);
    }
    send_all();
    clean_shutdown();
 }

void TCPConnection::end_input_stream() {
    _sender.stream_in().end_input();
    _sender.fill_window();
    send_all();
    clean_shutdown();
}

void TCPConnection::connect() {
    _sender.fill_window();
    send_all();
}

TCPConnection::~TCPConnection() {
    try {
        if (active()) {
            cerr << "Warning: Unclean shutdown of TCPConnection\n";
            unclean_shutdown(true);
            // Your code here: need to send a RST segment to the peer
        }
    } catch (const exception &e) {
        std::cerr << "Exception destructing TCP FSM: " << e.what() << std::endl;
    }
}

void TCPConnection::send_all() {
    TCPSegment seg;
    while (!_sender.segments_out().empty()) {
        seg = _sender.segments_out().front();
        _sender.segments_out().pop();
        if (_receiver.ackno().has_value()) {
            seg.header().ack = true;
            seg.header().ackno = _receiver.ackno().value();
            seg.header().win = _receiver.window_size();
        }
        _segments_out.push(seg);
    }
}

void TCPConnection::send_rst() {
    TCPSegment seg;
    while (!_sender.segments_out().empty()) {
        seg = _sender.segments_out().front();
        _sender.segments_out().pop();
        seg.header().rst = true;
        _segments_out.push(seg);
    }
}

void TCPConnection::unclean_shutdown(bool rst) {
    _receiver.stream_out().set_error();
    _sender.stream_in().set_error();
    _linger_after_streams_finish = false;
    _active = false;
    if (rst) {
        if (_sender.segments_out().empty()) {
            _sender.send_empty_segment();
        }
        send_rst();
    }
}

void TCPConnection::clean_shutdown() {
    if (_receiver.stream_out().input_ended() && !(_sender.stream_in().eof())) {
        _linger_after_streams_finish = false;
    }
    if (_sender.stream_in().eof() && _sender.bytes_in_flight() == 0 && _receiver.stream_out().input_ended()) {
        if (!_linger_after_streams_finish || time_since_last_segment_received() >= 10 * _cfg.rt_timeout) {
            _active = false;
        }
    }
}