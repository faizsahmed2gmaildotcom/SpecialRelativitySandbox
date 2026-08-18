#ifndef SPECIALRELATIVITYSANDBOX_OBJECTS_H
#define SPECIALRELATIVITYSANDBOX_OBJECTS_H
#include "vector_matrix.h"

struct RefFrame {
    Vector<3> pos;
    Vector<3> vel;
    Vector<3> acc;

    void process(const double dt) {
        vel += acc * dt;
        pos += vel * dt;
    }
};

#endif //SPECIALRELATIVITYSANDBOX_OBJECTS_H
