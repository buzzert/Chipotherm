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

#include "button_actor.h"
#include "clock_actor.h"
#include "graph_actor.h"
#include "palette.h"
#include "qube_actor.h"
#include <bubbles/bubbles.h>
#include <sigc++/sigc++.h>

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
    auto qube3 = std::make_shared<QubeActor>(Rect());
    grid->stack_actor(qube3, 0, -1);

    auto clock_actor = std::make_shared<ClockActor>(Rect());
    grid->stack_actor(clock_actor, 0, -1);

    auto graph_actor = std::make_shared<GraphActor>(Rect());
    std::srand(std::time(nullptr));
    for (unsigned i = 0; i < 100; i++) {
        double val = fmod(std::rand(), 100);
        graph_actor->add_sample(val);
    }
    grid->stack_actor(graph_actor, 0);

    auto button = std::make_shared<ButtonActor>(RECT_ZERO);
    button->set_label_text("HEAT ON");
    button->set_foreground_color(Palette::foreground);
    button->clicked.connect(sigc::slot<void>([]() {
        printf("button clicked\n");
    }));
    grid->stack_actor(button, 1);

    mainScene.add_actor(grid);

    // Start scene's runloop
    mainScene.run();
    return 0;
}
