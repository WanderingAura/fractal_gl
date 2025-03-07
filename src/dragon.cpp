#include "bit_array.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/trigonometric.hpp"
#include "numeric_types.h"
#include <cassert>
#include "dragon.hpp"


DragonCurve::DragonCurve(u32 n, f32 lineLen) :
    order(n),
    lineLen(lineLen),
    shader("../shaders/shader.vs", "../shaders/shader.fs") {}

void DragonCurve::initGL() {

    f32 vertices[] = {
        0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    shader.use();

    modelLoc = shader.getUniformLoc("model");
    greenLoc = shader.getUniformLoc("green");
}

void DragonCurve::renderCurve(i32 numLines) {
    shader.use();
    glBindVertexArray(VAO);

    for (i32 i = 0; i < numLines; i++) {
        Line& line = lines[i];
        
        float green = (float)i/lines.size();

        glUniform1f(greenLoc, green);
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(line.pos.x, line.pos.y, 0.0f));
        model = glm::rotate(model, glm::radians(line.direction*90.0f),
                            glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(lineLen*2, lineLen*2, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_LINES, 0, 2);

    }
}

void DragonCurve::generateLines(glm::vec2 startPos) {
    if (lines.size() != 0) {
        std::cerr << "lines have already been generated. generateLines should only be called"
            " once per instance\n";
    }
    if (order >= 20) {
        std::cerr << "unable to generate lines for dragon curve. order is too high\n";
        return;
    }

    genSequence(order);

    static const glm::vec2 cardinals[] {
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

    // sequence.print();
}
