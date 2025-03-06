#ifndef DRAGON_HPP
#define DRAGON_HPP

#include <vector>

#include "numeric_types.h"
#include "bit_array.hpp"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"


typedef struct {
    glm::vec2 pos;
    i32 direction; // in degrees
} Line;

class DragonCurve {
public:
    std::vector<Line> lines;

    DragonCurve(u32 n);

    void generateLines(f32 lineLen, glm::vec2 startPos);

private:
    BitArray sequence;
    u32 order;

    void genSequence(u32 n);
};

#endif
