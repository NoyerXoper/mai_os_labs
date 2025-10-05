#include <memory>
#include "matrix.hpp"

Matrix::Matrix(std::size_t width, std::size_t height): Matrix(width, height, 0){}

Matrix::Matrix(std::size_t width, std::size_t height, long double value): width_(width), height_(height_) {
    data_ = new long double[width * height];
    for (std::size_t i = 0; i < width * height; ++i) {
        data_[i] = value;
    }
}

Matrix::Matrix(const Matrix& other): width_(other.width_), height_(other.height_) {
    data_ = new long double[width_ * height_];
    std::uninitialized_copy(data_, data_ + width_ * height_, other.data_);
}

Matrix::Matrix(Matrix&& other) noexcept: width_(other.width_), height_(other.height_), data_(other.data_) {
    other.width_ = 0;
    other.height_ = 0;
    other.data_ = nullptr;
}

Matrix::~Matrix() noexcept {
    delete[] data_;
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this == &other) {
        return *this;
    }
    Matrix temp = other;
    this->Swap(temp);
    return *this;
}

Matrix& Matrix::operator=(Matrix&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    Matrix temp = std::move(other);
    this->Swap(temp);
    return *this;
}

long double* Matrix::operator[](std::size_t i) const {
    return data_ + i * width_;
}

void Matrix::Swap(Matrix& other) {
    std::swap(width_, other.width_);
    std::swap(height_, other.height_);
    std::swap(data_, other.data_);
}
