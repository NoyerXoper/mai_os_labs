#include <iostream>

#include "exp.hpp"
#include "translation.hpp"

int main() {
    int action;
    for (;;) {
        std::cin >> action;
        switch (action) {
        case 1: {
            int x;
            std::cin >> x;
            std::cout << E(x) << '\n';
            break;
        }
        case 2: {
            long number;
            std::cin >> number;
            char* translated = translate(number);
            std::cout << translated << '\n';
            DeleteTranslatedString(translated);
            break;
        }
        default: {
            std::cout << "Incorrect status, exiting" << std::endl;
            return 0;
        }
        }
    }
}