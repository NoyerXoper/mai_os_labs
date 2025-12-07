#include "exp.hpp"

extern "C" {
float E(int x) {
    float current_fact = 1;
    float ans = 1;
    for (int i = 1; i <= x; ++i) {
        current_fact *= i;
        ans += 1 / current_fact;
    }
    return ans;
}
}