#include "texture_actor.h"

#include <SDL2/SDL_image.h>
#include <iostream>

std::shared_ptr<SDL_Texture> TextureForRes(std::string const &resourceName, SDL_Renderer *renderer)
{
    // TODO: resources (use gresources?)
    std::string const fullResourceName(resourceName);

    SDL_Surface *imageSurface = IMG_Load(fullResourceName.c_str());
    if (!imageSurface) {
        std::cerr << "WARN: Could not find resource: " << fullResourceName << std::endl;
        return NULL;
    }

    SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);

    return std::shared_ptr<SDL_Texture>(imageTexture, SDL_DestroyTexture);
}

TextureActor::TextureActor(std::shared_ptr<SDL_Texture> texture)
    : Actor(Rect(0, 0, 0, 0)), texture(texture)
{
    int width, height;
    SDL_QueryTexture(texture.get(), NULL, NULL, &width, &height);

    rect.width = width;
    rect.height = height;
}

void TextureActor::render(SDL_Renderer *renderer)
{
    SDL_Rect dst_rect = rect.to_sdl_rect();

    SDL_SetTextureAlphaMod(texture.get(), (alpha * 255));
    SDL_SetTextureBlendMode(texture.get(), SDL_BLENDMODE_ADD);
    SDL_RenderCopy(renderer, texture.get(), NULL, &dst_rect);
}
