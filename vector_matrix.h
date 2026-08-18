#ifndef SPECIALRELATIVITYSANDBOX_VECTOR_MATRIX_H
#define SPECIALRELATIVITYSANDBOX_VECTOR_MATRIX_H
#include <cmath>

template<typename T, typename... Args>
concept ArgsType = (std::same_as<Args, T> and ...);

template<int n>
class Vector {
    static constexpr int size = n;
    double contents[n];

public:
    Vector() : contents{} {
    }

    template<typename... Args>
        requires ArgsType<double, Args...>
    Vector(Args... args) : contents{args...} {
    }

    Vector(const double contents[n]) : contents{contents} {
    }

    Vector(const Vector &other) = default;

    [[nodiscard]] const double *begin() const {
        return contents;
    }

    [[nodiscard]] const double *end() const {
        return &contents[n];
    }

    const double &operator[](const int i) const {
        return contents[i];
    }

    double &operator[](const int i) {
        return contents[i];
    }

    Vector &operator+=(const Vector &other) {
        for (int i = 0; i < n; i++) contents[i] += other.contents[i];
        return *this;
    }

    Vector operator+(const Vector &other) {
        Vector new_vec = other;
        return other += *this;
    }

    Vector &operator-=(const Vector &other) {
        for (int i = 0; i < n; i++) contents[i] -= other.contents[i];
        return *this;
    }

    Vector operator-(const Vector &other) {
        Vector new_vec = other;
        return other -= *this;
    }

    double operator*(const double other[n]) const {
        double result = 0.0;
        for (int i = 0; i < n; i++) result += contents[i] * other[i];
        return result;
    }

    double operator*(const Vector &other) const {
        return *this * other.contents;
    }

    [[nodiscard]] double magnitude2() const {
        double result = 0.0;
        for (const double d: contents) result += d * d;
        return result;
    }

    [[nodiscard]] double magnitude() const {
        return std::sqrt(magnitude2());
    }

    Vector &operator*=(const double mult) {
        for (double &d: contents) d *= mult;
        return *this;
    }

    Vector &operator*(const double mult) const {
        Vector result = *this;
        return result *= mult;
    }
};

template<int n>
std::ostream &operator<<(std::ostream &os, const Vector<n> &vec) {
    os << '[';
    for (int i = 0; i < n; i++) {
        os << vec[i];
        if (i != n - 1) os << ", ";
    }
    os << ']';
    return os;
}

template<int rows, int cols>
class Matrix {
    static constexpr int shape[2] = {rows, cols};
    double contents[rows * cols];

    double *operator[](const int row) {
        return &contents[row * cols];
    }

    const double *operator[](const int row) const {
        return &contents[row * cols];
    }

public:
    Matrix() : contents{} {
    }

    template<typename... Args>
        requires ArgsType<double, Args...>
    Matrix(Args... args) : contents{args...} {
    }

    Matrix(const double (&contents)[rows * cols]) : contents{contents} {
    }

    Matrix(const Matrix &other) = default;

    Matrix(Matrix &&other) = default;

    [[nodiscard]] const double *begin() const {
        return contents;
    }

    [[nodiscard]] const double *end() const {
        return &contents[rows * cols];
    }

    double &get(const int row, const int col) {
        return contents[row * cols + col];
    }

    [[nodiscard]] const double &get(const int row, const int col) const {
        return contents[row * cols + col];
    }

    Vector<rows> operator*(const Vector<cols> &other) const {
        Vector<rows> new_vec;
        for (int r = 0; r < rows; r++) new_vec[r] = other * (*this)[r];
        return new_vec;
    }

    template<int cols2>
    Matrix<rows, cols2> operator*(const Matrix<cols, cols2> &other) {
        Matrix<rows, cols2> new_mat;
        for (int r = 0; r < rows; r++)
            for (int c = 0; c < cols2; c++)
                for (int i = 0; i < cols; i++) new_mat.get(r, c) += get(r, i) * other.get(i, c);
        return new_mat;
    }

    void setIdentity() {
        if (rows != cols) return;
        for (int i = 0; i < rows; i++) get(i, i) = 1.0;
    }
};

template<int rows, int cols>
std::ostream &operator<<(std::ostream &os, const Matrix<rows, cols> mat) {
    for (int r = 0; r < rows; r++) {
        os << '[';
        for (int c = 0; c < cols; c++) {
            os << mat.get(r, c);
            if (c != cols - 1) os << ", ";
        }
        os << "]\n";
    }
    return os;
}

#endif
