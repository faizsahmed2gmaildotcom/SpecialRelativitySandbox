#include <iostream>
#include "vector_matrix.h"
#include "objects.h"

int main() {
    RefFrame cam;
    cam.vel = {0.0, 0.0, 99.5};
    BaseObj<3> test_obj{
        Vector<double, 3>{0.0, 10.0, 0.0},
        Vector<double, 3>{0.0, -10.0, 0.0},
        Vector<double, 3>{0.0, 0.0, 0.0}
    };
    test_obj.offset({0.0, 0.0, 10.0});

    std::cout << test_obj;

    return 0;
}
