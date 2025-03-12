#include "numeric_types.h"
#include "shader.h"
#include <GL/gl.h>
#include "sierpinski.hpp"

// TODO: use randomness to generate fractal
// 1. use rng to generate a random point within the triangle
// 2. give the point to the vertex shader, along with a random vertex
// 3. vertex shader calcs the midpoint between the two points and draws it
// 4. repeat

RandomSierpinski::RandomSierpinski()
    : shader("../shaders/random_sierpinski.vs", "../shaders/random_sierpinski.fs") {}

i32 RandomSierpinski::init(u32 order) {
    numPoints = order;

    f32 triangleVertices[] = {
        -0.5, -0.75, 0.0,
        0.5, -0.75, 0.0,
        0.0, 0.75, 0.0
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    shader.use();

    i32 vertLocs[3];
    vertLocs[0] = shader.getUniformLoc("triangleVert1");
    vertLocs[1] = shader.getUniformLoc("triangleVert2");
    vertLocs[2] = shader.getUniformLoc("triangleVert3");

    for (i32 i = 0; i < 9; i+=3) {
        glUniform3f(vertLocs[i], triangleVertices[i], triangleVertices[i+1], triangleVertices[i+2]);
    }

    return 0;
}

void RandomSierpinski::render() {
    glBindVertexArray(VAO);
    shader.use();
    glDrawArraysInstanced(GL_POINTS, 0, 3, numPoints);
}
