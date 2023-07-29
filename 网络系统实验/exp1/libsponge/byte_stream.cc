#include "byte_stream.hh"

#include <algorithm>
#include <iterator>
#include <stdexcept>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) { //remember to complete the construction function
    DUMMY_CODE(capacity);
    _capacity = capacity;
}

size_t ByteStream::write(const string &data) {
    DUMMY_CODE(data);
    string _data;
    if(data.length()>remaining_capacity())
    {
        _data.assign(data.begin(),data.begin()+remaining_capacity());
    }
    else _data.assign(data);
    _write_count+=_data.length(); //note: if add this following the next statement, _data.length() will be 0.
    int ret=_data.length();
    _buffer.append(BufferList(move(_data)));
    return ret;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    DUMMY_CODE(len);
    int _len=len;
    if(len>_buffer.size()) _len=_buffer.size();
    string data=_buffer.concatenate().substr(0,_len);
    return data;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    DUMMY_CODE(len);
    int _len=len;
    if(len>_buffer.size()) _len=_buffer.size();
    _buffer.remove_prefix(_len);
    _read_count+=_len;
}

void ByteStream::end_input() {_input_ended_flag = true;}

bool ByteStream::input_ended() const { return _input_ended_flag; }

size_t ByteStream::buffer_size() const { return _buffer.size(); }

bool ByteStream::buffer_empty() const { return _buffer.size()==0; }

bool ByteStream::eof() const { return (_buffer.size()==0)&_input_ended_flag; }

size_t ByteStream::bytes_written() const { return _write_count; }

size_t ByteStream::bytes_read() const { return _read_count; }

size_t ByteStream::remaining_capacity() const { return _capacity - _buffer.size(); }
