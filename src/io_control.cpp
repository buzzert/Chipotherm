/*
 * Created on Tue Jan 29 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#include "io_control.h"

float IOControl::read_temperature()
{
    if (simulate) {
        return simulated_temperature;
    } else {
        // TODO
        return 0.0;
    }
}

void IOControl::set_heater_on(bool on)
{
    if (simulate) {
        _sim_heat_on = on;
    } else {
        // TODO
    }
}

bool IOControl::get_heater_on() const
{
    if (simulate) {
        return _sim_heat_on;
    } else {
        // TODO
        return false;
    }
}
