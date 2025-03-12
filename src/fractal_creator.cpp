#include "dragon.hpp"
#include "fractal.hpp"
#include "sierpinski.hpp"
#include "numeric_types.h"
#include <memory>
#include <string>

std::unique_ptr<Fractal> FractalCreator::createFractal(const std::string& name) {
    if (name == std::string("dragon")) {
        return std::make_unique<DragonCurve>();
    }
    if (name == std::string("rsierpinski")) {
        return std::make_unique<RandomSierpinski>();
    }

    return nullptr;
}
