#pragma once
#include <string>

/**
 * Dynamically sized bit array.
 * Bit 0 is the least significant one. to_string() prints the highest index first.
 * Invalid index -> std::out_of_range.
 * Invalid size or negative shift, or operands of different sizes -> std::invalid_argument.
 */
class BitArray {
private:
    unsigned long* data;
    int numbits;
    void clear_tail();   // zeroes unused bits of the last word

public:
    /// Creates an empty array (size 0).
    BitArray();
    /// Releases the memory.
    ~BitArray();

    /**
     * Creates an array of num_bits bits. The lowest bits are taken from value;
     * bits of value beyond num_bits are discarded, the rest are zeros.
     * throws std::invalid_argument if num_bits < 0
     */
    explicit BitArray(int num_bits, unsigned long value = 0);
    /// Creates a deep copy of b.
    BitArray(const BitArray& b);

    /// Swaps contents with b. Never throws.
    void swap(BitArray& b);
    /// Deep copy assignment. Safe for self-assignment.
    BitArray& operator=(const BitArray& b);

    /**
     * Changes the size. Existing bits are kept (up to the new size),
     * new bits are set to value.
     * throws std::invalid_argument if num_bits < 0
     */
    void resize(int num_bits, bool value = false);
    /// Makes the array empty (size 0).
    void clear();
    /// Appends one bit to the end; size grows by 1.
    void push_back(bool bit);

    /**
     * Bitwise AND / OR / XOR with b, in place.
     * throws std::invalid_argument if sizes differ
     *         (the operation is defined only for equal sizes)
     */
    BitArray& operator&=(const BitArray& b);
    BitArray& operator|=(const BitArray& b);
    BitArray& operator^=(const BitArray& b);

    /**
     * Shifts bits by n positions (<< towards higher indexes, >> towards lower),
     * vacated bits are filled with zeros, size does not change.
     * If n >= size() the result is all zeros.
     * throws std::invalid_argument if n < 0
     */
    BitArray& operator<<=(int n);
    BitArray& operator>>=(int n);
    /// Same as above, but returns a new array and leaves *this unchanged.
    BitArray operator<<(int n) const;
    BitArray operator>>(int n) const;

    /**
     * Sets bit n to val.
     * throws std::out_of_range if n < 0 or n >= size()
     */
    BitArray& set(int n, bool val = true);
    /// Sets all bits to true.
    BitArray& set();

    /**
     * Sets bit n to false.
     * throws std::out_of_range if n < 0 or n >= size()
     */
    BitArray& reset(int n);
    /// Sets all bits to false.
    BitArray& reset();

    /// True if at least one bit is set.
    bool any() const;
    /// True if no bit is set (also true for an empty array).
    bool none() const;
    /// Returns a bitwise inverted copy.
    BitArray operator~() const;
    /// Number of set bits.
    int count() const;

    /**
     * Returns the value of bit i.
     * throws std::out_of_range if i < 0 or i >= size()
     */
    bool operator[](int i) const;

    /// Number of bits.
    int size() const;
    /// True if size() == 0.
    bool empty() const;

    /// String of '0'/'1', highest index first (e.g. size 4, value 5 -> "0101").
    std::string to_string() const;
};

/// Equal if sizes are equal and all bits are equal.
bool operator==(const BitArray& a, const BitArray& b);
bool operator!=(const BitArray& a, const BitArray& b);

/// Bitwise operations returning a new array. throws std::invalid_argument if sizes differ.
BitArray operator&(const BitArray& b1, const BitArray& b2);
BitArray operator|(const BitArray& b1, const BitArray& b2);
BitArray operator^(const BitArray& b1, const BitArray& b2);