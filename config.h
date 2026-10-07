#ifndef SPECIALRELATIVITYSANDBOX_CONFIG_H
#define SPECIALRELATIVITYSANDBOX_CONFIG_H
#include <limits>

// dtypes
using vec3 = Array<float, 3>;
using tris = std::vector<Array<unsigned, 3> >;

struct Range {
private:
    class Iter {
        unsigned cur;

    public:
        Iter(const unsigned cur) : cur(cur) {
        }

        Iter &operator++() {
            cur++;
            return *this;
        }

        unsigned operator*() const {
            return cur;
        }

        bool operator!=(const Iter &other) const {
            return cur != other.cur;
        }
    };

public:
    const unsigned first;
    const unsigned last;

    Range(const unsigned first, const unsigned last) : first(first), last(last) {
    }

    Range(const size_t first, const size_t last) : first(first), last(last) {
    }

    [[nodiscard]] Iter begin() const {
        return first;
    }

    [[nodiscard]] Iter end() const {
        return last;
    }
};

// invalid vals
constexpr float INVALID_FLOAT = std::numeric_limits<float>::infinity();
const Array<float, 3> INVALID_ARR3{INVALID_FLOAT, INVALID_FLOAT, INVALID_FLOAT};
const vec3 INVALID_VEC3{INVALID_FLOAT, INVALID_FLOAT, INVALID_FLOAT};

// constants
#define MAX_PAST_TIME 60  // seconds
constexpr float C = 299792458.0;
constexpr float C2 = C * C;
constexpr int fps = 60;
constexpr float spf = 1.0 / fps;
inline float time_since_last_frame = 0.0;
constexpr float WORLD_PLANE_Y = -3.0;

#endif //SPECIALRELATIVITYSANDBOX_CONFIG_H
