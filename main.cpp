#include <iostream>
#include "vector_matrix.h"

int main() {
    Matrix<3, 5> test_mat{.1, .2, .3, .4, .5};
    Matrix<5, 3> test_mat_2{1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.67};
    const Vector<5> test_vec{1.0, 2.0, 3.0, 4.0, 5.0};
    const auto mult1 = test_mat * test_mat_2;
    auto mult2 = test_mat_2 * test_mat;
    mult2.setIdentity();
    std::cout << mult1 << '\n';
    std::cout << mult2 << '\n';
    std::cout << mult2 * test_vec << '\n';

    return 0;
}
