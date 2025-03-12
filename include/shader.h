#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include  <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "numeric_types.h"

#define ASSERT_SHADER_COMP(shader) assert_shader_comp_success(shader, __LINE__)
#define ASSERT_SHADER_LINK(program) assert_shader_link_success(program, __LINE__)

class Shader {
public:
    u32 id;
    
    Shader(const char* vertexPath, const char* fragmentPath) {
        std::ifstream vertexFile;
        std::ifstream fragmentFile;
        std::string vShaderCode;
        std::string fShaderCode;
        vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            vertexFile.open(vertexPath);
            fragmentFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vertexFile.rdbuf();
            fShaderStream << fragmentFile.rdbuf();
            vertexFile.close();
            fragmentFile.close();
            vShaderCode = vShaderStream.str();
            fShaderCode = fShaderStream.str();
        } catch (std::ifstream::failure& e) {
            std::cout << "ERROR: failed to read shader files, " << e.what() << std::endl;
        }
        const char* vShaderCodeC = vShaderCode.c_str();
        const char* fShaderCodeC = fShaderCode.c_str();
        u32 vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vShaderCodeC, NULL);
        glCompileShader(vertexShader);
        ASSERT_SHADER_COMP(vertexShader);

        u32 fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fShaderCodeC, NULL);
        glCompileShader(fragmentShader);
        ASSERT_SHADER_COMP(fragmentShader);

        id = glCreateProgram();
        glAttachShader(id, vertexShader);
        glAttachShader(id, fragmentShader);
        glLinkProgram(id);
        ASSERT_SHADER_LINK(id);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void use() {
        glUseProgram(id);
    }

    int getUniformLoc(const char* uniform) {
        return glGetUniformLocation(id, uniform);
    }

private:

    void assert_shader_comp_success(u32 shaderHandle, i32 line) {
        i32 success;
        char infoLog[512];
        glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shaderHandle, 512, NULL, infoLog);
            printf("ERROR: shader (id:%u) compilation failed (line:%d)\n%s\n",
                   shaderHandle, line, infoLog);
        }
    }

    void assert_shader_link_success(u32 shaderHandle, i32 line) {
        i32 success;
        char infoLog[512];
        glGetShaderiv(shaderHandle, GL_LINK_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shaderHandle, 512, NULL, infoLog);
            printf("ERROR: shader program (id:%u) linking failed (line:%d)\n%s\n",
                   shaderHandle, line, infoLog);
        }
    }
};

#endif
