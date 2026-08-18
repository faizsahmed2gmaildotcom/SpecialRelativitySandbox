#ifndef SPECIALRELATIVITYSANDBOX_OBJECTS_H
#define SPECIALRELATIVITYSANDBOX_OBJECTS_H
#include "vector_matrix.h"

struct RefFrame {
    Vector<3> pos;
    Vector<3> vel;
    Vector<3> acc;
};

template <int v_rows, int v_cols>
class BaseObj {
    RefFrame world_frame;
    Matrix<v_rows, v_cols> world_vertices;
    Matrix<v_rows, v_rows> cam_vertices;  // Lorentz-transformed result of origin+world_vertices
};

#endif //SPECIALRELATIVITYSANDBOX_OBJECTS_H
