#ifndef SPECIAL_RELATIVITY
#define SPECIAL_RELATIVITY
#include "vector_matrix.h"
#include "relativity.h"

template<int vts>
class BaseObj {
    Vector<VtxRefFrame, vts> vtx_frames;
    const Matrix<vts, 3> vertices;
    Matrix<vts, 3> world_vertices; // Physical positions of vertices in world ref frame
    Matrix<vts, 3> optical_vertices; // Optical positions of vertices in cam ref frame

    void updateWorldVtc(const double dt, const int vtx) {
        const Vector v_rel = vtx_frames[vtx].vel * getLF(vtx_frames[vtx].vel);
        const Vector v_new = v_rel + vtx_frames[vtx].acc * dt;
        vtx_frames[vtx].vel = v_new; // * (1.0 / std::sqrt(1 + v_new.magnitude2() / C2));
        world_vertices[vtx] += vtx_frames[vtx].vel * dt;
    }

    double updateOptVtc(const CamRefFrame &cam, const int vtx) {
        // 1. Retarded-time geometry in the stationary (world) frame
        const Vector<double, 3> dx = cam.pos - world_vertices[vtx];

        const double a = vtx_frames[vtx].vel.magnitude2() - C2;
        const double b = -2.0 * (dx * vtx_frames[vtx].vel);
        const double c = dx.magnitude2();
        const double discriminant = b * b - 4.0 * a * c;
        if (discriminant < 0.0 or std::abs(a) < 1e-12) {
            optical_vertices[vtx] = dx;
            return 1.0;
        }

        const double sqrt_disc = std::sqrt(discriminant);
        const double t1 = (-b + sqrt_disc) / (2.0 * a);
        const double t2 = (-b - sqrt_disc) / (2.0 * a);
        const double t_ret = t1 < 0.0 ? t1 : t2;

        // 2. Emission position and direction toward camera
        const Vector<double, 3> x_emit = world_vertices[vtx] + vtx_frames[vtx].vel * t_ret;
        const Vector<double, 3> n = (x_emit - cam.pos).normalize(); // unit vector from vertex to camera at emission

        // 3. Relativistic Doppler factor k = 1.0 / [γ_vtx (1 - n·β_vtx) * γ_cam (1 - n·β_cam)]
        const double LF_cam = getLF(cam.vel);
        const double LF_vtx = getLF(vtx_frames[vtx].vel);
        const Vector<double, 3> B_cam = cam.vel * (1.0 / C);
        const Vector<double, 3> B_vtx = vtx_frames[vtx].vel * (1.0 / C);
        const double doppler_k = 1.0 / (LF_vtx * (1.0 - n * B_vtx) * LF_cam * (1.0 - n * B_cam));

        // 4. Lorentz boost the emission event into the camera rest frame
        const Vector<double, 3> x_rel = x_emit - cam.pos;
        const Vector<double, 3> V = cam.vel;
        const double V2 = V.magnitude2();

        Vector<double, 3> x_new;
        if (V2 > 1e-12) {
            const double v_dot_x = V * x_rel;
            const double scale = (LF_cam - 1.0) * v_dot_x / V2 - LF_cam * t_ret;
            x_new = x_rel + V * scale;
        } else {
            x_new = x_rel;
        }

        optical_vertices[vtx] = x_new;
        return doppler_k;
    }

    friend std::ostream &operator<<(std::ostream &os, const BaseObj &obj) {
        os << "Vertices (world):\n" << obj.world_vertices << '\n';
        os << "Vertices (optical):\n" << obj.optical_vertices << '\n';
        return os;
    }

public:
    explicit BaseObj(const Matrix<vts, 3> &vertices) : vertices(vertices), world_vertices(vertices) {
    }

    virtual ~BaseObj() = default;

    // Runs every frame
    void process(const double dt, const CamRefFrame &cam) {
        for (int vtx = 0; vtx < vts; vtx++)
            updateWorldVtc(updateOptVtc(cam, vtx) * dt, vtx);
    }

    void offset(const Vector<double, 3> &dx) {
        for (int v = 0; v < vts; v++)
            world_vertices[v] += dx;
    }
};

#endif
