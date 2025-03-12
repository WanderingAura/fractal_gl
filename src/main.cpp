// OpenGL Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

// Image loading library
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Vector maths library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>
#include <cstring>

#include "numeric_types.h"
#include "shader.h"
#include "fractal.hpp"

const i32 WIN_WIDTH = 800;
const i32 WIN_HEIGHT = 600;

int curve_iteration = 0;

void framebuffer_size_callback(GLFWwindow *window, i32 width, i32 height);
void processInput(GLFWwindow *window, i32 maxIteration);

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Usage: fractal_gl {fractal_name} {N}\n";
        std::cout << "List of fractals supported\n";
        std::cout << "\t- dragon\tDragon Curve: N is the order of the curve\n";
        std::cout << "\t- rsierpinski\tRandomly generated Sierpinski triangle: N is the number of points generated\n";
        return 0;
    }

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

    FractalCreator creator;
    std::string fractalName = std::string(argv[1]);
    std::unique_ptr<Fractal> fractal = creator.createFractal(fractalName);

    int order = atoi(argv[2]);
    if (order <= 0) {
        std::cerr << "invalid input. N must be bigger than 0\n";
        glfwTerminate();
        return 0;
    }

    glEnable(GL_DEBUG_OUTPUT);

    fractal->init(order);

    // i32 numLines[30]; // FIX: temp calculation to test different orders of curves
    // numLines[0] = 1;
    // for (i32 i = 1; i < n; i++) {
    //     numLines[i] = numLines[i-1]*2 + 1;
    //     printf("numLines = %d\n", numLines[i]);
    // }

    double totalTime = 0;
    i32 numFrames = 0;
    f64 startTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        f64 curTime = glfwGetTime();
        f64 timeSinceLastFrame = curTime - startTime;
        totalTime += timeSinceLastFrame;
        numFrames++;
        if (numFrames == 60) {
            f64 averageFrameTime = totalTime/numFrames;
            printf("averageFrameTime = %lf, FPS = %lf\n", averageFrameTime,
                   1/averageFrameTime);
            numFrames = 0;
            totalTime = 0;
        }
        startTime = curTime;
        processInput(window, order);

        // glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT);

        fractal->render();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
cleanup:
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
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (curve_iteration < maxIteration) {
            curve_iteration += 1;
        } else {
            curve_iteration = 0;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (curve_iteration > 0) {
            curve_iteration-=1;
        } else {
            curve_iteration = 0;
        }
    }
}
