/*
 * Created on Fri Jan 25 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#pragma once

#include <bubbles/bubbles.h>

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
        // timestaemp?
    };

    std::shared_ptr<LabelActor> _label;
    std::vector<Sample> _samples;
};

