/*
 * Created on Fri Jan 25 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#include "graph_actor.h"
#include "palette.h"

#include <limits>

GraphActor::GraphActor(Rect r)
    : Actor(r)
{
    _label = std::make_shared<LabelActor>(RECT_ZERO, "HISTORICAL DATA");
    _label->set_font_prop(Palette::sector_label_font);
    _label->set_foreground_color(Palette::foreground);
    add_subactor(_label);
}

void GraphActor::layout_actors()
{
    Size padding = Palette::sector_label_padding;
    Size intrinsic_size = _label->get_intrinsic_size();
    Rect label_rect = Rect(padding.width, padding.height, intrinsic_size.width, intrinsic_size.height);
    _label->set_rect(label_rect);
}

void GraphActor::add_sample(double value)
{
    Sample s = { .value = value };
    _samples.push_back(s);
}

void GraphActor::render(cairo_t *cr, Rect at_rect)
{
    Actor::render(cr, at_rect);

    const double line_width = 1.0;
    cairo_set_line_width(cr, line_width);
    cairo_set_source_rgba(cr, CAIRO_COLOR(Palette::foreground));

    const double height = rect.height;
    const double width = rect.width;
    const double sample_width = 16.0;

    // Compute normalized height
    double low_val = std::numeric_limits<double>::infinity();
    double high_val = 0.0;
    for (Sample &s : _samples) {
        double val = s.value;
        if (val < low_val) low_val = val;
        if (val > high_val) high_val = val;
    }

    double scale_val = (1.0 / high_val);

    double x_offset = width;
    for (unsigned i = 0; i < _samples.size(); i++) {
        unsigned rev_idx = _samples.size() - 1 - i;
        Sample s = _samples.at(rev_idx);

        double normalized_val = (-low_val + s.value) * scale_val;
        double render_val = (1.0 - normalized_val) * height;
        if (i == 0) {
            cairo_move_to(cr, x_offset, render_val);
        } else {
            cairo_line_to(cr, x_offset, render_val);
        }

        x_offset -= sample_width;
        if (x_offset < 0.0) break;
    }

    cairo_stroke(cr);
}

