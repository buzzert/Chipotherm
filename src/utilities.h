/*
 * Created on Mon Jan 28 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#pragma once

extern "C" {
#include <stdlib.h>
}

namespace Utilities {
    double time_now()
    {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC_RAW, &ts);

        long ms = (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);                                                           
        return (ms / 1000.0);
    }
}

