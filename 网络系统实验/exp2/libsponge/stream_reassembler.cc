#include "stream_reassembler.hh"
#include <iostream>

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity, const size_t head_index) : _output(capacity), _capacity(capacity), _head_index(head_index) {
    _output = ByteStream(capacity);
}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    DUMMY_CODE(data, index, eof);
    if (index >= _head_index + _capacity || index + data.length() > _head_index + _capacity){
        return;
    }
    block_node elm;
    if (index<_head_index && index + data.length() >= _head_index)
    {
        elm.data = data.substr(_head_index-index,data.length()-_head_index+index);
        elm.begin = _head_index;
        elm.length = elm.data.length();
    }
    else if(index<_head_index) return;
    else
    {
        elm.data = data;
        elm.begin = index;
        elm.length = elm.data.length();
    }
    _unassembled_byte += elm.length;
    //merge_next
    long merged_bytes = 0;
    auto iter = _blocks.lower_bound(elm);
    while(iter != _blocks.end() && (merged_bytes = merge_block(elm, *iter)) >= 0)
    {
        _unassembled_byte -= merged_bytes;
        _blocks.erase(iter);
        iter = _blocks.lower_bound(elm);
    }
    //merge_pred
    iter = _blocks.lower_bound(elm);
    while(iter != _blocks.begin() && (merged_bytes = merge_block(elm, *prev(iter))) >= 0)
    {
        iter--;
        _unassembled_byte -= merged_bytes;
        _blocks.erase(iter);
        iter = _blocks.lower_bound(elm);
    }
    _blocks.insert(elm);
    for(auto block = _blocks.begin(); block != _blocks.end();)
    {
        if(block->begin>_head_index) break;
        _output.write(block->data);
        _unassembled_byte -= block->length;
        _head_index += block->length;
        block = _blocks.erase(block);
    }
    if(eof) _eof=true;
    if(_eof==true && empty())
    {
        _output.end_input();
    }
}

size_t StreamReassembler::unassembled_bytes() const { return _unassembled_byte; }

bool StreamReassembler::empty() const { return _blocks.empty(); }

long StreamReassembler::merge_block(block_node &elm1, const block_node &elm2)
{
    block_node x,y;
    if(elm1.begin > elm2.begin)
    {
        x = elm2;
        y = elm1;
    }
    else if(elm1.begin == elm2.begin)
    {
        if(elm1.length < elm2.length)
        {
            x=elm1;
            y=elm2;
        }
        else
        {
            x=elm2;
            y=elm1;
        }
    }
    else
    {
        x = elm1;
        y = elm2;
    }
    if(x.begin+x.length<y.begin) return -1;
    if(x.begin+x.length==y.begin)
    {
        x.data+=y.data;
        x.length+=y.length;
        elm1=x;
        return 0;
    }
    else if(x.begin==y.begin && x.length == y.length) return x.length;
    else if(x.begin+x.length>y.begin+y.length) //y in x
    {
        elm1=x;
        return y.length;
    }
    else
    {
        x.data.assign(x.data.substr(0,y.begin-x.begin));
        x.data+=y.data;
        size_t ret = x.begin+x.length-y.begin;
        x.length=y.begin-x.begin+y.length;
        elm1=x;
        return ret;
    }
}
