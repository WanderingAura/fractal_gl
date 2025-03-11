#include "numeric_types.h"
#include "shader.h"

#include <random>

class RandomSierpinski {
public:
    RandomSierpinski();
    void initGL();

    void render();
private:
    Shader shader;
    u32 VAO;
    u32 VBO;
    
    // TODO: move rng to own file
    std::mt19937 rng;
};
