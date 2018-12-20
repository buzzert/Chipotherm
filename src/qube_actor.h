#ifndef QUBE_ACTOR_H
#define QUBE_ACTOR_H

#include "actor.h"
#include <cairo/cairo.h>
#include <memory>

class QubeActor : public Actor 
{
public:
    QubeActor(Rect rect);

    void update(SDL_Renderer *renderer) override;
    void render(SDL_Renderer *renderer) override;

private:
    std::shared_ptr<cairo_t>     _cairo_ctx;
    std::shared_ptr<SDL_Surface> _surface;

    void draw_line(double x1, double y1, double x2, double y2, double size);
    void draw_point(double x, double y, double size);

};

#endif
