/*
 * Created on Sun Jan 06 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#include "clock_actor.h"

BUBBLES_NAMESPACE_BEGIN

ClockActor::ClockActor(Rect r)
    : CairoActor(r)
{
    _label = std::make_shared<LabelActor>(r, "hey");
    _label->set_alignment(PANGO_ALIGN_CENTER);
    add_subactor(_label);
}

void ClockActor::update(SDL_Renderer *renderer)
{
    CairoActor::update(renderer);

    TimePoint now = Clock::now();
    if ( (now - last_update) > std::chrono::seconds(1) ) {
        last_update = now;

        time_t now_time;
        struct tm *timeinfo;

        std::string time_str(32, 0);
        time(&now_time);
        timeinfo = localtime(&now_time);
        strftime(&time_str[0], time_str.size(), "%H:%M:%S", timeinfo);

        _label->set_contents(time_str);
    }
}

void ClockActor::display_surface()
{
    CairoActor::display_surface();

    cairo_t *cr = _cairo_ctx.get();

    const double padding = 10.0;
    const double line_width = 2.0;
    cairo_set_line_width(cr, line_width);
    cairo_set_source_rgb(cr, 0.0, 0.0, 1.0);
    cairo_move_to(cr, padding, rect.height - line_width - padding);
    cairo_line_to(cr, rect.width - (padding * 2.0), rect.height - line_width - padding);
    cairo_stroke(cr);
}

void ClockActor::layout_actors()
{
    Rect bounds = get_rect();

    Rect label_rect(0, 0, bounds.width, bounds.height);
    _label->set_rect(label_rect);
}

BUBBLES_NAMESPACE_END

