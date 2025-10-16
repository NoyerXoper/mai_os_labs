#pragma once

#include <cstdint>

class Matrix {
public:
    Matrix() = delete;
    Matrix(std::size_t width, std::size_t height);
    Matrix(std::size_t width, std::size_t height, long double value);
    Matrix(const Matrix& other);
    Matrix(Matrix&& other) noexcept;
    ~Matrix() noexcept;

    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix&& other) noexcept;

    // returns i'th "row", but trully a pointer data_ + i * width
    long double* operator[](std::size_t i) const;

    std::size_t GetWidth() const;
    std::size_t GetHeight() const;

    void Swap(Matrix& other);

private:
    std::size_t width_;
    std::size_t height_;
    long double* data_;
};
