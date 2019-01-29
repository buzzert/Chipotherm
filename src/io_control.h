/*
 * Created on Tue Jan 29 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#pragma once

#include <stdbool.h>

class IOControl 
{
public:
    float read_temperature();

    void set_heater_on(bool on);
    bool get_heater_on() const;

    // Simulation
    bool simulate = false;
    float simulated_temperature = 69.2;

private:
    bool _sim_heat_on = false;
};
