#ifndef SPECIALRELATIVITYSANDBOX_VECTOR_MATRIX_H
#define SPECIALRELATIVITYSANDBOX_VECTOR_MATRIX_H

template<int n>
class Vector {
    static constexpr int size = n;
    double contents[n];

public:
    Vector() : contents{} {
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

    double operator[](const int i) const {
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

    double operator*(const Vector &other) const {
        double result = 0.0;
        for (int i = 0; i < n; i++) result += contents[i] * other.contents[i];
        return result;
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
    Vector<cols> contents[rows];

public:
    Matrix() : contents{} {
    }

    Matrix(const Vector<cols> (&contents)[rows]) : contents{contents} {
    }

    Matrix(const Matrix &other) = default;

    Matrix(Matrix &&other) = default;

    [[nodiscard]] const Vector<cols> *begin() const {
        return contents;
    }

    [[nodiscard]] const Vector<cols> *end() const {
        return &contents[rows];
    }

    Vector<cols> operator[](const int i) const {
        return contents[i];
    }

    Vector<cols> &operator[](const int i) {
        return contents[i];
    }

    Vector<rows> operator*(const Vector<cols> &other) const {
        Vector<rows> new_vec;
        for (int r = 0; r < rows; r++) new_vec[r] = contents[r] * other;
        return new_vec;
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
        for (int i = 0; i < rows; i++) contents[i][i] = 1.0;
    }
};

template<int rows, int cols>
std::ostream &operator<<(std::ostream &os, const Matrix<rows, cols> mat) {
    for (const Vector<cols> &vec: mat) os << vec << '\n';
    return os;
}

#endif
