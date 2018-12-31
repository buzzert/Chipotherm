/*
 * Created on Sat Dec 22 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#pragma once

#include "texture_actor.h"
#include <memory>
#include <cairo/cairo.h>

class CairoActor : public TextureActor 
{
public:
    CairoActor(Rect r);

    void render(SDL_Renderer *renderer) override;

    // This is called when the cairo context needs to be redrawn. Independent from rendering,
    // which simply blits the cairo surface to the SDL renderer. 
    virtual void display_surface() {};
protected:
    std::shared_ptr<cairo_t>     _cairo_ctx;
    std::shared_ptr<SDL_Surface> _surface;

    void generate_surface();
};

