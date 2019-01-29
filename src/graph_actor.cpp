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

    const double line_width = 2.0;
    cairo_set_line_width(cr, line_width);
    cairo_set_source_rgba(cr, CAIRO_COLOR(Color(0xCA, 0xED, 0xEF, 0xFF)));

    const double height = rect.height;
    const double width = rect.width;
    const double sample_width = 16.0;

    // Compute normalized height
    double mean_val = 0.0;
    double low_val = std::numeric_limits<double>::infinity();
    double high_val = 0.0;
    for (Sample &s : _samples) {
        double val = s.value;
        mean_val += val;
        if (val < low_val) low_val = val;
        if (val > high_val) high_val = val;
    }
    mean_val /= _samples.size();

    double scale_val = (1.0 / MAX(1.0, (high_val - low_val)));

    const double y_padding = 10.0;
    double x_offset = width;
    for (unsigned i = 0; i < _samples.size(); i++) {
        unsigned rev_idx = _samples.size() - 1 - i;
        Sample s = _samples.at(rev_idx);

        // "Center" graph about the mean
        double norm = 0.5 + ((-mean_val + s.value) * (scale_val / 2.0));
        double render_val = (1.0 - norm) * (height - (2.0 * y_padding));
        if (i == 0) {
            cairo_move_to(cr, x_offset, y_padding + render_val);
        } else {
            cairo_line_to(cr, x_offset, y_padding + render_val);
        }

        x_offset -= sample_width;
        if (x_offset < 0.0) break;
    }

    cairo_stroke(cr);

    // Draw measurement lines
    cairo_set_line_width(cr, 1.0);
    cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.1);
    x_offset = width;
    for (unsigned i = 0; i < _samples.size(); i++) {
        cairo_move_to(cr, x_offset, y_padding);
        cairo_line_to(cr, x_offset, height - y_padding);
        cairo_stroke(cr);

        x_offset -= sample_width;
    }
}

