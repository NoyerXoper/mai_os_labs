#include <cstddef>

#include "matrix.hpp"

namespace {
struct ThreadData;
void* CalculatePartialDeterminant(void* rawData);
}

long double Determinant(const Matrix& matrix, std::size_t numThreads);

