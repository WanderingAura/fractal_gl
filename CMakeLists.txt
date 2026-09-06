cmake_minimum_required( VERSION 3.25 )

project(fractal_gl)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_executable(fractals
    src/dragon.cpp
    src/fractal_creator.cpp
    src/main.cpp
    src/sierpinski.cpp
    deps/src/glad.c
)
target_include_directories(fractals PRIVATE include deps/include)
target_link_libraries(fractals PRIVATE glfw OpenGL::GL)
