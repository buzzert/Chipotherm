/*
 * Created on Tue Jan 29 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#include "monitor.h"

#include <iostream>
#include <algorithm>
#include <chrono>
#include <cmath>

#include "runloop.h"

// How often to allow the heater to toggle between engaged/disengaged
static const std::chrono::minutes k_toggle_hysteresis(3);

Monitor::Monitor()
{}

void Monitor::monitor_main()
{
    using namespace std::chrono_literals;

    bool running = true;
    while (running) {
        if (_simulation_mode) {
            update_simulation_state();
        }

        float current_temperature = _controller.read_temperature();
        bool heater_engaged = (std::round(current_temperature) < _target_temperature);
        set_heater_engaged_if_time_allows(heater_engaged);

        std::unique_lock<std::mutex> lk(_monitor_mutex);
        _monitor_condition.wait_for(lk, 2s);
        running = _enabled;
    }
}

void Monitor::set_simulation_mode(bool simulate)
{
    _simulation_mode = simulate;
    _controller.simulate = simulate;
}

void Monitor::update_simulation_state()
{
    if (_state == State::HEATING) {
        _controller.simulated_temperature++;
    } else if (_state == State::IDLE) {
        _controller.simulated_temperature--;
    }
}

void Monitor::set_monitoring_enabled(bool enabled)
{
    {
        std::lock_guard<std::mutex> lk(_monitor_mutex);
        _enabled = enabled;
    }
    
    if (enabled) {
        // Turn off display blanking 
        if (controls_screen) {
            int res = system("xset s off -dpms");
            if (res != 0) {
                printf("WARN: Could not disable screen blanking (retcode: %d)\n", res);
            }
        }

        transition_to_state(State::IDLE);    
        _monitor_thread = std::thread(&Monitor::monitor_main, this);
    } else {
        // Re-enable display blanking
        if (controls_screen) {
            system("xset s off +dpms");
        }

        // Turn off heater
        _controller.set_heater_on(false);

        transition_to_state(State::DISABLED);
        _monitor_condition.notify_all();
        _monitor_thread.join();
    }
}

void Monitor::set_heater_engaged_if_time_allows(bool engaged)
{
    bool heater_on = _controller.get_heater_on();
    if (engaged == heater_on) return;

    // Toggle heater engagement based on some defined hysteresis
    auto hysteresis = std::chrono::duration_cast<std::chrono::seconds>(k_toggle_hysteresis);
    if (_simulation_mode) {
        hysteresis = std::chrono::seconds(5);
    }

    if (Clock::now() - _last_engagement_time > hysteresis) {
        printf("Setting heater to: %s\n", engaged ? "ON" : "OFF");

        _controller.set_heater_on(engaged);
        _last_engagement_time = Clock::now();

        if (engaged) {
            transition_to_state(State::HEATING);
        } else {
            transition_to_state(State::IDLE);
        }
    }
}

void Monitor::set_target_temperature(float target)
{
    _target_temperature = std::min(100.f, std::max(0.f, target));
}

void Monitor::transition_to_state(State newstate)
{
    _state = newstate;
    Runloop::main_runloop().schedule_task([=]() {
        state_changed(newstate);
    });
}
