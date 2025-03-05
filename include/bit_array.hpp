#ifndef BIT_ARRAY_HPP
#define BIT_ARRAY_HPP

#include "numeric_types.h"
#include <iostream>
#include <cstddef>
#include <vector>

class BitArray {
public:
    std::size_t size;
    BitArray() : data(0), size(0) {}
    bool get(std::size_t idx) {
        if (idx >= size ) {
            std::cerr << "index out of range\n";
            return 0;
        }
        return data[getPageIdx(idx)] & (1ull << getBitIdx(idx));
    }

    void set(std::size_t idx, bool bit) {
        if (idx >= size ) {
            std::cerr << "index out of range\n";
            return;
        }
        u64& page = data[getPageIdx(idx)];
        if (bit) {
            page |= (1ull << getBitIdx(idx));
        } else {
            page &= ~(1ull << getBitIdx(idx));
        }
    }

    void append(bool bit) {
        if (data.size() * bits_per_u64 < size + 1) {
            data.push_back(0);
        }
        size++;
        set(size - 1, bit);
    }

    void print() {
        std::cout << "The dragon curve in binary is: \n";
        for (u32 i = 0; i < size; i++) {
            std::cout << get(i);
        }
        std::cout << std::endl;
    }

    
private:
    std::vector<u64> data;

    static const u64 bits_per_u64 = sizeof(u64)*8;
    
    inline std::size_t getPageIdx(std::size_t idx) {
        return idx / 64;
    }

    inline std::size_t getBitIdx(std::size_t idx) {
        return idx % 64;
    }
};

#endif
