/*
 * Created on Tue Jan 29 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#include "monitor.h"

#include <iostream>
#include <algorithm>
#include <chrono>

#include "runloop.h"

Monitor::Monitor()
{}

void Monitor::monitor_main()
{
    using namespace std::chrono_literals;

    bool running = true;
    while (running) {
        // TODO: Monitor

        std::unique_lock<std::mutex> lk(_monitor_mutex);
        _monitor_condition.wait_for(lk, 4s);
        running = _enabled;
    }

    std::cout << "Exiting" << std::endl;
}

void Monitor::set_monitoring_enabled(bool enabled)
{
    {
        std::lock_guard<std::mutex> lk(_monitor_mutex);
        _enabled = enabled;
    }
    
    if (enabled) {
        transition_to_state(State::IDLE);    
        _monitor_thread = std::thread(&Monitor::monitor_main, this);
    } else {
        transition_to_state(State::DISABLED);
        _monitor_condition.notify_all();
        _monitor_thread.join();
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
