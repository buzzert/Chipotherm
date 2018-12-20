#pragma once

#include "actor.h"

#include <string>
#include <memory>

std::shared_ptr<SDL_Texture> TextureForRes(std::string const &resourceName, SDL_Renderer *renderer);

class TextureActor : public Actor
{
public:
    TextureActor(std::shared_ptr<SDL_Texture> texture);
    ~TextureActor() override {};

    void render(SDL_Renderer *renderer) override;

    std::shared_ptr<SDL_Texture> texture;
};

