#include "BitArray.h"
#include <stdexcept>
#include <utility>

namespace {
    const int BITS_PER_WORD=sizeof(unsigned long)*8;
    int words_for(int num_bits) {
        return (num_bits+BITS_PER_WORD-1)/BITS_PER_WORD;
    }
}

BitArray::BitArray(int num_bits, unsigned long value) {
    if (num_bits < 0) {
        throw std::invalid_argument("num_bits must be non-negative");
    }
    numbits = num_bits;
    int words = words_for(num_bits);
    if  (words >0) {
        data=new unsigned long[words]();
        data[0]=value;
        if (numbits % BITS_PER_WORD != 0) {
            int used = numbits % BITS_PER_WORD;
            data[words-1]&=(1UL<<used)-1;
        }
    }
    else {
        data=nullptr;
    }
}

BitArray::~BitArray() {
    delete [] data;
}

int BitArray::size() const{
    return numbits;
}

bool BitArray::empty() const {
    return numbits==0;
}

BitArray::BitArray(const BitArray& b) : numbits(b.numbits) {
    int words = words_for(numbits);
    if  (words >0) {
        data=new unsigned long[words]();
    }
    else {
        data=nullptr;
    }
    for (int i=0;i<words;i++) {
        data[i]=b.data[i];
    }
}

void BitArray::swap(BitArray& b) {
    std::swap(numbits,b.numbits);
    std::swap(data,b.data);
}

BitArray& BitArray::operator=(const BitArray& b) {
    BitArray tmp(b);
    swap(tmp);
    return *this;
}

void BitArray::clear() {
    delete [] data;
    numbits = 0;
    data=nullptr;
}

void BitArray::resize(int num_bits, bool value) {
    if (num_bits < 0) {
        throw std::invalid_argument("num_bits must be non-negative");
    }
    BitArray tmp(num_bits);

    int to_copy = (numbits < num_bits) ? numbits : num_bits;
    for (int i = 0; i < to_copy; i++) {
        tmp.set(i, (*this)[i]);
    }
    if (value) {
        for (int i = numbits; i < num_bits; i++) {
            tmp.set(i, true);
        }
    }
    swap(tmp);
}

BitArray::BitArray() : data(nullptr), numbits(0) {}

void BitArray::clear_tail() {
    if (numbits % BITS_PER_WORD != 0) {
        int used = numbits % BITS_PER_WORD;
        data[words_for(numbits) - 1] &= (1UL << used) - 1;
    }
}

void BitArray::push_back(bool bit) {
    resize(numbits + 1);
    set(numbits - 1, bit);
}

BitArray& BitArray::operator&=(const BitArray& b) {
    if (numbits != b.numbits) {
        throw std::invalid_argument("sizes differ");
    }
    int words = words_for(numbits);
    for (int i = 0; i < words; i++) {
        data[i] &= b.data[i];
    }
    return *this;
}

BitArray& BitArray::operator|=(const BitArray& b) {
    if (numbits != b.numbits) {
        throw std::invalid_argument("sizes differ");
    }
    int words = words_for(numbits);
    for (int i = 0; i < words; i++) {
        data[i] |= b.data[i];
    }
    return *this;
}

BitArray& BitArray::operator^=(const BitArray& b) {
    if (numbits != b.numbits) {
        throw std::invalid_argument("sizes differ");
    }
    int words = words_for(numbits);
    for (int i = 0; i < words; i++) {
        data[i] ^= b.data[i];
    }
    return *this;
}

BitArray& BitArray::operator<<=(int n) {
    if (n < 0) {
        throw std::invalid_argument("shift must be non-negative");
    }
    if (n >= numbits) {
        reset();
        return *this;
    }
    for (int i = numbits - 1; i >= n; i--) {
        set(i, (*this)[i - n]);
    }
    for (int i = 0; i < n; i++) {
        reset(i);
    }
    return *this;
}

BitArray& BitArray::operator>>=(int n) {
    if (n < 0) {
        throw std::invalid_argument("shift must be non-negative");
    }
    if (n >= numbits) {
        reset();
        return *this;
    }
    for (int i = 0; i < numbits - n; i++) {
        set(i, (*this)[i + n]);
    }
    for (int i = numbits - n; i < numbits; i++) {
        reset(i);
    }
    return *this;
}

BitArray BitArray::operator<<(int n) const {
    BitArray result(*this);
    result <<= n;
    return result;
}

BitArray BitArray::operator>>(int n) const {
    BitArray result(*this);
    result >>= n;
    return result;
}

BitArray& BitArray::set(int n, bool val) {
    if (n < 0 || n >= numbits) {
        throw std::out_of_range("index out of range");
    }
    if (val) {
        data[n / BITS_PER_WORD] |= (1UL << (n % BITS_PER_WORD));
    } else {
        data[n / BITS_PER_WORD] &= ~(1UL << (n % BITS_PER_WORD));
    }
    return *this;
}

BitArray& BitArray::set() {
    int words = words_for(numbits);
    for (int i = 0; i < words; i++) {
        data[i] = ~0UL;
    }
    clear_tail();
    return *this;
}

BitArray& BitArray::reset(int n) {
    return set(n, false);
}

BitArray& BitArray::reset() {
    int words = words_for(numbits);
    for (int i = 0; i < words; i++) {
        data[i] = 0;
    }
    return *this;
}

bool BitArray::any() const {
    int words = words_for(numbits);
    for (int i = 0; i < words; i++) {
        if (data[i] != 0) {
            return true;
        }
    }
    return false;
}

bool BitArray::none() const {
    return !any();
}

BitArray BitArray::operator~() const {
    BitArray result(*this);
    int words = words_for(numbits);
    for (int i = 0; i < words; i++) {
        result.data[i] = ~result.data[i];
    }
    result.clear_tail();
    return result;
}

int BitArray::count() const {
    int total = 0;
    int words = words_for(numbits);
    for (int i = 0; i < words; i++) {
        unsigned long w = data[i];
        while (w != 0) {
            w &= (w - 1);
            total++;
        }
    }
    return total;
}

bool BitArray::operator[](int i) const {
    if (i < 0 || i >= numbits) {
        throw std::out_of_range("index out of range");
    }
    return (data[i / BITS_PER_WORD] >> (i % BITS_PER_WORD)) & 1UL;
}

std::string BitArray::to_string() const {
    std::string s;
    for (int i = numbits - 1; i >= 0; i--) {
        s += (*this)[i] ? '1' : '0';
    }
    return s;
}

bool operator==(const BitArray& a, const BitArray& b) {
    if (a.size() != b.size()) {
        return false;
    }
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const BitArray& a, const BitArray& b) {
    return !(a == b);
}

BitArray operator&(const BitArray& b1, const BitArray& b2) {
    BitArray result(b1);
    result &= b2;
    return result;
}

BitArray operator|(const BitArray& b1, const BitArray& b2) {
    BitArray result(b1);
    result |= b2;
    return result;
}

BitArray operator^(const BitArray& b1, const BitArray& b2) {
    BitArray result(b1);
    result ^= b2;
    return result;
}