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

DEF_SHARED_PTR(QubeActor)

class QubeActor : public Actor
{
public:
    QubeActor(Rect rect);

    void update() override;
    void render(cairo_t *cr, Rect at_rect) override;

    Color get_color() const { return _color; }
    void  set_color(Color c) { _color = c; }

    void set_rotation_speed(float speed) { _rotation_speed = speed; };

private:
    float  _rotation_speed = 1.0;
    double _time_offset = 0.0;
    Color  _color;

    void draw_line(cairo_t *cr, double x1, double y1, double x2, double y2, double size);
    void draw_point(cairo_t *cr, double x, double y, double size);
    void draw_mesh(
        cairo_t *cr,
        double x, double y, 
        std::vector<Vec3> vertices, 
        std::vector<std::pair<unsigned int, unsigned int>> edges,
        double scale,
        Color color,
        double time_offset
    );
    void draw_tetrahedron(cairo_t *cr, double cx, double cy, double radius, double time_offset);
};

