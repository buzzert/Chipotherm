#ifndef QUBE_ACTOR_H
#define QUBE_ACTOR_H

#include "actor.h"
#include "color.h"

#include <cairo/cairo.h>
#include <memory>
#include <vector>

struct Vec2 {
    double x;
    double y;
};

struct Vec3 {
    double x;
    double y;
    double z;

    double& operator[] (const int index) {
        switch (index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw;
        }
    }
};

class QubeActor : public Actor 
{
public:
    QubeActor(Rect rect);

    void update(SDL_Renderer *renderer) override;
    void render(SDL_Renderer *renderer) override;

private:
    std::shared_ptr<cairo_t>     _cairo_ctx;
    std::shared_ptr<SDL_Surface> _surface;
    double                       _time_offset;

    void draw_line(double x1, double y1, double x2, double y2, double size);
    void draw_point(double x, double y, double size);
    void draw_mesh(
        double x, double y, 
        std::vector<Vec3> vertices, 
        std::vector<std::pair<unsigned int, unsigned int>> edges,
        double scale,
        Color color,
        double time_offset
    );
    void draw_tetrahedron(double cx, double cy, double radius, double time_offset);
};

#endif
