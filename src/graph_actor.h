/*
 * Created on Fri Jan 25 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#pragma once

#include <bubbles/bubbles.h>

BUBBLES_NAMESPACE_BEGIN

class GraphActor : public Actor
{
public:
    GraphActor(Rect r) : Actor(r) {};
    void render(cairo_t *cr, Rect at_rect) override;

    void add_sample(double value);

private:
    struct Sample {
        double value;
        // timestaemp?
    };

    std::vector<Sample> _samples;
};

BUBBLES_NAMESPACE_END

