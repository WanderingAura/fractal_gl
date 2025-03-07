#ifndef DRAGON_HPP
#define DRAGON_HPP

#include <vector>

#include "numeric_types.h"
#include "bit_array.hpp"
#include "shader.h"

#include <glm/vec2.hpp>

typedef struct {
    glm::vec2 pos;
    i32 direction; // in quarter turns with 0 being towards the right
} Line;

class DragonCurve {
public:
    std::vector<Line> lines;

    DragonCurve(u32 n);

    void generateLines();

    void initGL();
    void renderCurve(i32 numLines);

private:
    BitArray sequence;

    // OpenGL shader/buffers
    Shader shader;
    u32 VAO;
    u32 VBO;

    // uniform locations
    i32 modelLoc;
    i32 greenLoc;

    // curve properties
    u32 order;
    f32 lineLen;

    void genSequence(u32 n);
};

#endif
