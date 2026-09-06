#include "bit_array.hpp"
#include "glm/common.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/trigonometric.hpp"
#include "numeric_types.h"
#include <cassert>
#include <cmath>
#include "dragon.hpp"
#include <GLFW/glfw3.h>

static const float SIZE_FACTOR = 0.8;

class Boundary {
public:
    float xMin;
    float xMax;
    float yMin;
    float yMax;
    float maxRange;

    Boundary() : xMin(INFINITY), xMax(-INFINITY), yMin(INFINITY), yMax(-INFINITY) {}
    
    void setMinMax(float x, float y) {
        if (x > xMax) {
            xMax = x;
        }
        if (x < xMin) {
            xMin = x;
        }
        if (y > yMax) {
            yMax = y;
        }
        if (y < yMin) {
            yMin = y;
        }
        maxRange = glm::max(xMax-xMin, yMax-yMin);
    }

    void normalise(glm::vec2& vec) {
        vec.x = SIZE_FACTOR*2*(vec.x - (xMax + xMin)/2) / maxRange;
        vec.y = SIZE_FACTOR*2*(vec.y - (yMax + yMin)/2) / maxRange;
    }
};

DragonCurve::DragonCurve() :
    shader("../shaders/shader.vs", "../shaders/shader.fs") {}

i32 DragonCurve::init(u32 order) {
    if (order > 20) {
        std::cerr << "the order given for the dragon curve is too high\n";
        return 1;
    }

    generateLines(order);

    // OpenGL set up: upload the precomputed (pos.xyz, green) vertex data for every
    // line so render() can draw the whole curve with a single glDrawArrays call.
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(f32), vertexData.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);

    shader.use();

    modelLoc = shader.getUniformLoc("model");

    numLinesToRender = lines.size() - 1;

    return 0;
}

void DragonCurve::setOrderToRender(u32 order) {
    u32 numLines = std::pow(2, order) - 1;
    if (numLines <= lines.size() - 1) {
        numLinesToRender = numLines;
    } else {
        std::cerr << "invalid number of lines: " << order << ", max is " << lines.size() - 1 << std::endl;
    }
}

void DragonCurve::render() {
    shader.use();
    glBindVertexArray(VAO);
    float curTime = glfwGetTime();

    glm::mat4 model(1.0f);
#if 1 // spinning the fractal for fun
    model = glm::rotate(model, glm::radians(curTime*120.0f),
                        glm::vec3(0.0f, 0.0f, 1.0f));
#endif
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_LINE_STRIP, 0, numLinesToRender + 1);
}

void DragonCurve::generateLines(u32 order) {
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
    lines.push_back(curLine);

    Boundary curveBoundary;

    for (u32 i = 0; i < sequence.size; i++) {
        i32 turn = sequence.get(i) ? 1 : -1;
        curLine.pos += cardinals[curLine.direction];
        curLine.pos.x = round(curLine.pos.x);
        curLine.pos.y = round(curLine.pos.y);
        curveBoundary.setMinMax(curLine.pos.x, curLine.pos.y);

        curLine.direction += turn;
        curLine.direction = curLine.direction & 3;
        assert(curLine.direction < 4 && curLine.direction >= 0);
        lines.push_back(curLine);
    }

    // normalise coords to values between -1.0 and 1.0
    for (auto& line : lines) {
        curveBoundary.normalise(line.pos);
    }
    // bake each point of the curve (and a smooth color gradient along it) into a
    // flat vertex buffer, so render() can draw the whole curve with a single
    // GL_LINE_STRIP call. Consecutive segments share an endpoint, so one vertex
    // per point is enough - no need to duplicate shared points or use an index buffer.
    vertexData.reserve(lines.size() * 4);
    for (u32 i = 0; i < lines.size(); i++) {
        f32 green = (f32)i / (lines.size() - 1);
        vertexData.insert(vertexData.end(), {lines[i].pos.x, lines[i].pos.y, 0.0f, green});
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
