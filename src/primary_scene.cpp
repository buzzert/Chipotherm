/*
 * Created on Mon Jan 28 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#include "primary_scene.h"

#include <memory>

#include "button_actor.h"
#include "clock_actor.h"
#include "graph_actor.h"
#include "palette.h"
#include "qube_actor.h"
#include "rounded_title_actor.h"

ActorGridPtr PrimaryScene::initialize_status_grid()
{
    auto status_grid = std::make_shared<ActorGrid>(RECT_ZERO, 1);
    status_grid->set_orientation(ActorGrid::Orientation::HORIZONTAL);

    auto title_label = std::make_shared<LabelActor>(RECT_ZERO, " XION HVAC System Control Panel");
    title_label->set_font_prop("Karla 11");
    title_label->set_foreground_color(Palette::foreground);
    status_grid->stack_actor(title_label, 0);

    auto status_label = std::make_shared<LabelActor>(RECT_ZERO, "ONLINE");
    status_label->set_alignment(PangoAlignment::PANGO_ALIGN_RIGHT);
    status_label->set_font_prop("Karla 10");
    status_label->set_foreground_color(Color(0x00, 0xFF, 0x12, 0xFF));
    status_grid->stack_actor(status_label, 0);

    return status_grid;
    //main_grid->stack_actor(status_grid, 0, 30.0);
}

ActorGridPtr PrimaryScene::initialize_statistics_grid()
{
    auto grid = std::make_shared<ActorGrid>(RECT_ZERO, 1);
    grid->set_padding(10.0);

    // Clock
    auto clock_actor = std::make_shared<ClockActor>(Rect());
    grid->stack_actor(clock_actor, 0, 50.0);

    // Temperatures 
    {
        auto temp_stack = std::make_shared<ActorGrid>(RECT_ZERO, 1);
        temp_stack->set_orientation(ActorGrid::Orientation::HORIZONTAL);
        temp_stack->set_padding(5.0);
        grid->stack_actor(temp_stack, 0, -1);

        auto qube3 = std::make_shared<QubeActor>(Rect());
        temp_stack->stack_actor(qube3, 0);

        auto current = std::make_shared<RoundedTitleActor>(RECT_ZERO, "CURRENT");
        current->get_label()->set_contents("69");
        current->set_foreground_color(Palette::foreground);
        current->set_filled(true);
        temp_stack->stack_actor(current, 0);

        auto target = std::make_shared<RoundedTitleActor>(RECT_ZERO, "TARGET");
        target->pulsing = true;
        target->get_label()->set_contents("80");
        target->set_foreground_color(Color(0xFF, 0x00, 0x00, 0xFF));
        target->set_filled(true);
        temp_stack->stack_actor(target, 0);
    }

    // Graph
    auto graph_actor = std::make_shared<GraphActor>(Rect());
    std::srand(std::time(nullptr));
    for (unsigned i = 0; i < 20; i++) {
        double val = fmod(std::rand(), 100);
        graph_actor->add_sample(val);
    }
    grid->stack_actor(graph_actor, 0);

    return grid;
}

ActorGridPtr PrimaryScene::initialize_controls_grid()
{
    auto grid = std::make_shared<ActorGrid>(RECT_ZERO, 1);

    // Heat ON button 
    {
        auto heat_button = std::make_shared<ButtonActor>(RECT_ZERO);
        heat_button->set_label_text("HEAT ON");
        heat_button->set_foreground_color(Color(0xFF, 0x00, 0x00, 0xFF));
        heat_button->clicked.connect(sigc::slot<void>([]() {
            printf("button clicked\n");
        }));
        grid->stack_actor(heat_button, 0, 160);
    }

    // Temperature adjustment
    {
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

        grid->stack_actor(subgrid, 0);
    }

    return grid;
}

PrimaryScene::PrimaryScene(Rect canvas_rect, bool windowed, double scale)
    : MainScene(canvas_rect, windowed, scale)
{
    Rect canvasRect(0, 0, canvas_rect.width, canvas_rect.height);

    auto main_grid = std::make_shared<ActorGrid>(canvasRect, 1);
    main_grid->set_orientation(ActorGrid::Orientation::VERTICAL);
    add_actor(main_grid);

    // Add status bar
    ActorGridPtr status_grid = initialize_status_grid();
    main_grid->stack_actor(status_grid, 0, 30.0);

    // Two panel stats and controls
    {
        ActorGridPtr panel_grid = std::make_shared<ActorGrid>(RECT_ZERO, 1);
        panel_grid->set_orientation(ActorGrid::Orientation::HORIZONTAL);
        main_grid->stack_actor(panel_grid, 0);

        // Statistics (left side)
        ActorGridPtr stats_grid = initialize_statistics_grid();
        panel_grid->stack_actor(stats_grid, 0);

        // Controls (right side)
        ActorGridPtr controls_grid = initialize_controls_grid();
        panel_grid->stack_actor(controls_grid, 0);
    }


}
