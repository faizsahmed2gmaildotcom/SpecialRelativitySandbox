#ifndef VECTOR_MATRIX
#define VECTOR_MATRIX
#include <iostream>
#include <cmath>
#include <ranges>

template<typename T, typename... Args>
concept ArgsType = (std::same_as<Args, T> and ...);

template<typename T, int n>
class Vector {
    T contents[n];

public:
    Vector() : contents{} {
    }

    template<typename... Args>
        requires ArgsType<T, Args...>
    Vector(Args... args) : contents{args...} {
    }

    Vector(const T contents[n]) {
        for (int i = 0; i < n; i++) this->contents[i] = contents[i];
    }

    Vector(const Vector &other) = default;

    [[nodiscard]] const T *begin() const {
        return contents;
    }

    [[nodiscard]] const T *end() const {
        return &contents[n];
    }

    const T &operator[](const int i) const {
        return contents[i];
    }

    T &operator[](const int i) {
        return contents[i];
    }

    Vector &operator+=(const Vector &other) {
        for (int i = 0; i < n; i++) contents[i] += other.contents[i];
        return *this;
    }

    Vector operator+(const Vector &other) const {
        Vector result = *this;
        return result += other;
    }

    Vector &operator-=(const Vector &other) {
        for (int i = 0; i < n; i++) contents[i] -= other.contents[i];
        return *this;
    }

    Vector operator-(const Vector &other) const {
        Vector result = *this;
        return result -= other;
    }

    T operator*(const Vector &other) const {
        T result{};
        for (int i = 0; i < n; i++) result += (*this)[i] * other[i];
        return result;
    }

    Vector &operator*=(const T mult) {
        for (T &d: contents) d *= mult;
        return *this;
    }

    Vector operator*(const T mult) const {
        Vector result = *this;
        return result *= mult;
    }

    [[nodiscard]] T magnitude2() const {
        T result{};
        for (const T d: contents) result += d * d;
        return result;
    }

    [[nodiscard]] T magnitude() const {
        return std::sqrt(magnitude2());
    }

    [[nodiscard]] Vector normalize() const {
        const T mag = magnitude();
        if (mag == 0.0) return *this;
        return *this * (1.0 / mag);
    }
};

template<typename T, int n>
std::ostream &operator<<(std::ostream &os, const Vector<T, n> &vec) {
    os << '[';
    for (const double d: vec | std::ranges::views::take(n - 1)) {
        os << d << ", ";
    }
    os << vec[n - 1] << ']';
    return os;
}

template<int rows, int cols>
class Matrix {
    Vector<Vector<double, cols>, rows> contents;

public:
    Matrix() : contents{} {
    }

    Matrix(const Vector<Vector<double, cols>, rows> &contents) : contents{contents} {
    }

    Matrix(const Matrix &other) = default;

    Matrix(Matrix &&other) = default;

    [[nodiscard]] const Vector<double, cols> *begin() const {
        return contents;
    }

    [[nodiscard]] const Vector<double, cols> *end() const {
        return &contents[rows - 1];
    }

    Vector<double, rows> operator*(const Vector<double, cols> &other) const {
        Vector<double, rows> result;
        for (int r = 0; r < rows; r++) result[r] = contents[r] * other;
        return result;
    }

    Vector<double, cols> &operator[](const int row) {
        return contents[row];
    }

    const Vector<double, cols> &operator[](const int row) const {
        return contents[row];
    }

    template<int cols2>
    Matrix<rows, cols2> operator*(const Matrix<cols, cols2> &other) {
        Matrix<rows, cols2> new_mat;
        for (int r = 0; r < rows; r++)
            for (int c = 0; c < cols2; c++)
                for (int i = 0; i < cols; i++) new_mat[r][c] += (*this)[r][i] * other[i][c];
        return new_mat;
    }

    void setIdentity() {
        if (rows != cols) return;
        for (int i = 0; i < rows; i++) (*this)[i][i] = 1.0;
    }
};

template<int rows, int cols>
std::ostream &operator<<(std::ostream &os, const Matrix<rows, cols> mat) {
    for (int r = 0; r < rows; r++) {
        os << '[';
        for (int c = 0; c < cols; c++) {
            os << mat[r][c];
            if (c != cols - 1) os << ", ";
        }
        os << "]\n";
    }
    return os;
}

#endif
