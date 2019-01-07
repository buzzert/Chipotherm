/*
 * Created on Sun Jan 06 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#pragma once

#include <chrono>

#include <bubbles/actor/cairo_actor.h>
#include <bubbles/actor/label_actor.h>

BUBBLES_NAMESPACE_BEGIN

class ClockActor : public CairoActor
{
public:
    ClockActor(Rect r);

    void update(SDL_Renderer *renderer) override;
    void layout_actors() override;
    void display_surface() override;
private:
    std::shared_ptr<LabelActor> _label;

    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    TimePoint last_update;
};

BUBBLES_NAMESPACE_END
