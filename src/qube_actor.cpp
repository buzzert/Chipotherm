#include "qube_actor.h"

#include "color.h"
#include <SDL2/SDL.h>

QubeActor::QubeActor(Rect rect)
    : Actor(rect)
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

void QubeActor::update(SDL_Renderer *renderer)
{

}

void QubeActor::render(SDL_Renderer *renderer)
{
    cairo_set_source_rgb(_cairo_ctx.get(), 0.0, 1.0, 0.0);
    draw_point(50.0, 50.0, 50.0);

    // TODO: three classes doing this same thing now
    SDL_Rect dst_rect = rect.to_sdl_rect();

    // slow
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, _surface.get());
    SDL_SetTextureAlphaMod(texture, (alpha * 255));
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);
    SDL_RenderCopy(renderer, texture, NULL, &dst_rect);
}

void QubeActor::draw_line(double x1, double y1, double x2, double y2, double size)
{
    cairo_t *cr = _cairo_ctx.get();
    cairo_set_line_width(cr, size);
    cairo_move_to(cr, x1, y1);
    cairo_line_to(cr, x2, y2);
    cairo_stroke(cr);
}

void QubeActor::draw_point(double x, double y, double size)
{
    cairo_t *cr = _cairo_ctx.get();
    cairo_arc(cr, x, y, size, 0, 2 * M_PI);
    cairo_fill(cr);
}
