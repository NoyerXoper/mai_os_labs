#include <stdlib.h>
#include <iostream>

#include "queue.hpp"

bool IsPrime(int num) {
    if (num <= 1) {
        return false;
    }
    if (num == 2) {
        return true;
    }
    if (num % 2 == 0) {
        return false;
    }

    for (int divisor = 3; divisor * divisor <= num; divisor += 2) {
        if (num % divisor == 0) {
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Use: child SHARED_MEMORY_NAME\n";
        return 1;
    }
    int number;

    shared_queue::WritingEndSharedQueue<int, 512> shared_memory(argv[1], false);

    while (std::cin >> number) {
        if (number < 0 || IsPrime(number)) {
            return 0;
        }
        shared_memory.PushBack(number);
    }
    return 0;
}
