/*
 * Created on Tue Jan 29 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#pragma once

#include <functional>
#include <vector>

using RunloopTask = std::function<void()>;

class Runloop
{
public:
    static Runloop& main_runloop()
    {
        static Runloop instance;
        return instance;
    }

    void iterate();    
    void schedule_task(const RunloopTask &task);

private:
    Runloop() {};

    Runloop(Runloop const&)        = delete;
    void operator=(Runloop const&) = delete;

    std::vector<RunloopTask> _pending_tasks;
};
