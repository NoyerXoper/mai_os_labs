#include "exp.hpp"

#include <cmath>

extern "C" {
float E(int x) {
    return std::pow(1 + 1/static_cast<float>(x), x);
}
}