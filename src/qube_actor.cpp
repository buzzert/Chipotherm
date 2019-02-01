/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#include "qube_actor.h"

#include <iostream>

QubeActor::QubeActor(Rect rect)
    : Actor(rect), _color(Color(0xFF, 0x00, 0x00, 0xFF))
{
}

void QubeActor::update()
{
    _time_offset += (0.01 * _rotation_speed);
    set_needs_display();
}

void QubeActor::render(cairo_t *cr, Rect at_rect)
{
    Actor::render(cr, at_rect);

    double radius = MIN(rect.width, rect.height) / 2.0;
    draw_tetrahedron(cr, rect.width / 2, rect.height / 2, radius, _time_offset);
}

void QubeActor::draw_line(cairo_t *cr, double x1, double y1, double x2, double y2, double size)
{
    cairo_set_line_width(cr, size);
    cairo_move_to(cr, x1, y1);
    cairo_line_to(cr, x2, y2);
    cairo_stroke(cr);
}

void QubeActor::draw_point(cairo_t *cr, double x, double y, double size)
{
    cairo_arc(cr, x, y, size, 0, 2 * M_PI);
    cairo_fill(cr);
}

void QubeActor::draw_mesh(
    cairo_t *cr,
    double x, double y, 
    std::vector<Vec3> vertices, 
    std::vector<std::pair<unsigned int, unsigned int>> edges,
    double scale,
    Color color,
    double time_offset)
{
    double point_size = 5.0;
    double line_size = 2.0;

    // Calculate points, but don't draw yet (needs to draw on top of lines)
    for (Vec3 &point : vertices) {
        point = rotate_x(point, time_offset / 2);
        point = rotate_y(point, time_offset / 3);
        point = rotate_z(point, time_offset / 5);
    }

    // Draw lines connecting the points
    _color.set_source(cr);
    for (std::pair<unsigned int, unsigned int> edge : edges) {
        double x1 = vertices[edge.first].x * scale;
        double y1 = vertices[edge.first].y * scale;

        double x2 = vertices[edge.second].x * scale;
        double y2 = vertices[edge.second].y * scale;

        draw_line(cr, x + x1, y + y1, x + x2, y + y2, line_size);
    }

    // Now draw points
    for (const Vec3 &point : vertices) {
        draw_point(cr, x + point.x * scale, y + point.y * scale, point_size);
    }
}

void QubeActor::draw_tetrahedron(cairo_t *cr, double cx, double cy, double radius, double time_offset)
{
    double q = 0.5;

    std::vector<Vec3> vertices = {
        { q, q, q},
        { q,-q,-q},
        {-q, q,-q},
        {-q,-q, q}
    };

    std::vector<std::pair<unsigned int, unsigned int>> edges = {
        { 0, 1 },
        { 0, 2 },
        { 0, 3 },
        { 1, 2 },
        { 2, 3 },
        { 3, 1 }
    };

    draw_mesh(cr, cx, cy, vertices, edges, radius, Color(0, 0, 0xFF, 0xFF), time_offset);
}
