/*
 * Created on Mon Jan 28 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#include "primary_scene.h"

#include <memory>

#include "clock_actor.h"
#include "graph_actor.h"
#include "palette.h"
#include "qube_actor.h"
#include "runloop.h"
#include "utilities.h"

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

        RoundedTitleActorPtr current = std::make_shared<RoundedTitleActor>(RECT_ZERO, "CURRENT");
        current->get_label()->set_contents("69");
        current->set_foreground_color(Palette::foreground);
        current->set_filled(true);
        temp_stack->stack_actor(current, 0);
        _current_temp_indicator = current;

        RoundedTitleActorPtr target = std::make_shared<RoundedTitleActor>(RECT_ZERO, "TARGET");
        target->pulsing = true;
        target->get_label()->set_contents("80");
        target->set_filled(true);
        target->set_foreground_color(Color(0xFF, 0x00, 0x00, 0xFF));
        temp_stack->stack_actor(target, 0);
        _target_temp_indicator = target;
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
        _heat_button = std::make_shared<ButtonActor>(RECT_ZERO);
        _heat_button->set_label_text("HEAT ON");
        _heat_button->set_foreground_color(Color(0xFF, 0x00, 0x00, 0xFF));
        _heat_button->clicked.connect(sigc::slot<void>([this]() {
            bool enabled = _monitor.get_monitoring_enabled();
            _monitor.set_monitoring_enabled(!enabled);
        }));
        grid->stack_actor(_heat_button, 0, 160);
    }

    // Temperature adjustment
    {
        auto subgrid = std::make_shared<ActorGrid>(RECT_ZERO, 1);
        subgrid->set_orientation(ActorGrid::Orientation::HORIZONTAL);

        auto minus_button = std::make_shared<ButtonActor>(RECT_ZERO);
        minus_button->set_label_text("-");
        minus_button->set_foreground_color(Palette::foreground);
        minus_button->clicked.connect(sigc::slot<void>([this]() {
            _monitor.set_target_temperature(_monitor.get_target_temperature() - 1);
            _target_temp_indicator->flash();
            update_ui_state();
        }));
        subgrid->stack_actor(minus_button, 0);

        auto plus_button = std::make_shared<ButtonActor>(RECT_ZERO);
        plus_button->set_foreground_color(Palette::foreground);
        plus_button->set_label_text("+");
        plus_button->clicked.connect(sigc::slot<void>([this]() {
            _monitor.set_target_temperature(_monitor.get_target_temperature() + 1);
            _target_temp_indicator->flash();
            update_ui_state();
        }));
        subgrid->stack_actor(plus_button, 0);

        grid->stack_actor(subgrid, 0);
    }

    return grid;
}

PrimaryScene::PrimaryScene(Rect canvas_rect, bool windowed, double scale)
    : MainScene(canvas_rect, windowed, scale)
{
    _monitor.get_controller().simulate = true;
    _monitor.state_changed.connect(sigc::slot<void, Monitor::State>([this](Monitor::State newstate) {
        update_ui_state();
    }));

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

    update_ui_state();
}

void PrimaryScene::update()
{
    MainScene::update();

    Runloop::main_runloop().iterate();
}

void PrimaryScene::update_ui_state()
{
    float current_temp = _monitor.get_controller().read_temperature();

    std::string temp_string = Utilities::string_val(current_temp);
    _current_temp_indicator->get_label()->set_contents(temp_string);

    float target_temp = _monitor.get_target_temperature();
    _target_temp_indicator->get_label()->set_contents(Utilities::string_val(target_temp));

    bool enabled = _monitor.get_monitoring_enabled();
    bool heat_on = _monitor.get_controller().get_heater_on();
    _target_temp_indicator->pulsing = heat_on;

    if (enabled) {
        _target_temp_indicator->set_alpha(1.0);

        _heat_button->set_label_text("HEAT OFF");
        _heat_button->set_filled(true);
    } else {
        _target_temp_indicator->set_alpha(0.5);

        _heat_button->set_label_text("HEAT ON");
        _heat_button->set_filled(false);
    }
}
