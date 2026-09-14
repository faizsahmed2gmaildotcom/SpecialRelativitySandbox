#ifndef SPECIALRELATIVITYSANDBOX_DEBUG_TOOLS_H
#define SPECIALRELATIVITYSANDBOX_DEBUG_TOOLS_H
#include "objects.h"

template<typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &vec) {
    os << '[';
    for (const auto &val: vec) {
        os << val << ", ";
    }
    os << "\b\b]";
    return os;
}

inline void unpackGPU(const GlobalVertices &global_vtc) {
    const double* start = &global_vtc.optical_vtc[0][0];
    const double* end = start + global_vtc.optical_vtc.size() * 3;
    for (const double* d = start; d < end; ++d) {
        std::cout << *d << '\n';
    }
}

#endif //SPECIALRELATIVITYSANDBOX_DEBUG_TOOLS_H
