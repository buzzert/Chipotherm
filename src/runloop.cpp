/*
 * Created on Tue Jan 29 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#include "runloop.h"

void Runloop::iterate()
{
    for (RunloopTask &task : _pending_tasks) {
        task();
    }

    _pending_tasks.clear();
}

void Runloop::schedule_task(const RunloopTask &task)
{
    _pending_tasks.push_back(task);
}
