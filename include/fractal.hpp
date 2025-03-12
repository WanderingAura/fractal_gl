#pragma once
#include "numeric_types.h"
#include <memory>
#include <string>

class Fractal {
public:
    virtual ~Fractal() = default;

    virtual i32 init(u32 order) = 0;

    virtual void setOrderToRender(u32 order) = 0;

    virtual void render() = 0;
};

class FractalCreator {
public:
    std::unique_ptr<Fractal> createFractal(const std::string& name);
};
