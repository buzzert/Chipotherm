/*
 * Created on Tue Jan 08 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#pragma once

#include <bubbles/core/color.h>
#include <bubbles/core/geometry.h>

namespace Palette {
    static const Bubbles::Color foreground(0x71, 0xa6, 0xfc, 0xff);

    static const std::string sector_label_font("Exan 8");
    static const Bubbles::Size sector_label_padding(5, 5);

    void draw_rounded_rect(cairo_t *cr,   double width, 
                           double height, double radius, double padding);
}
