#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/trigonometric.hpp"
#include <cstring>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <unistd.h>

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

int curve_iteration = 0;

void framebuffer_size_callback(GLFWwindow *window, i32 width, i32 height);
void processInput(GLFWwindow *window, i32 maxIteration);

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: fractal_gl {N}\n";
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        std::cerr << "invalid input. this program only accepts integer arguments < 2^32\n";
    }
    const float line_length = 0.02f;

    DragonCurve curve(n);
    curve.generateLines(line_length, glm::vec2(0.0f, 0.0f));

    // create window
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

    i32 modelLoc = shader.getUniformLoc("model");
    i32 greenLoc = shader.getUniformLoc("green");
    // i32 viewLoc = shader.getUniformLoc("view");
    // i32 projLoc = shader.getUniformLoc("projection");

    glfwSwapBuffers(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    f64 startTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        f64 curTime = glfwGetTime();
        f64 timeSinceLastFrame = curTime - startTime;
        startTime = curTime;
        processInput(window, curve.lines.size());

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        glBindVertexArray(VAO);
        for (i32 i = 0; i < curve_iteration; i++) {
            Line& line = curve.lines[i];
            
            float green = (float)i/curve.lines.size();

            glUniform1f(greenLoc, green);
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(line.pos.x, line.pos.y, 0.0f));
            model = glm::rotate(model, glm::radians(line.direction*90.0f),
                                glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(line_length*2, line_length*2, 1.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_LINES, 0, 2);

        }
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, i32 width, i32 height) {
    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
}

void processInput(GLFWwindow *window, i32 maxIteration) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_UP)) {
        if (curve_iteration < maxIteration) {
            curve_iteration++;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN)) {
        if (curve_iteration > 0) {
            curve_iteration--;
        }
    }
}
