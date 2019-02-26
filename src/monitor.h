/*
 * Created on Tue Jan 29 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#pragma once

#include "io_control.h"

#include <boost/signals2.hpp>
#include <condition_variable>
#include <mutex>
#include <thread>

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

    // Simulation mode provides a fake temperature and the current temperature pretends
    // to fluctuate while the heater is on. 
    void set_simulation_mode(bool simulate);

    void set_monitoring_enabled(bool enabled);
    bool get_monitoring_enabled() const { return _enabled; };

    void set_target_temperature(float target);
    float get_target_temperature() const { return _target_temperature; };

    State get_current_state() const { return _state; };
    IOControl& get_controller() { return _controller; };

    // Set this to `true` to automatically enable/disable DPMS (display sleep)
    // If this is enabled, only while monitoring is enabled will the monitor remain awake,
    // otherwise the typical timeout is enforced via X11. 
    bool controls_screen = false;

    // Attach to this signal to be notified when the state changes (see State enum above)
    boost::signals2::signal<void(State)> state_changed;

private:
    bool      _simulation_mode = false;
    bool      _enabled = false;
    float     _target_temperature = 73.0;
    unsigned  _num_cycles = 0;

    State     _state = State::DISABLED;
    IOControl _controller;

    std::thread _monitor_thread;
    std::mutex  _monitor_mutex;
    std::condition_variable _monitor_condition;

    using Clock = std::chrono::system_clock;
    std::chrono::time_point<Clock> _last_engagement_time;

    void monitor_main();
    void transition_to_state(State newstate);
    void update_simulation_state();
    void set_heater_engaged_if_time_allows(bool engaged, float current_temperature);
};
