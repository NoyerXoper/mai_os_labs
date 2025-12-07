#include "translation.hpp"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <string>

extern "C" {
char* translate(long number) {
    std::stringstream ss;
    bool is_negative = number < 0;
    number = std::abs(number);
    int size = 0;

    while (number > 0) {
        ss << number % 3;
        number /= 3;
        ++size;
    }

    if (is_negative) {
        ss << '-';
        ++size;
    }

    char* translated = new char[std::max(2, size + 1)];
    if (size == 0) {
        translated[0] = '0';
        translated[1] = 0;
        return translated;
    }
    translated[size] = 0;
    for (int i = size - 1; i >= 0; --i) {
        ss >> translated[i];
    }
    return translated;
}

void DeleteTranslatedString(char* translated) {
    delete[] translated;
}
}