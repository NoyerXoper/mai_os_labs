#include <stdlib.h>
#include <stdio.h>

#include "os.hpp"

bool IsPrime(long long num) {
    if (num <= 1) {
        return false;
    }
    if (num == 2) {
        return true;
    }
    if (num % 2 == 0) {
        return false;
    }
    for (long long divisor = 3; divisor * divisor <= num; divisor += 2) {
        if (num % divisor == 0) {
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[]) {
    long long number;

    while (scanf("%llu", &number) != EOF) {
        if (number < 0 || IsPrime(number)) {
            return 0;
        }
        printf("%llu\n", number);
    }
    return 0;

}
