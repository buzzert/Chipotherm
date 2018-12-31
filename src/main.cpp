/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#include <SDL2/SDL.h>

#include <cassert>
#include <libgen.h>
#include <memory>

#include "actor_grid.h"
#include "main_scene.h"
#include "label_actor.h"
#include "qube_actor.h"

int main(int argc, char **argv)
{
    // CHIP resolution is 480x272
    const int canvasWidth = 480;
    const int canvasHeight = 272;
    const float scale = 2.0; // for testing on hiDPI

    bool windowed = false;
    if (argc > 1 && strcmp("-w", argv[1]) == 0) {
        windowed = true;
    }

    Rect canvasRect(0, 0, canvasWidth * scale, canvasHeight * scale);

    MainScene mainScene(canvasRect, windowed);
    mainScene.set_scale(scale);

    auto grid = std::make_shared<ActorGrid>(Rect(0, 0, canvasWidth, canvasHeight), 2);

    auto qube1 = std::make_shared<QubeActor>(Rect());
    grid->stack_actor(qube1, 0, -1);
    
    auto qube2 = std::make_shared<QubeActor>(Rect());
    grid->stack_actor(qube2, 1, -1);

    auto qube3 = std::make_shared<QubeActor>(Rect());
    grid->stack_actor(qube3, 1, -1);

    mainScene.add_actor(grid);

    bool running = true;
    const int kTicksPerFrame = 1000 / 60;

    while (running) {
        Uint32 startTime = SDL_GetTicks();

        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }

        mainScene.update();
        mainScene.render();

        Uint32 ticks = SDL_GetTicks() - startTime;
        if (ticks < kTicksPerFrame) {
            SDL_Delay(kTicksPerFrame - ticks);
        }
    }

    return 0;
}
