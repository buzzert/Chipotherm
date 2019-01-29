/*
 * Created on Mon Jan 28 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#include "palette.h"

#include <cairo/cairo.h>
#include <bubbles/core/geometry.h>

namespace Palette {
    void draw_rounded_rect(cairo_t *cr,   double width, 
                                  double height, double radius, double padding)
    {
        width -= padding * 2.0;
        height -= padding * 2.0;

        cairo_new_sub_path(cr);
        cairo_arc(cr, padding + width - radius, padding + radius, radius, -M_PI_2, 0);
        cairo_arc(cr, padding + width - radius, padding + height - radius, radius, 0, M_PI_2);
        cairo_arc(cr, padding + radius, padding + height - radius, radius, M_PI_2, M_PI);
        cairo_arc(cr, padding + radius, padding + radius, radius, M_PI, 1.5 * M_PI);
        cairo_close_path(cr);
    }
}
