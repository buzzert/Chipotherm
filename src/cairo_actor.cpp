/*
 * Created on Sat Dec 22 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#include "cairo_actor.h"

CairoActor::CairoActor(Rect r)
    : Actor(r)
{
    SDL_Surface *surface = SDL_CreateRGBSurface(0, rect.width, rect.height, 32, DEFAULT_RGBA_MASK);    
    _surface = std::shared_ptr<SDL_Surface>(surface, SDL_FreeSurface);

    cairo_surface_t *cairosurf = cairo_image_surface_create_for_data(
        (unsigned char *)surface->pixels,
        CAIRO_FORMAT_RGB24,
        surface->w,
        surface->h,
        surface->pitch
    );

    cairo_t *cr = cairo_create(cairosurf);
    cairo_surface_destroy(cairosurf);
    _cairo_ctx = std::shared_ptr<cairo_t>(cr, cairo_destroy);
}

void CairoActor::render(SDL_Renderer *renderer)
{
    // TODO: three classes doing this same thing now
    SDL_Rect dst_rect = rect.to_sdl_rect();

    // slow
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, _surface.get());
    SDL_SetTextureAlphaMod(texture, (alpha * 255));
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);
    SDL_RenderCopy(renderer, texture, NULL, &dst_rect);
}
