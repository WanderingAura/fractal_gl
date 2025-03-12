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

    // OpenGL set up
    f32 vertices[] = {
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
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

    generateLines(order);

    numLinesToRender = lines.size();

    return 0;
}

void DragonCurve::setNumLinesToRender(u32 n) {
    if (n <= lines.size()) {
        numLinesToRender = n;
    } else {
        std::cerr << "invalid number of lines: " << n << ", max is " << lines.size() << std::endl;
    }
}

void DragonCurve::render() {
    shader.use();
    glBindVertexArray(VAO);
    float curTime = glfwGetTime();

    for (i32 i = 0; i < numLinesToRender; i++) {
        Line& line = lines[i];
        
        float green = (float)i/lines.size();

        glUniform1f(greenLoc, green);
        glm::mat4 model(1.0f);
        model = glm::rotate(model, glm::radians(curTime*120.0f),
                            glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(line.pos.x, line.pos.y, 0.0f));
        model = glm::rotate(model, glm::radians(line.direction*90.0f),
                            glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(lineLen, lineLen, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_LINES, 0, 2);

    }
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
    lineLen = SIZE_FACTOR * 2/curveBoundary.maxRange;
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
