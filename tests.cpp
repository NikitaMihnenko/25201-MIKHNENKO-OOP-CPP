#include <gtest/gtest.h>
#include <stdexcept>
#include "BitArray.h"

static const int W = sizeof(unsigned long) * 8;

// ---------- constructors, size ----------
TEST(BitArray, DefaultIsEmpty) {
    BitArray a;
    EXPECT_EQ(a.size(), 0);
    EXPECT_TRUE(a.empty());
    EXPECT_EQ(a.to_string(), "");
}

TEST(BitArray, SizeConstructor) {
    BitArray a(10);
    EXPECT_EQ(a.size(), 10);
    EXPECT_FALSE(a.empty());
    EXPECT_TRUE(a.none());
}

TEST(BitArray, ValueConstructor) {
    BitArray a(4, 5);
    EXPECT_EQ(a.to_string(), "0101");
}

TEST(BitArray, ValueIsTruncatedToSize) {
    BitArray a(4, 255);
    EXPECT_EQ(a.count(), 4);
}

TEST(BitArray, ValueWiderThanWordKeepsOnlyWord) {
    BitArray a(W + 6, ~0UL);
    EXPECT_EQ(a.count(), W);
}

TEST(BitArray, ZeroSizeWithValue) {
    BitArray a(0, 5);
    EXPECT_TRUE(a.empty());
}

TEST(BitArray, NegativeSizeThrows) {
    EXPECT_THROW(BitArray a(-1), std::invalid_argument);
}

TEST(BitArray, SizeMultipleOfWord) {
    BitArray a(W);
    a.set();
    EXPECT_EQ(a.count(), W);
}

// ---------- copy, assignment, swap ----------
TEST(BitArray, CopyIsIndependent) {
    BitArray a(10, 5);
    BitArray b(a);
    b.set(1);
    EXPECT_EQ(a.to_string(), "0000000101");
    EXPECT_EQ(b.to_string(), "0000000111");
}

TEST(BitArray, CopyOfEmpty) {
    BitArray a;
    BitArray b(a);
    EXPECT_TRUE(b.empty());
}

TEST(BitArray, Assignment) {
    BitArray a(4, 5), b(20);
    b = a;
    EXPECT_EQ(b, a);
    b.set(3);
    EXPECT_NE(b, a);
}

TEST(BitArray, SelfAssignment) {
    BitArray a(10, 5);
    BitArray& r = a;
    a = r;
    EXPECT_EQ(a.to_string(), "0000000101");
}

TEST(BitArray, Swap) {
    BitArray a(4, 5), b(8, 255);
    a.swap(b);
    EXPECT_EQ(a.size(), 8);
    EXPECT_EQ(a.count(), 8);
    EXPECT_EQ(b.to_string(), "0101");
}

// ---------- resize, clear, push_back ----------
TEST(BitArray, ResizeGrowWithTrue) {
    BitArray a(3, 5);
    a.resize(6, true);
    EXPECT_EQ(a.to_string(), "111101");
}

TEST(BitArray, ResizeGrowWithFalse) {
    BitArray a(3, 5);
    a.resize(5);
    EXPECT_EQ(a.to_string(), "00101");
}

TEST(BitArray, ResizeShrink) {
    BitArray a(6, 45);          // 101101
    a.resize(4);
    EXPECT_EQ(a.to_string(), "1101");
}

TEST(BitArray, ResizeSame) {
    BitArray a(4, 5);
    a.resize(4, true);
    EXPECT_EQ(a.to_string(), "0101");
}

TEST(BitArray, ResizeAcrossWords) {
    BitArray a(10);
    a.resize(100, true);
    EXPECT_EQ(a.size(), 100);
    EXPECT_EQ(a.count(), 90);
}

TEST(BitArray, ResizeNegativeThrows) {
    BitArray a(4);
    EXPECT_THROW(a.resize(-1), std::invalid_argument);
}

TEST(BitArray, Clear) {
    BitArray a(10, 5);
    a.clear();
    EXPECT_TRUE(a.empty());
    a.clear();                  // repeated call is safe
    EXPECT_EQ(a.size(), 0);
}

TEST(BitArray, PushBack) {
    BitArray a;
    a.push_back(true);
    a.push_back(false);
    a.push_back(true);
    EXPECT_EQ(a.size(), 3);
    EXPECT_EQ(a.to_string(), "101");
}

TEST(BitArray, PushBackAcrossWordBoundary) {
    BitArray a(W);
    a.push_back(true);
    EXPECT_EQ(a.size(), W + 1);
    EXPECT_TRUE(a[W]);
    EXPECT_EQ(a.count(), 1);
}

// ---------- set / reset / [] ----------
TEST(BitArray, SetAndGet) {
    BitArray a(8);
    a.set(3);
    EXPECT_TRUE(a[3]);
    EXPECT_FALSE(a[2]);
    a.set(3, false);
    EXPECT_FALSE(a[3]);
}

TEST(BitArray, SetChain) {
    BitArray a(8);
    a.set(1).set(3).reset(1);
    EXPECT_EQ(a.to_string(), "00001000");
}

TEST(BitArray, SetAllKeepsTailClean) {
    BitArray a(70);
    a.set();
    EXPECT_EQ(a.count(), 70);
}

