/*
 * Created on Mon Jan 28 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#include "palette.h"

#include <cairo/cairo.h>
#include <bubbles/core/geometry.h>

namespace Palette {
    void draw_rounded_rect(cairo_t *cr, Bubbles::Rect rect, double radius)
    {
        Bubbles::Vec2 point(rect.x, rect.y);
        double width = rect.width;
        double height = rect.height;

        cairo_new_sub_path(cr);
        // Top right corner
        cairo_arc(cr, 
            point.x + width - radius, point.y + radius, 
            radius, -M_PI_2, 0
        );

        // Bottom right corner
        cairo_arc(cr, 
            point.x + width - radius, point.y + height - radius, 
            radius, 0, M_PI_2
        );

        // Bottom left corner
        cairo_arc(cr, 
            point.x + radius, point.y + height - radius, 
            radius, M_PI_2, M_PI
        );

        // Top left corner
        cairo_arc(cr, 
            point.x + radius, point.y + radius, 
            radius, M_PI, 1.5 * M_PI
        );
        cairo_close_path(cr);
    }
}
