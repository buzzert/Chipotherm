/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#include "actor.h"

Actor::Actor(Rect rect)
    : rect(rect), alpha(1.0), needs_display(true)
{}

void Actor::set_rect(Rect r)
{
    rect = r;
    needs_display = true;
}
