#include <iostream>
#include "vector_matrix.h"

int main() {
    Matrix<3, 5> test_mat;
    Matrix<5, 3> test_mat_2;
    Vector<5> test_vec;
    test_vec[0] = 1.0;
    test_vec[1] = 2.67;
    const auto mult1 = test_mat * test_mat_2;
    auto mult2 = test_mat_2 * test_mat;
    mult2.setIdentity();
    mult2.get(1, 0) = 67;
    mult2.get(4, 1) = 21;
    std::cout << mult1 << '\n';
    std::cout << mult2 << '\n';
    std::cout << mult2 * test_vec << '\n';

    return 0;
}
