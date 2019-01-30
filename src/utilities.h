/*
 * Created on Mon Jan 28 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#pragma once

#include <string>

extern "C" {
#include <stdlib.h>
}

namespace Utilities {
    // Returns the current time (in seconds) from the monotonic clock 
    double time_now();

    // Returns string representation of `val`
    template<typename T> std::string string_val(T val);
}

