cmake_minimum_required( VERSION 3.5 )

project(fractal_gl)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE) # exports compile settings so that linter can see
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

file(GLOB SOURCE_FILES "src/*.cpp")
set(DEP_SOURCE_DIR deps/src/glad.c)

link_libraries(glfw3 OpenGL::GL)
include_directories(PRIVATE include)
include_directories(PRIVATE deps/include)

add_executable(fractals ${SOURCE_FILES} ${DEP_SOURCE_DIR})

# run cmake in debug mode:
# make a debug dir in project, cd into it and run
# cmake -DCMAKE_BUILD_TYPE=Debug ..
