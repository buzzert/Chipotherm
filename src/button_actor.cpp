/*
 * Created on Sun Jan 27 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#include "button_actor.h"

BUBBLES_NAMESPACE_BEGIN

ButtonActor::ButtonActor(Rect r)
    : Actor(r), _label(RECT_ZERO, ""), _foreground_color(Color(0xFF, 0xFF, 0xFF, 0xFF)),
      _grid(RECT_ZERO, 1)
{
    ActorPtr grid_p = std::shared_ptr<Actor>(&_grid);
    add_subactor(grid_p);

    ActorPtr label_p = std::shared_ptr<Actor>(&_label);
    _grid.stack_actor(label_p, 0);
}

void ButtonActor::set_label_text(const std::string &str)
{
    _label.set_contents(str);
}

void ButtonActor::render(cairo_t *cr, Rect at_rect)
{
    // Draw background
    double width = rect.width;
    double height = rect.height;
    double cornerRadius = height / 10.0;
    double padding = 8.0;
    width -= padding * 2.0;
    height -= padding * 2.0;

    cairo_new_sub_path(cr);
    cairo_arc(cr, padding + width - cornerRadius, padding + cornerRadius, cornerRadius, -M_PI_2, 0);
    cairo_arc(cr, padding + width - cornerRadius, padding + height - cornerRadius, cornerRadius, 0, M_PI_2);
    cairo_arc(cr, padding + cornerRadius, padding + height - cornerRadius, cornerRadius, M_PI_2, M_PI);
    cairo_arc(cr, padding + cornerRadius, padding + cornerRadius, cornerRadius, M_PI, 1.5 * M_PI);
    cairo_close_path(cr);

    _foreground_color.set_source(cr);
    if (_highlighted) {
        cairo_fill(cr);
    } else {
        cairo_set_line_width(cr, 2.0);
        cairo_stroke(cr);
    }

    Actor::render(cr, at_rect);
}

BUBBLES_NAMESPACE_END
