/*
 * Created on Sun Jan 06 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#include "clock_actor.h"
#include "palette.h"

ClockActor::ClockActor(Rect r)
    : Actor(r)
{
    _label = std::make_shared<LabelActor>(r, "");
    _label->set_foreground_color(Palette::foreground);
    _label->set_alignment(PANGO_ALIGN_CENTER);
    _label->set_font_prop("Input Mono 19");
    add_subactor(_label);
}

void ClockActor::update()
{
    Actor::update();

    struct tm *timeinfo;
    time_t now_time;
    std::string time_str(32, 0);
    TimePoint now = Clock::now();
    if ( (now - last_update) > std::chrono::seconds(1) ) {
        last_update = now;

        time(&now_time);
        timeinfo = localtime(&now_time);
        strftime(&time_str[0], time_str.size(), "%H:%M:%S", timeinfo);

        _label->set_contents(time_str);
    }
}

void ClockActor::render(cairo_t *cr, Rect at_rect)
{
    Actor::render(cr, at_rect);

    /*
    const double padding = 10.0;
    const double line_width = 2.0;
    cairo_set_line_width(cr, line_width);
    cairo_set_source_rgba(cr, CAIRO_COLOR(Palette::foreground));
    cairo_move_to(cr, padding, rect.height - line_width - padding);
    cairo_line_to(cr, rect.width - (padding * 2.0), rect.height - line_width - padding);
    cairo_stroke(cr);
    */
}

void ClockActor::layout_actors()
{
    Rect bounds = get_rect();

    Rect label_rect(0, 0, bounds.width, bounds.height);
    _label->set_rect(label_rect);
}

