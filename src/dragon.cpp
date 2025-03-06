#include "bit_array.hpp"
#include "glm/ext/vector_float2.hpp"
#include "numeric_types.h"
#include <cassert>
#include "dragon.hpp"


DragonCurve::DragonCurve(u32 n) : order(n) {}

void DragonCurve::generateLines(f32 lineLen, glm::vec2 startPos) {
    if (lines.size() != 0) {
        std::cerr << "lines have already been generated. generateLines should only be called"
            " once per instance\n";
    }
    if (order >= 20) {
        std::cerr << "unable to generate lines for dragon curve. order is too high\n";
        return;
    }

    genSequence(order);

    static glm::vec2 cardinals[] {
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(-1.0f, 0.0f),
        glm::vec2(0.0f, -1.0f),
    };
    
    Line curLine = {};
    curLine.pos = startPos;
    lines.push_back(curLine);

    for (u32 i = 0; i < sequence.size; i++) {
        i32 turn = sequence.get(i) ? 1 : -1;
        curLine.pos += lineLen * cardinals[curLine.direction];
        curLine.direction += turn;
        curLine.direction = curLine.direction & 3;
        assert(curLine.direction < 4 && curLine.direction >= 0);
        lines.push_back(curLine);
    }
    
    std::cout << "Lines: \n";
    for (auto line : lines) {
        std::cout << "coord: " << line.pos.x << " " << line.pos.y
            << ", direction: " << line.direction << std::endl;
    }
}

void DragonCurve::genSequence(u32 n) {
    assert(n < 20);

    // generate the binary sequence for dragon curve using method
    // shown in https://mathworld.wolfram.com/DragonCurve.html
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
