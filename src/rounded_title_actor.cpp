/*
 * Created on Mon Jan 28 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#include "rounded_title_actor.h"
#include "palette.h"

RoundedTitleActor::RoundedTitleActor(Rect r, std::string title)
    : Actor(r), _foreground_color(Color(0xFF, 0xFF, 0xFF, 0xFF))
{
    _label = std::make_shared<LabelActor>(RECT_ZERO, "");
    _label->set_font_prop("Karla 16");
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
    _label->set_rect(Rect(0, 0, rect.width, rect.height));
}

void RoundedTitleActor::render(cairo_t *cr, Rect in_rect)
{
    // Draw background
    double padding = 8.0;
    double cornerRadius = 16.0;
    Palette::draw_rounded_rect(cr, rect.width, rect.height, cornerRadius, padding);

    _foreground_color.set_source(cr);
    cairo_fill(cr);

    Actor::render(cr, in_rect);
}
