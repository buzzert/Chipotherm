/*
 * Created on Sat Dec 22 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#include "cairo_actor.h"

CairoActor::CairoActor(Rect r)
    : TextureActor(r)
{
    generate_surface();
}

void CairoActor::generate_surface()
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

    needs_display = false;
}

void CairoActor::render(SDL_Renderer *renderer)
{
    if (needs_display) {
        generate_surface();
    }

    // TODO: three classes doing this same thing now
    SDL_Rect dst_rect = rect.to_sdl_rect();

    if (texture.get() == NULL) {
        SDL_Texture *textureptr = SDL_CreateTextureFromSurface(renderer, _surface.get());
        texture = std::shared_ptr<SDL_Texture>(textureptr, SDL_DestroyTexture);
    } else {
        SDL_UpdateTexture(texture.get(), NULL, _surface->pixels, _surface->pitch);
    }

    TextureActor::render(renderer);
}
