/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#pragma once

#include "actor.h"

#include <vector>
#include <map>
#include <chrono>
#include <memory>

typedef std::chrono::high_resolution_clock::time_point update_time;

class MainScene
{
public:
    MainScene(Rect canvasRect, bool windowed);
    ~MainScene();

    void add_actor(ActorPtr actor);
    void set_scale(float scale);

    void update();
    void render();

private:
    SDL_Window   *_window;
    SDL_Renderer *_renderer;

    std::vector<ActorPtr> _actors;

    Rect _canvasRect;
};

