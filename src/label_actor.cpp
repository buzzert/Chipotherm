/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#include "label_actor.h"

#include <iostream>
#include <SDL2/SDL.h>

extern "C" {
#include "SDL_Pango.h"
}

LabelActor::LabelActor(Rect rect, std::string contents)
    : Actor(rect), _contents(contents), _foreground_color(0xFF, 0x00, 0x00, 0xFF)
{
    _needs_texture_update = true;
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

void LabelActor::render(SDL_Renderer *renderer)
{
    SDL_Rect dst_rect = rect.to_sdl_rect();

    SDL_SetTextureAlphaMod(_texture.get(), (alpha * 255));
    SDL_SetTextureBlendMode(_texture.get(), SDL_BLENDMODE_ADD);
    SDL_RenderCopy(renderer, _texture.get(), NULL, &dst_rect);
}

void LabelActor::update(SDL_Renderer *renderer)
{
    if (_needs_texture_update) {
        update_texture(renderer);
    }
}

void LabelActor::update_texture(SDL_Renderer *renderer)
{
    _needs_texture_update = false;

    SDLPango_Context *context = SDLPango_CreateContext();

    SDL_Color color = _foreground_color.to_sdl_color();
    SDLPango_SetColor(context, &color);
    SDLPango_SetMinimumSize(context, rect.width, rect.height);

    SDLPango_SetMarkup(context, _contents.c_str(), -1);

    Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    SDL_Surface *surface = SDL_CreateRGBSurface(
        0, rect.width, rect.height, 32, rmask, gmask, bmask, amask
    );

    if (!surface) {
        std::cerr << "Error creating surface for label: "
            << SDL_GetError() << std::endl;
        
        return;
    }

    SDLPango_Draw(context, surface, 0, 0);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "Error creating texture for label: "
            << SDL_GetError() << std::endl;
        
        return;
    }    

    SDL_FreeSurface(surface);

    _texture = std::shared_ptr<SDL_Texture>(texture, SDL_DestroyTexture);
}
