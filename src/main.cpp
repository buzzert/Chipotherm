/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#include <SDL2/SDL.h>

#include <cassert>
#include <libgen.h>
#include <memory>

#include "qube_actor.h"
#include <bubbles/bubbles.h>

using namespace Bubbles;

int main(int argc, char **argv)
{
    const int canvasWidth = 640;
    const int canvasHeight = 480;
    const float scale = 1.0; // for testing on hiDPI

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
    
    auto label = std::make_shared<LabelActor>(Rect(), "This is a test!");
    grid->stack_actor(label, 1, -1);

    auto qube3 = std::make_shared<QubeActor>(Rect());
    grid->stack_actor(qube3, 1, -1);

    mainScene.add_actor(grid);

    // Start scene's runloop
    mainScene.run();
    return 0;
}
