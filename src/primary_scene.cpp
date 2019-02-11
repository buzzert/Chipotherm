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

    _online_label = std::make_shared<LabelActor>(RECT_ZERO, "Connecting... ");
    _online_label->set_alignment(PangoAlignment::PANGO_ALIGN_RIGHT);
    _online_label->set_font_prop("Karla 10");
    _online_label->set_foreground_color(Color(0xFF, 0xFF, 0xFF, 0x88));
    status_grid->stack_actor(_online_label, 0);

    return status_grid;
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

        _qube = std::make_shared<QubeActor>(Rect());
        temp_stack->stack_actor(_qube, 0);

        RoundedTitleActorPtr current = std::make_shared<RoundedTitleActor>(RECT_ZERO, "CURRENT");
        current->set_label_contents("00");
        current->set_foreground_color(Palette::foreground);
        current->set_filled(true);
        temp_stack->stack_actor(current, 0);
        _current_temp_indicator = current;

        RoundedTitleActorPtr target = std::make_shared<RoundedTitleActor>(RECT_ZERO, "TARGET");
        target->pulsing = true;
        target->set_label_contents("00");
        target->set_filled(true);
        target->set_foreground_color(Color(0xFF, 0x00, 0x00, 0xFF));
        temp_stack->stack_actor(target, 0);
        _target_temp_indicator = target;
    }

    // Graph
    _graph = std::make_shared<GraphActor>(Rect());
    grid->stack_actor(_graph, 0);

    return grid;
}

ActorGridPtr PrimaryScene::initialize_controls_grid()
{
    auto grid = std::make_shared<ActorGrid>(RECT_ZERO, 1);

    // Heat ON button 
    {
        _heat_button = std::make_shared<ButtonActor>(RECT_ZERO);
        _heat_button->set_foreground_color(Color(0xFF, 0x00, 0x00, 0xFF));
        _heat_button->clicked.connect([this]() {
            bool enabled = _monitor.get_monitoring_enabled();
            _monitor.set_monitoring_enabled(!enabled);
        });
        grid->stack_actor(_heat_button, 0, 160);
    }

    // Temperature adjustment
    {
        auto subgrid = std::make_shared<ActorGrid>(RECT_ZERO, 1);
        subgrid->set_orientation(ActorGrid::Orientation::HORIZONTAL);

        auto minus_button = std::make_shared<ButtonActor>(RECT_ZERO);
        minus_button->set_label_text("-");
        minus_button->set_foreground_color(Palette::foreground);
        minus_button->clicked.connect([this]() {
            _monitor.set_target_temperature(_monitor.get_target_temperature() - 1);
            _target_temp_indicator->flash();
            update_ui_state();
        });
        subgrid->stack_actor(minus_button, 0);

        auto plus_button = std::make_shared<ButtonActor>(RECT_ZERO);
        plus_button->set_foreground_color(Palette::foreground);
        plus_button->set_label_text("+");
        plus_button->clicked.connect([this]() {
            _monitor.set_target_temperature(_monitor.get_target_temperature() + 1);
            _target_temp_indicator->flash();
            update_ui_state();
        });
        subgrid->stack_actor(plus_button, 0);

        grid->stack_actor(subgrid, 0);
    }

    return grid;
}

PrimaryScene::PrimaryScene(Rect canvas_rect, bool windowed, double scale, std::string server_url)
    : MainScene(canvas_rect, windowed, scale)
{
    // Naievely assume if we're running in windowed mode, we want to simulate
    _monitor.set_simulation_mode(windowed);
    _monitor.controls_screen = !windowed;
    _monitor.state_changed.connect([this](Monitor::State newstate) {
        update_ui_state();
    });

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

    // Connect signals for remote control
    _remote.set_enabled.connect([this](bool enabled) {
        _monitor.set_monitoring_enabled(enabled);
    });

    _remote.set_temperature.connect([this](float temp) {
        _monitor.set_target_temperature(temp);
    });

    _remote.refresh_state.connect([this](Remote::State &state) {
        bool enabled = _monitor.get_monitoring_enabled();
        float target = _monitor.get_target_temperature();
        float current = _monitor.get_controller().read_temperature();
        bool heat_on = _monitor.get_controller().get_heater_on();

        state.enabled = enabled;
        state.heat_on = heat_on;
        state.current_temp = current;
        state.target_temp = target;
    });

    _remote.start_listening(server_url);

    update_ui_state();
}

void PrimaryScene::update()
{
    MainScene::update();

    Runloop::main_runloop().iterate();

    using namespace std::chrono_literals;
    if (Clock::now() - _last_sample_time > 1s) {
        update_ui_state();
    }

    if (Clock::now() - _last_graph_update_time > 5s) {
        float current_temp = _monitor.get_controller().read_temperature();
        _graph->add_sample(current_temp);
        _last_graph_update_time = Clock::now();
    }
}

void PrimaryScene::update_ui_state()
{
    _last_sample_time = Clock::now();
    
    bool enabled = _monitor.get_monitoring_enabled();
    bool heat_on = _monitor.get_controller().get_heater_on();
    
    // Update current temperature
    float current_temp = _monitor.get_controller().read_temperature();
    _current_temp_indicator->set_label_contents(Utilities::string_val(current_temp));

    // Update target temperature
    float target_temp = _monitor.get_target_temperature();
    _target_temp_indicator->set_label_contents(Utilities::string_val(target_temp));

    // Target temperature rect pulses if heat is on
    _target_temp_indicator->pulsing = heat_on;
    
    // Qube rotates faster if heat is on
    if (heat_on) {
        _qube->set_rotation_speed(5.0);
    } else {
        _qube->set_rotation_speed(1.0);
    }

    // Online status
    bool online = _remote.get_online_status();
    if (online) {
        _online_label->set_contents("ONLINE ");
        _online_label->set_foreground_color(Colors::green);
    } else {
        _online_label->set_contents("OFFLINE ");
        _online_label->set_foreground_color(Colors::red);
    }

    // Monitoring status
    if (enabled) {
        // Qube is red if monitoring is enabled
        _qube->set_color(Color(0xFF, 0x00, 0x00, 0xFF));

        // Target temperature is opaque
        _target_temp_indicator->set_alpha(1.0);

        // Heat button filled
        _heat_button->set_label_text("DISABLE");
        _heat_button->set_filled(true);
    } else {
        // If monitoring is disabled, qube is foreground color
        _qube->set_color(Palette::foreground);

        // Target temperature is transparend (dark)
        _target_temp_indicator->set_alpha(0.5);

        // Heat button is an outline
        _heat_button->set_label_text("ENABLE");
        _heat_button->set_filled(false);
    }
}
