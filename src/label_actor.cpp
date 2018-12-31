/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#include "label_actor.h"

#include <iostream>
#include <SDL2/SDL.h>
#include <pango/pangocairo.h>

#define DEFAULT_FONT_PROP "Karla 24"

LabelActor::LabelActor(Rect rect, std::string contents)
    : CairoActor(rect), _contents(contents), 
      _foreground_color(0xFF, 0x00, 0x00, 0xFF),
      _font_prop(DEFAULT_FONT_PROP),
      _needs_texture_update(true)
{
}

LabelActor::~LabelActor()
{

}

void LabelActor::set_contents(std::string &contents)
{
    _contents = contents;
    _needs_texture_update = true;
}

const std::string& LabelActor::get_contents() const
{
    return _contents;
}

void LabelActor::set_foreground_color(Color &c)
{
    _foreground_color = c;
    _needs_texture_update = true;
}

const Color& LabelActor::get_foreground_color() const
{
    return _foreground_color;
}

void LabelActor::display_surface()
{
    _needs_texture_update = false;

    PangoLayout *pango_layout = pango_cairo_create_layout(_cairo_ctx.get());

    PangoFontDescription *pango_desc = pango_font_description_from_string(_font_prop.c_str());
    pango_layout_set_font_description(pango_layout, pango_desc);
    pango_font_description_free(pango_desc);
    
    pango_layout_set_markup(pango_layout, _contents.c_str(), -1);

    Color &color = _foreground_color;
    cairo_set_source_rgb(_cairo_ctx.get(), color.red, color.green, color.blue);

    cairo_move_to(_cairo_ctx.get(), 0.0, 0.0);
    pango_cairo_show_layout(_cairo_ctx.get(), pango_layout);
    g_object_unref(pango_layout);
}
