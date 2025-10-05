#include "permutations.hpp"
#include <algorithm>

Permutation::Permutation(unsigned int size): data_(size), sign_(1) {
    for (int i = 1; i <= size; ++i) {
        data_.push_back(i); 
    }
} 

char Permutation::Sign() const {
    return sign_;
}

const std::vector<unsigned int>& Permutation::GenerateNext() {
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
        // in brackets num_of_swaps - 1
        sign_ *= ((data_.size() - i - 1) / 2) % 2 == 0 ? -1 : 1;
    }
    return data_;
}
