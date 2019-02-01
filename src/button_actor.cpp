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
    set_background_color(Colors::transparent);

    _label->set_alignment(PangoAlignment::PANGO_ALIGN_CENTER);
    _label->set_background_color(Colors::transparent);
    add_subactor(_label);
}

void ButtonActor::set_highlighted(bool highlighted)
{
    if (_highlighted != highlighted) {
        _highlighted = highlighted;
        set_needs_display();
    }
}

void ButtonActor::set_filled(bool filled)
{
    if (_filled != filled) {
        _filled = filled;
        set_needs_display();
    }
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
    if (get_bounds().point_inside(x, y)) {
        clicked();
    }
}

void ButtonActor::render(cairo_t *cr, Rect at_rect)
{
    // Clear with background color
    clear(cr, Colors::black);

    // Draw background
    double padding = 8.0;
    Palette::draw_rounded_rect(cr, get_bounds().inset_by(padding, padding), Palette::corner_radius);

    if (_filled && !_highlighted) {
        Color fg = _foreground_color;
        cairo_set_source_rgba(cr, fg.red / 255.0, fg.green / 255.0, fg.blue / 255.0, 0.3);
        cairo_fill(cr);
    }

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
