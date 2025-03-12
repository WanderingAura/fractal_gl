#include "numeric_types.h"
#include "shader.h"
#include <GL/gl.h>
#include <glm/vec2.hpp>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include "sierpinski.hpp"

static const glm::vec2 triangleVertices[] = {
    glm::vec2(-0.5, -0.5),
    glm::vec2(0.5, -0.5),
    glm::vec2(0.0, 0.5),
};

RandomSierpinski::RandomSierpinski()
    : shader("../shaders/random_sierpinski.vs", "../shaders/random_sierpinski.fs") {}

i32 RandomSierpinski::init(u32 order) {
    numPoints = order;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    generatePoints();

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*points.size(), points.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    shader.use();

    return 0;
}

void RandomSierpinski::generatePoints() {
    srand(time(0));

    // iterate on multiple points at a time
    glm::vec2 curPoints[] = {
        glm::vec2(-0.25, -0.4),
        glm::vec2(0.25, 0.35),
        glm::vec2(0.0, 0.0),
    };

    for (i32 i = 0; i < numPoints; i++) {
        i32 r = rand() % 3;
#if 0
        printf("r: %d\n", r);
#endif
        assert(r >= 0);
        for (i32 i = 0; i < ARR_SIZE(curPoints); i++) {
            curPoints[i] = 0.5f * (curPoints[i] + triangleVertices[r]);
            points.push_back(curPoints[i].x);
            points.push_back(curPoints[i].y);
        }
    }
}

void RandomSierpinski::setOrderToRender(u32 order) {
    return;
}

void RandomSierpinski::render() {
    glBindVertexArray(VAO);
    shader.use();
    glDrawArrays(GL_POINTS, 0, points.size() / 2);
}
