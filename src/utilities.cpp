/*
 * Created on Tue Jan 29 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#include "utilities.h"

namespace Utilities {
    double time_now()
    {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME_COARSE, &ts);

        long ms = (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);                                                           
        return (ms / 1000.0);
    }
    
    template<typename T>
    static std::string string_val(T val, const char *format)
    {
        std::string temp_string = "      ";
        snprintf((char *)temp_string.data(), temp_string.max_size(), format, val);

        return temp_string;
    }

    template<>
    std::string string_val<double>(double dbl)
    {
        return string_val(dbl, "%.0f");
    }

    template<>
    std::string string_val<float>(float dbl)
    {
        return string_val(dbl, "%.0f");
    }

    template<>
    std::string string_val<int>(int num)
    {
        return string_val(num, "%d");
    }
}
