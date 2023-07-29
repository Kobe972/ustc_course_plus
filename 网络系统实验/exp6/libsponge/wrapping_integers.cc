#include "wrapping_integers.hh"

// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
    DUMMY_CODE(n, isn);
    return isn + static_cast<uint32_t> (n);
}

uint64_t dist(uint64_t a, uint64_t b)
{
    if(a > b) return a - b;
    else return b - a;
}

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    DUMMY_CODE(n, isn, checkpoint);
    uint32_t offset = n - isn;
    uint64_t offset64 = static_cast<uint64_t>(offset) + (checkpoint & ((~0ull) - (1ull << 32) + 1ull));
    uint64_t candidate1 = offset64 - (1ull << 32);
    uint64_t candidate2 = offset64 + (1ull << 32);
    uint64_t candidate3 = offset64;
    uint64_t dist1 = dist(candidate1, checkpoint);
    uint64_t dist2 = dist(candidate2, checkpoint);
    uint64_t dist3 = dist(candidate3, checkpoint);
    if(offset64 < (1ull << 32))
    {
        if(dist2<dist3) return candidate2;
        else return candidate3;
    }
    else
    {
        if(dist1 < dist2) return dist1<dist3?candidate1:candidate3;
        else return dist2<dist3?candidate2:candidate3;
    }
}
