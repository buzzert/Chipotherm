/*
 * Created on Tue Jan 29 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#pragma once

#include "io_control.h"

#include <condition_variable>
#include <mutex>
#include <thread>
#include <sigc++/sigc++.h>

class Monitor
{
public:
    enum State {
        // Monitoring system and heat turned off
        DISABLED,

        // Heater engaged
        HEATING,

        // Monitoring, but heater disengaged
        IDLE
    };

    Monitor();

    void set_monitoring_enabled(bool enabled);
    bool get_monitoring_enabled() const { return _enabled; };

    void set_target_temperature(float target);
    float get_target_temperature() const { return _target_temperature; };

    State get_current_state() const { return _state; };
    IOControl& get_controller() { return _controller; };

    sigc::signal<void, State> state_changed;

private:
    bool      _enabled = false;
    float     _target_temperature = 73.0;

    State     _state = State::DISABLED;
    IOControl _controller;

    std::thread _monitor_thread;
    std::mutex  _monitor_mutex;
    std::condition_variable _monitor_condition;

    void monitor_main();
    void transition_to_state(State newstate);
};
