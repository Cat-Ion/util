#ifndef BITSTREAM_MATCH_HPP
#define BITSTREAM_MATCH_HPP
#include "BitArray.hpp"
#include <cstddef>

/** @brief Class to look for a bit string in a stream, based on the
 * Knuth-Morris-Pratt algorithm.
 *
 * @tparam NEEDLE_LEN is the length of the string to look for
 */
template<size_t NEEDLE_LEN>
class BitStreamMatch {
public:
    static constexpr size_t needle_len = NEEDLE_LEN;

    constexpr BitStreamMatch(unsigned char const *needle) : needle(needle) { init_table(); }

    /** @brief Restarts the search by setting the number of matched bits to zero */
    constexpr void restart() { k = 0; }

    /** @brief Handles a single bit of the haystack.
     *
     * @return True if the needle is fully matched after this bit
     */
    constexpr bool handle_bit(bool bit) {
        if (needle[k] != bit) {
            k = table[k];
        }

        if (k == needle_len - 1) {
            k = 0;
            return true;
        } else {
            k++;
            return false;
        }
    }

    /** @brief Get the number of bits matched right now */
    constexpr size_t get_matched_bits() const { return k; }

private:
    constexpr void init_table() {
        size_t pos = 1;
        size_t cnd = 0;

        table[0] = ~size_t{0};
        while (pos < needle_len) {
            if (needle[pos] == needle[cnd]) {
                table[pos] = table[cnd];
            } else {
                table[pos] = cnd;
                cnd = table[cnd];
                while (cnd >= 0 && needle[pos] != needle[cnd]) {
                    cnd = table[cnd];
                }
            }
            pos++;
            cnd++;
        }
    }

    BitArray<unsigned char const> const needle;
    int table[needle_len]{};
    size_t k = 0;
};

#endif
