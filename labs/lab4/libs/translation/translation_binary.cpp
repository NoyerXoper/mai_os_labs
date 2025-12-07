#include "translation.hpp"

#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>

extern "C" {
char* translate(long number) {
    std::stringstream ss;
    bool is_negative = number < 0; 
    number = std::abs(number);
    int size = 0;

    while (number > 0) {
        ss << number % 2;
        number /= 2;
        ++size;
    }

    if (is_negative) {
        ss << '-';
        ++size;
    }

    char* translated = new char[size+1];
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