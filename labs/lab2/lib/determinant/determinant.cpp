#include <string.h>
#include <vector>

#include <iostream>

#include "determinant.hpp"
#include "threads.hpp"
#include "matrix.hpp"
#include "permutations.hpp"
#include "factorial.hpp"

namespace {
struct ThreadData {
    ThreadData(const Matrix& matrix, long double& writePlace, const Permutation& start, const Permutation& end): matrix(matrix), writePlace(writePlace), start(start), end(end) {
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
    } while(!(start == end));
    return NULL;
}
}

long double Determinant(const Matrix& matrix, std::size_t numThreads) {
    // order of matrix
    std::size_t n = matrix.GetWidth();
    // thread pool
    std::vector<os::Thread> threads;
    std::size_t numberOfOperands = Factorial(n);
    // the amount that each thread will calculate
    std::size_t batchSize = numberOfOperands / numThreads;
    // threads will output here
    std::vector<long double> partialDeterminants(numThreads, 0);
    std::vector<ThreadData*> dataForThreads;

    for (std::size_t i = 0; i < numThreads; ++i) {
        threads.emplace_back(CalculateParitalDeterminant);
        //std::cout << "Created Thread " << i << std::endl;
        dataForThreads.push_back(new ThreadData(matrix, partialDeterminants[i], Permutation(n, batchSize * i), Permutation(n, std::min(batchSize * (i + 1) - 1, numberOfOperands - 1))));
        //std::cout << "Created ThreadData " << i << std::endl;
        threads.back().Run(reinterpret_cast<void*>(dataForThreads.back()));
        //std::cout << "Started Thread " << i << std::endl;
    }

    for (std::size_t i = 0; i < numThreads; ++i) {
        threads[i].Join();
    }

    for (ThreadData* dat: dataForThreads) {
        delete dat;
    }

    long double determinant = 0;
    for (long double partial: partialDeterminants) {
        determinant += partial;
    }

    return determinant;

}