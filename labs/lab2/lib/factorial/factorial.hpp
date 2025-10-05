#include <cstddef>

constexpr std::size_t Factorial(std::size_t num) noexcept {
    std::size_t factorial = 1;
    for (std::size_t i = 2; i <= num; ++i) {
        factorial *= i;
    }
    return factorial;
}