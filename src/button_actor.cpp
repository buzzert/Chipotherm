/*
 * Created on Sun Jan 27 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#include "button_actor.h"

BUBBLES_NAMESPACE_BEGIN

ButtonActor::ButtonActor(Rect r)
    : Actor(r),
      _label(std::make_shared<LabelActor>(RECT_ZERO, "")),
      _foreground_color(Color(0xFF, 0xFF, 0xFF, 0xFF))
{
    _label->set_alignment(PangoAlignment::PANGO_ALIGN_CENTER);
    add_subactor(_label);
}

void ButtonActor::set_label_text(const std::string &str)
{
    _label->set_contents(str);
    _needs_layout = true;
}

void ButtonActor::layout_actors()
{
    Actor::layout_actors();

    Rect bounds = Rect(0, 0, rect.width, rect.height);
    _label->set_rect(bounds);
}

void ButtonActor::render(cairo_t *cr, Rect at_rect)
{
    // Draw background
    double width = rect.width;
    double height = rect.height;
    double cornerRadius = 16.0;
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
        _label->set_foreground_color(_background_color);
        cairo_fill(cr);
    } else {
        _label->set_foreground_color(_foreground_color);
        cairo_set_line_width(cr, 2.0);
        cairo_stroke(cr);
    }

    Actor::render(cr, at_rect);
}

BUBBLES_NAMESPACE_END

