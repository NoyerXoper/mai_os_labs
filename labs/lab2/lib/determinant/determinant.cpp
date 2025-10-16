#include <string.h>
#include <vector>
#include <stdexcept>

#include "determinant.hpp"
#include "threads.hpp"
#include "matrix.hpp"
#include "permutations.hpp"
#include "factorial.hpp"

namespace {
struct ThreadData {
    ThreadData(const Matrix& matrix, long double& writePlace, const Permutation& start, const Permutation& end): matrix(matrix), writePlace(writePlace), start(std::move(start)), end(std::move(end)) {
        this->start.ResetFirst();
    }
    const Matrix& matrix;
    long double& writePlace;
    Permutation start;
    const Permutation end;
};

void* CalculateParitalDeterminant(void* rawData) {
    ThreadData* data = reinterpret_cast<ThreadData*>(rawData);
    const Matrix& matrix = data->matrix;
    long double& writePlace = data->writePlace;
    Permutation& start = data->start;
    const Permutation& end = data->end;

    do {
        const std::vector<unsigned int>& permutation = start.GenerateNext();
        long double val = start.Sign();
        for (std::size_t i = 0; i < matrix.GetHeight(); ++i) {
            val *= matrix[i][permutation[i]];
        }
        writePlace += val;
    } while(start != end);
    return NULL;
}
}

long double Determinant(const Matrix& matrix, std::size_t numThreads) {
    if (matrix.GetHeight() != matrix.GetWidth()) {
        throw std::invalid_argument("Matrix must be square matrix.");
    }
    // order of matrix
    std::size_t n = matrix.GetWidth();
    if (n == 1) {
        return matrix[0][0];
    }
    if (n == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
    // thread pool
    std::vector<os::Thread> threads;
    std::size_t numberOfOperands = Factorial(n);
    // the amount that each thread will calculate
    std::size_t batchSize = numberOfOperands / numThreads;
    // threads will output here
    std::vector<long double> partialDeterminants(numThreads, 0);
    std::vector<ThreadData> dataForThreads;

    threads.reserve(numThreads);
    dataForThreads.reserve(numThreads);

    for (std::size_t i = 0; i < numThreads; ++i) {
        threads.emplace_back(CalculateParitalDeterminant);
        dataForThreads.emplace_back(matrix, partialDeterminants[i], Permutation(n, batchSize * i), Permutation(n, std::min(batchSize * (i + 1) - 1, numberOfOperands - 1)));
        threads[i].Run(&dataForThreads[i]);
    }

    for (std::size_t i = 0; i < numThreads; ++i) {
        threads[i].Join();
    }


    long double determinant = 0;
    for (long double partial: partialDeterminants) {
        determinant += partial;
    }

    return determinant;

}