#include <iostream>
#include <string>
#include <chrono>

#include "determinant.hpp"
#include "factorial.hpp"


int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Invalid parameters. Correct use: " << argv[0] << " --num-threads NUM_THREADS";
        return 1;
    }
    std::string argNumThreads = argv[1];
    if (argNumThreads != "--num-threads") {
        std::cerr << "Invalid arguments. Correct use: " << argv[0] << " --num-threads NUM_THREADS";
        return 1;
    }

    std::size_t n;
    argNumThreads = argv[2];
    std::cout << "Enter an order of a matrix (upto 13, or your death will be closer, then the determinant): ";
    std::cin >> n;
    std::size_t numThreads = std::min(std::stoul(argNumThreads), Factorial(n));

    if (n > 13) {
        std::cout << "that's too much, I can't handle it.";
        return 1;
    }

    Matrix matrix(n, n);
    std::cout << "Enter matrix:\n";
    for (std::size_t i = 0; i < n; ++i) {
        for(std::size_t j = 0; j < n; ++j) {
            std::cin >> matrix[i][j]; 
        }
    }
    auto start = std::chrono::steady_clock::now();
    std::cout << "Determinant: " << Determinant(matrix, numThreads) << "\n";
    auto end = std::chrono::steady_clock::now();
    std::cout << "Calculations time: " << end - start << "\n";

}