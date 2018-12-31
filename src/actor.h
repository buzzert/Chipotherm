/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#pragma once

#include "geometry.h"
#include <memory>

#define DEFAULT_RGBA_MASK 0x00FF0000, 0x0000FF00, 0x000000FF, 0x00

class Actor;
using ActorPtr = std::shared_ptr<Actor>;

class Actor
{
public:
    Actor(Rect rect);
    virtual ~Actor() {};

    virtual void update(SDL_Renderer *renderer) {};
    virtual void render(SDL_Renderer *renderer) {};

    void set_rect(Rect r);

protected: 
    Rect  rect;
    float alpha;
    bool  needs_display;
};
