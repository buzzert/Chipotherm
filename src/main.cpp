/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#include <cassert>
#include <cstdlib>
#include <libgen.h>
#include <memory>
#include <string.h>

#include "clock_actor.h"
#include "graph_actor.h"
#include "palette.h"
#include "qube_actor.h"
#include <bubbles/bubbles.h>

using namespace Bubbles;

int main(int argc, char **argv)
{
    // CHIP resolution is 480x272
    const int canvasWidth = 480;
    const int canvasHeight = 272;
    const float scale = 2.0; // for testing on hiDPI

    bool windowed = true;
    if (argc > 1 && strcmp("-w", argv[1]) == 0) {
        windowed = true;
    }

    Rect canvasRect(0, 0, canvasWidth * scale, canvasHeight * scale);

    MainScene mainScene(canvasRect, windowed);
    mainScene.set_scale(scale);
    if (!windowed) {
        mainScene.set_hides_cursor(true);
    }

    auto grid = std::make_shared<ActorGrid>(Rect(0, 0, canvasWidth, canvasHeight), 2);

    auto clock_actor = std::make_shared<ClockActor>(Rect());
    grid->stack_actor(clock_actor, 0, -1);

    auto graph_actor = std::make_shared<GraphActor>(Rect());
    std::srand(std::time(nullptr));
    for (unsigned i = 0; i < 100; i++) {
        double val = fmod(std::rand(), 100);
        graph_actor->add_sample(val);
    }
    grid->stack_actor(graph_actor, 0);

    auto label = std::make_shared<LabelActor>(Rect(), "CHIP thermostat");
    label->set_font_prop("Karla 18");
    label->set_foreground_color(Color(0xFF, 0x00, 0xFF, 0xFF));
    grid->stack_actor(label, 1, -1);

    auto qube3 = std::make_shared<QubeActor>(Rect());
    grid->stack_actor(qube3, 1, -1);

    mainScene.add_actor(grid);

    // Start scene's runloop
    mainScene.run();
    return 0;
}
