#include "bit_array.hpp"
#include "numeric_types.h"

void generateDragonSeq(u32 n) {
    BitArray sequence;

    while (n--) {
        sequence.append(1);
        u32 cur_size = sequence.size;
        for (i32 i = 0; i < cur_size - 1; i++) {
            // flips the bit when the seq is at its midpoint
            bool bit = sequence.get(i) ^ (i == (cur_size / 2 - 1));
            sequence.append(bit);
        }
    }

    sequence.print();
}
