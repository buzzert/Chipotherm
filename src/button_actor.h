/*
 * Created on Sun Jan 27 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#pragma once

#include <bubbles/bubbles.h>

BUBBLES_NAMESPACE_BEGIN

class ButtonActor : public Actor
{
public:
    ButtonActor(Rect r);

    void set_label_text(const std::string &str);
    void set_foreground_color(Color c) { _foreground_color = c; };
    void set_highlighted(bool highlighted) { _highlighted = highlighted; };
    void render(cairo_t *cr, Rect at_rect) override;

private:
    ActorGrid  _grid;
    bool       _highlighted;
    LabelActor _label;
    Color      _foreground_color;
};

BUBBLES_NAMESPACE_END

