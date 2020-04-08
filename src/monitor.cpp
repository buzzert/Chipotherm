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
static const float k_temperature_hysteresis = 2; // in degrees

// Minimum amount of time between cycles
static const std::chrono::seconds k_toggle_hysteresis(30);

// How long the heater runs the cooldown cycle for, after turning off (for gas heaters)
static const std::chrono::seconds k_cooldown_time(60);

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
        set_heater_engaged_if_time_allows(heater_engaged, current_temperature);

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
    if (enabled != _enabled) {
        std::lock_guard<std::mutex> lk(_monitor_mutex);
        _enabled = enabled;
    } else {
        // NOTE: early return
        return;
    }
    
    if (enabled) {
        // Turn off display blanking 
        if (controls_screen) {
            int res = system("xset s off -dpms");
            if (res != 0) {
                printf("WARN: Could not disable screen blanking (retcode: %d)\n", res);
            }
        }

        // Reset throttle
        _num_cycles = 0;
        _last_engagement_time = Clock::now() - std::chrono::hours(1);

        transition_to_state(State::IDLE);    
        _monitor_thread = std::thread(&Monitor::monitor_main, this);
    } else {
        // Re-enable display blanking
        if (controls_screen) {
            system("xset s off dpms 3600 3600 3600");
        }

        // Turn off heater
        _controller.set_heater_on(false);

        transition_to_state(State::COOLDOWN); // COOLDOWN -> DISABLED
        _monitor_condition.notify_all();
        _monitor_thread.join();
    }
}

void Monitor::set_heater_engaged_if_time_allows(bool engaged, float current_temperature)
{
    bool heater_on = _controller.get_heater_on();
    if (engaged == heater_on) return;

    // Toggle heater engagement based on some defined hysteresis
    auto hysteresis = std::chrono::duration_cast<std::chrono::seconds>(k_toggle_hysteresis);
    if (_simulation_mode) {
        hysteresis = std::chrono::seconds(5);
    }

    bool time_allows = (Clock::now() - _last_engagement_time) > hysteresis;
    bool hysteresis_allows = fabsf(_target_temperature - current_temperature) > k_temperature_hysteresis;

    if (time_allows && (hysteresis_allows || _num_cycles == 0)) {
        printf("Setting heater to: %s\n", engaged ? "ON" : "OFF");

        _controller.set_heater_on(engaged);
        _last_engagement_time = Clock::now();

        if (engaged) {
            transition_to_state(State::HEATING);
        } else {
            transition_to_state(State::IDLE);
        }

        _num_cycles++;
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

    _cooldown_condition.notify_all();
    if (newstate == State::COOLDOWN) {
        std::thread timer([=] {
            // If going from heating -> cooldown, set a cooldown timer to automatically transition
            // back to the Idle state.
            std::unique_lock<std::mutex> lk(_cooldown_mutex);
            _cooldown_condition.wait_for(lk, k_cooldown_time);
            if (_state == State::COOLDOWN) {
                transition_to_state(State::DISABLED);
            }
        });

        timer.detach();
    }
}
