#include <algorithm>
#include <stdexcept>
#include <sstream>

#include "permutations.hpp"
#include "factorial.hpp"

Permutation::Permutation(unsigned int size): sign_(1), isFirst_(true), isEnd_(false){
    for (int i = 0; i < size; ++i) {
        data_.push_back(i); 
    }
} 

Permutation::Permutation(unsigned int size, std::size_t i): Permutation(size){ 
    std::size_t fact = Factorial(size);
    if (i >= fact) {
        std::ostringstream oss("The indes is too big: ");
        oss << i << " >= " << fact; 
        throw std::invalid_argument(oss.str());
    }
    fact /= size;

    std::size_t index;
    // Calculating permutation
    for (std::size_t k = 0; k < size; ++k) {
        index = k + i / fact;
        i %= fact;
        fact /= std::max(size - k - 1, static_cast<std::size_t>(1));
        unsigned int selected = data_[index];
        for (std::size_t j = index; j > k; --j) {
            data_[j] = data_[j - 1];
        }
        data_[k] = selected;
    }
    // Calculating sign
    std::size_t numOfInverses = 0;
    unsigned int temp;
    // Counting number of inverses
    for (std::size_t i = 0; i < size; ++i) {
        for (std::size_t j = i + 1; j < size; ++j) {
            if (data_[i] > data_[j]) {
                ++numOfInverses;
            }
        }
    }
    sign_ = numOfInverses % 2 == 0 ? 1 : -1;
}

char Permutation::Sign() const noexcept{
    return sign_;
}

const std::vector<unsigned int>& Permutation::GetCurrent() const noexcept {
    return data_;
}

const std::vector<unsigned int>& Permutation::GenerateNext() {
    if (isFirst_) {
        isFirst_ = false;
        return data_;
    }
    int i = data_.size() - 2;
    while (i >= 0 && data_[i] >= data_[i + 1]) {
        --i;
    }
    if (i >= 0) {
        int j = data_.size() - 1;
        while (j > i && data_[i] >= data_[j]) {
            --j;
        }
        std::swap(data_[i], data_[j]);
        std::reverse(data_.begin() + i + 1, data_.end());
        // in brackets num_of_swaps - 1 (number of multiplications by a transpositions - 1)
        sign_ *= ((data_.size() - i - 1) / 2) % 2 == 0 ? -1 : 1;
    } else {
        isEnd_ = true;
    }
    return data_;
}

void Permutation::ResetFirst() noexcept {
    isFirst_ = true;
}

bool Permutation::IsEnd() const noexcept {
    return isEnd_;
}

bool operator==(const Permutation& first, const Permutation& second) noexcept {
    return first.data_ == second.data_;
}
bool operator!=(const Permutation& first, const Permutation& second) noexcept {
    return !(first == second);
}