#pragma  once
#include "fractal.hpp"
#include "numeric_types.h"
#include "shader.h"

#include <vector>

class RandomSierpinski : public Fractal {
public:
    RandomSierpinski();
    i32 init(u32 order);

    void setOrderToRender(u32 order);

    void render();
private:
    std::vector<float> points;
    u32 numPoints;
    Shader shader;
    u32 VAO;
    u32 VBO;

    void generatePoints();
};
