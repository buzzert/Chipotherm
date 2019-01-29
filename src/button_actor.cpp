/*
 * Created on Sun Jan 27 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#include "button_actor.h"
#include "palette.h"

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

void ButtonActor::mouse_down(int x, int y)
{
    set_highlighted(true);
}

void ButtonActor::mouse_up(int x, int y)
{
    set_highlighted(false);
    if (rect.point_inside(x, y)) {
        clicked();
    }
}

void ButtonActor::render(cairo_t *cr, Rect at_rect)
{
    // Draw background
    double width = rect.width;
    double height = rect.height;
    double cornerRadius = 16.0;
    double padding = 8.0;

    Palette::draw_rounded_rect(cr, get_bounds().inset_by(padding, padding), cornerRadius);

    _foreground_color.set_source(cr);
    if (_highlighted) {
        _label->set_foreground_color(Color(0x00, 0x00, 0x00, 0xFF));
        cairo_fill(cr);
    } else {
        _label->set_foreground_color(_foreground_color);
        cairo_set_line_width(cr, 2.0);
        cairo_stroke(cr);
    }

    Actor::render(cr, at_rect);
}
