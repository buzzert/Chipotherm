/*
 * Created on Tue Jan 29 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#pragma once

#include <memory>
#include <libudev.h>
#include <stdbool.h>
#include <tempered.h>

class IOControl 
{
public:
    // Returns the current temperature in degrees fahrenheit
    float read_temperature();

    void set_heater_on(bool on);
    bool get_heater_on();

    bool get_display_on();

    // Simulation
    bool simulate = false;
    float simulated_temperature = 69.2;

private:
    bool _sim_heat_on = false;

    std::string                      _relay_switch_path;
    std::shared_ptr<tempered_device> _monitor_device;
    std::shared_ptr<udev_device>     _display_device;
    void initialize_devices_if_necessary();
};
