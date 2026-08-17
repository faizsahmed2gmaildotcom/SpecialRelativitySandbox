#include <iostream>
#include "vector_matrix.h"

int main() {
    Matrix<3, 5> test_mat;
    Matrix<5, 3> test_mat_2;
    std::cout << test_mat * test_mat_2 << '\n';
    std::cout << test_mat_2 * test_mat << '\n';

    return 0;
}
