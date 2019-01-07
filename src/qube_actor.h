/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#pragma once 

#include <bubbles/bubbles.h>

#include <cairo/cairo.h>
#include <memory>
#include <vector>

using namespace Bubbles;

class QubeActor : public CairoActor
{
public:
    QubeActor(Rect rect);

    void update(SDL_Renderer *renderer) override;
    void render(SDL_Renderer *renderer, Rect at_rect) override;

private:
    double _time_offset;

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

