#pragma  once
#include "fractal.hpp"
#include "numeric_types.h"
#include "shader.h"

class RandomSierpinski : public Fractal {
public:
    RandomSierpinski();
    i32 init(u32 order);

    void render();
private:
    u32 numPoints;
    Shader shader;
    u32 VAO;
    u32 VBO;
};
