#ifndef BITARRAY_HPP
#define BITARRAY_HPP
#include <cstddef>

/** @brief BitArray is used to read or write single bits in an array */
template<class T>
class BitArray {
public:
    /** @brief A writable reference to a single bit in an array */
    class reference {
        friend BitArray;
        constexpr reference(BitArray &parent, size_t pos) : parent(parent), pos(pos) {}

    public:
        /** @brief Return the referenced bit */
        constexpr operator bool() const { return parent.get(pos); }

        /** @brief Return the negation of the referenced bit */
        constexpr bool operator~() const { return !static_cast<bool>(*this); }

        /** @brief Write a new value to the referenced bit */
        constexpr reference &operator=(bool v) {
            parent.set(pos, v);
            return *this;
        }

        /** @brief Write a new value to the referenced bit */
        constexpr reference &operator=(reference const &v) {
            parent.set(pos, static_cast<bool>(v));
            return *this;
        }

        /** @brief Invert the referenced bit */
        constexpr reference &flip() {
            *this = ~*this;
            return *this;
        }

    private:
        BitArray &parent;
        size_t pos;
    };

    /** @brief Construct a new BitArray that wraps data */
    constexpr BitArray(T *const data) : data(data) {}

    /** @brief Reads the bit at position pos */
    constexpr bool operator[](size_t pos) const { return get(pos); }
    /** @brief Reads the bit at position pos. Can also be used to write to it. */
    constexpr reference operator[](size_t pos) { return reference(*this, pos); }

    /** @brief Reads the bit at position pos */
    constexpr bool get(size_t pos) const { return 1 & (data[word(pos)] >> bit(pos)); }
    /** @brief Writes val to the bit at position pos */
    constexpr void set(size_t pos, bool val) {
        T &p = data[word(pos)];
        if (val) {
            p |= T(1) << bit(pos);
        } else {
            p &= ~(T(1) << bit(pos));
        }
    }

private:
    static constexpr size_t TS = sizeof(T) * 8;
    constexpr size_t word(size_t pos) const { return pos / TS; }
    constexpr size_t bit(size_t pos) const { return TS - 1 - (pos % TS); }
    T *const data;
};
#endif
