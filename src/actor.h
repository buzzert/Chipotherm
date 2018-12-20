#ifndef ACTOR_H
#define ACTOR_H

#define DEFAULT_RGBA_MASK 0x00FF0000, 0x0000FF00, 0x000000FF, 0x00

#include "SDL2/SDL.h"

struct Vector
{
    float x;
    float y;

    Vector(int x, int y) : x(x), y(y) {};
};

struct Rect
{
    float x;
    float y;

    int width;
    int height;

    Rect() : x(0), y(0), width(0), height(0) {};
    Rect(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {};
    SDL_Rect to_sdl_rect() {
        SDL_Rect rect;
        rect.x = x;
        rect.y = y;
        rect.w = width;
        rect.h = height;

        return rect;
    };
};

class Actor
{
public:
    Actor(Rect rect);
    virtual ~Actor() {};

    virtual void update(SDL_Renderer *renderer) {};
    virtual void render(SDL_Renderer *renderer) {};

    Rect rect;
    float alpha;
};

#endif
