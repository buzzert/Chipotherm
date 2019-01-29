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

    Rect scaledCanvasRect(0, 0, canvasWidth * scale, canvasHeight * scale);

    MainScene mainScene(scaledCanvasRect, windowed);
    mainScene.set_scale(scale);
    if (!windowed) {
        mainScene.set_hides_cursor(true);
    }

    Rect canvasRect(0, 0, canvasWidth, canvasHeight);
    auto main_grid = std::make_shared<ActorGrid>(canvasRect, 1);
    main_grid->set_orientation(ActorGrid::Orientation::VERTICAL);
    mainScene.add_actor(main_grid);

    auto status_grid = std::make_shared<ActorGrid>(RECT_ZERO, 1);
    status_grid->set_orientation(ActorGrid::Orientation::HORIZONTAL);
    main_grid->stack_actor(status_grid, 0, 30.0);

    auto title_label = std::make_shared<LabelActor>(RECT_ZERO, " XION HVAC System Control Panel");
    title_label->set_font_prop("Karla 11");
    title_label->set_foreground_color(Palette::foreground);
    status_grid->stack_actor(title_label, 0);

    auto status_label = std::make_shared<LabelActor>(RECT_ZERO, "ONLINE");
    status_label->set_alignment(PangoAlignment::PANGO_ALIGN_RIGHT);
    status_label->set_font_prop("Karla 10");
    status_label->set_foreground_color(Color(0x00, 0xFF, 0x12, 0xFF));
    status_grid->stack_actor(status_label, 0);

    auto grid = std::make_shared<ActorGrid>(Rect(0, 0, canvasWidth, canvasHeight), 2);
    main_grid->stack_actor(grid, 0);

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
    button->set_foreground_color(Color(0xFF, 0x00, 0x00, 0xFF));
    button->clicked.connect(sigc::slot<void>([]() {
        printf("button clicked\n");
    }));
    grid->stack_actor(button, 1, 160);

    auto subgrid = std::make_shared<ActorGrid>(RECT_ZERO, 1);
    subgrid->set_orientation(ActorGrid::Orientation::HORIZONTAL);

    auto minus_button = std::make_shared<ButtonActor>(RECT_ZERO);
    minus_button->set_label_text("-");
    minus_button->set_foreground_color(Palette::foreground);
    subgrid->stack_actor(minus_button, 0);

    auto plus_button = std::make_shared<ButtonActor>(RECT_ZERO);
    plus_button->set_foreground_color(Palette::foreground);
    plus_button->set_label_text("+");
    subgrid->stack_actor(plus_button, 0);

    grid->stack_actor(subgrid, 1);

    // Start scene's runloop
    mainScene.run();
    return 0;
}
