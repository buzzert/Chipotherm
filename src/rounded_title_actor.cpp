/*
 * Created on Mon Jan 28 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#include "rounded_title_actor.h"
#include "palette.h"
#include "utilities.h"

static const double k_label_offset = 10.0;

RoundedTitleActor::RoundedTitleActor(Rect r, std::string title)
    : Actor(r), _foreground_color(0xFF, 0xFF, 0xFF, 0xFF)
{
    _label = std::make_shared<LabelActor>(RECT_ZERO, "");
    _label->set_font_prop("Karla 21");
    _label->set_alignment(PangoAlignment::PANGO_ALIGN_CENTER);
    add_subactor(_label);
    
    _title_label = std::make_shared<LabelActor>(RECT_ZERO, title);
    _title_label->set_font_prop(Palette::sector_label_font);
    _title_label->set_foreground_color(Palette::foreground);
    add_subactor(_title_label);

    Size padding = Palette::sector_label_padding;
    Size label_size = _title_label->get_intrinsic_size();
    Rect title_rect = Rect(padding.width, padding.height, label_size.width, label_size.height);
    _title_label->set_rect(title_rect);
}

void RoundedTitleActor::layout_actors()
{
    _label->set_rect(Rect(0, k_label_offset, rect.width, rect.height - k_label_offset));
}

void RoundedTitleActor::set_foreground_color(Color c)
{
    _foreground_color = c;
    _label->set_foreground_color(c);
}

void RoundedTitleActor::set_filled(bool filled)
{
    _filled = filled;
    if (filled) {
        _label->set_foreground_color(Color(0x00, 0x00, 0x00, 0xFF));
    } else {
        _label->set_foreground_color(_foreground_color);
    }
}

void RoundedTitleActor::update()
{
    Actor::update();

    if (!pulsing) return;

    double now = Utilities::time_now();
    const double pulse_duration = 0.4;
    double progress = (now - _pulse_begin) / pulse_duration;
    if (!_pulse_direction) {
        _pulse_progress = MAX(0.0, 1.0 - progress);
    } else {
        _pulse_progress = MIN(1.0, progress);
    }
    
    if (progress > 1.0) {
        _pulse_direction = !_pulse_direction;
        _pulse_begin = now;
    }
}

void RoundedTitleActor::render(cairo_t *cr, Rect in_rect)
{
    // Draw background
    double padding = 8.0;
    Rect background_rect = Rect(0.0, k_label_offset, rect.width, rect.height - k_label_offset).inset_by(padding, padding);
    Palette::draw_rounded_rect(cr, background_rect, Palette::corner_radius);

    _foreground_color.set_source(cr);
    if (pulsing) {
        Color fg = _foreground_color;
        fg.alpha = _pulse_progress * 255;
        cairo_set_source_rgba(cr, CAIRO_COLOR(fg));

        double inv_progress = (1.0 - _pulse_progress);
        Color bg = _foreground_color;
        bg.red = 255 * inv_progress;
        _label->set_foreground_color(bg);
    }

    if (_filled) {
        cairo_fill(cr);
    } else {
        cairo_stroke(cr);
    }

    Actor::render(cr, in_rect);
}
