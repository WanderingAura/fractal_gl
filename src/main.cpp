#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include <cstring>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <math.h>

#include "numeric_types.h"
#include "shader.h"
#include "dragon.hpp"

const i32 WIN_WIDTH = 800;
const i32 WIN_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow *window, i32 width, i32 height);
void processInput(GLFWwindow *window);

int main(int argc, char** argv) {
    if (argc == 2) {
        int n = atoi(argv[1]);
        generateDragonSeq(n);
        // TODO: draw the dragon curve. to draw the next line in the curve we need:
        // 1. the current position of the head
        // 2. the current orientation of the head
        return 0;
    }

    if (!glfwInit()) {
        std::cout << "Failed to initialise GLFW\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window =
        glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Epic Window", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialise GLAD\n";
        glfwTerminate();
        return -1;
    }

    Shader shader("../shaders/shader.vs",
                  "../shaders/shader.fs");

    f32 vertices[] = {
        0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
    };

    u32 VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    u32 VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    shader.use();

    glEnable(GL_DEPTH_TEST);

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f, 0.0f, 0.0f ),
    };

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();


        i32 modelLoc = shader.getUniformLoc("model");
        i32 viewLoc = shader.getUniformLoc("view");
        i32 projLoc = shader.getUniformLoc("projection");
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 2);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, i32 width, i32 height) {
    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
