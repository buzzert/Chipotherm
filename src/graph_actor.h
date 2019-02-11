/*
 * Created on Fri Jan 25 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#pragma once

#include <bubbles/bubbles.h>
#include <deque>

using namespace Bubbles;

DEF_SHARED_PTR(GraphActor);

class GraphActor : public Actor
{
public:
    GraphActor(Rect r);
    void render(cairo_t *cr, Rect at_rect) override;

    void add_sample(double value);

    void layout_actors() override;

private:
    struct Sample {
        double value;
        // timestamp?
    };

    std::shared_ptr<LabelActor> _label;
    std::deque<Sample>          _samples;

    unsigned max_samples(double for_width);
};


