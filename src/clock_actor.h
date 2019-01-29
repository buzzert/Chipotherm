/*
 * Created on Sun Jan 06 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#pragma once

#include <chrono>

#include <bubbles/actor/label_actor.h>

using namespace Bubbles;

DEF_SHARED_PTR(ClockActor)

class ClockActor : public Actor
{
public:
    ClockActor(Rect r);

    void update() override;
    void render(cairo_t *cr, Rect at_rect) override;
    void layout_actors() override;
private:
    std::shared_ptr<LabelActor> _label;

    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    TimePoint last_update;
};