TEST(BitArray, ResetAll) {
    BitArray a(70);
    a.set();
    a.reset();
    EXPECT_TRUE(a.none());
}

TEST(BitArray, ResetBit) {
    BitArray a(4, 15);
    a.reset(2);
    EXPECT_EQ(a.to_string(), "1011");
}

TEST(BitArray, IndexOutOfRangeThrows) {
    BitArray a(4);
    EXPECT_THROW(a[-1], std::out_of_range);
    EXPECT_THROW(a[4], std::out_of_range);
    EXPECT_THROW(a.set(-1), std::out_of_range);
    EXPECT_THROW(a.set(4), std::out_of_range);
    EXPECT_THROW(a.reset(4), std::out_of_range);
    BitArray e;
    EXPECT_THROW(e[0], std::out_of_range);
}

// ---------- any / none / count / ~ ----------
TEST(BitArray, AnyNone) {
    BitArray a(70);
    EXPECT_FALSE(a.any());
    EXPECT_TRUE(a.none());
    a.set(69);
    EXPECT_TRUE(a.any());
    EXPECT_FALSE(a.none());
}

TEST(BitArray, AnyNoneOnEmpty) {
    BitArray a;
    EXPECT_FALSE(a.any());
    EXPECT_TRUE(a.none());
}

TEST(BitArray, Count) {
    BitArray a(130);
    EXPECT_EQ(a.count(), 0);
    a.set(0).set(64).set(129);
    EXPECT_EQ(a.count(), 3);
}

TEST(BitArray, Invert) {
    BitArray a(4, 5);
    EXPECT_EQ((~a).to_string(), "1010");
    EXPECT_EQ(a.to_string(), "0101");      // original unchanged
}

TEST(BitArray, InvertKeepsTailClean) {
    BitArray a(70);
    EXPECT_EQ((~a).count(), 70);
}

// ---------- bitwise operations ----------
TEST(BitArray, AndOrXor) {
    BitArray a(4, 12), b(4, 10);        // 1100, 1010
    EXPECT_EQ((a & b).to_string(), "1000");
    EXPECT_EQ((a | b).to_string(), "1110");
    EXPECT_EQ((a ^ b).to_string(), "0110");
}

TEST(BitArray, CompoundOperations) {
    BitArray b(4, 10);
    BitArray x(4, 12);  x &= b;  EXPECT_EQ(x.to_string(), "1000");
    BitArray y(4, 12);  y |= b;  EXPECT_EQ(y.to_string(), "1110");
    BitArray z(4, 12);  z ^= b;  EXPECT_EQ(z.to_string(), "0110");
}

TEST(BitArray, DifferentSizesThrow) {
    BitArray a(4), b(5);
    EXPECT_THROW(a &= b, std::invalid_argument);
    EXPECT_THROW(a |= b, std::invalid_argument);
    EXPECT_THROW(a ^= b, std::invalid_argument);
    EXPECT_THROW(a & b, std::invalid_argument);
    EXPECT_THROW(a | b, std::invalid_argument);
    EXPECT_THROW(a ^ b, std::invalid_argument);
}

// ---------- shifts ----------
TEST(BitArray, ShiftLeft) {
    BitArray a(8, 13);                  // 00001101
    EXPECT_EQ((a << 2).to_string(), "00110100");
    EXPECT_EQ(a.to_string(), "00001101");
}

TEST(BitArray, ShiftRight) {
    BitArray a(8, 13);
    EXPECT_EQ((a >> 2).to_string(), "00000011");
}

TEST(BitArray, ShiftLeftDropsHighBits) {
    BitArray a(4, 13);                  // 1101
    a <<= 1;
    EXPECT_EQ(a.to_string(), "1010");
}

TEST(BitArray, ShiftAcrossWords) {
    BitArray a(130);
    a.set(1);
    a <<= 100;
    EXPECT_TRUE(a[101]);
    EXPECT_EQ(a.count(), 1);
    a >>= 100;
    EXPECT_TRUE(a[1]);
    EXPECT_EQ(a.count(), 1);
}

TEST(BitArray, ShiftByZero) {
    BitArray a(8, 13);
    EXPECT_EQ(a << 0, a);
    EXPECT_EQ(a >> 0, a);
}

TEST(BitArray, ShiftByTooMuchGivesZeros) {
    BitArray a(8, 255);
    EXPECT_TRUE((a << 8).none());
    EXPECT_TRUE((a >> 8).none());
    EXPECT_TRUE((a << 100).none());
    EXPECT_TRUE((a >> 100).none());
}

TEST(BitArray, ShiftNegativeThrows) {
    BitArray a(8);
    EXPECT_THROW(a <<= -1, std::invalid_argument);
    EXPECT_THROW(a >>= -1, std::invalid_argument);
}

TEST(BitArray, ShiftEmpty) {
    BitArray a;
    a <<= 0;
    a >>= 3;
    EXPECT_TRUE(a.empty());
}

// ---------- comparison, to_string ----------
TEST(BitArray, EqualityAndInequality) {
    BitArray a(8, 5), b(8, 5), c(8, 6), d(9, 5);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    EXPECT_TRUE(a != c);   // same size, different bits
    EXPECT_TRUE(a != d);   // different size
}

TEST(BitArray, ToString) {
    BitArray a(5, 22);                  // 10110
    EXPECT_EQ(a.to_string(), "10110");
}