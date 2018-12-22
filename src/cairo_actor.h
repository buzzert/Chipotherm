/*
 * Created on Sat Dec 22 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#pragma once

#include "actor.h"
#include <memory>
#include <cairo/cairo.h>

class CairoActor : public Actor
{
public:
    CairoActor(Rect r);

    void render(SDL_Renderer *renderer) override;

protected:
    std::shared_ptr<cairo_t>     _cairo_ctx;
    std::shared_ptr<SDL_Surface> _surface;
};

