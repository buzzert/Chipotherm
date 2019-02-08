/*
 * Created on Sun Jan 27 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#pragma once

#include <boost/signals2.hpp>
#include <bubbles/bubbles.h>
#include <functional>

using namespace Bubbles;

DEF_SHARED_PTR(ButtonActor)

class ButtonActor : public Actor
{
public:
    ButtonActor(Rect r);

    void set_label_text(const std::string &str);
    void set_foreground_color(Color c) { _foreground_color = c; };
    void set_highlighted(bool highlighted);
    void set_filled(bool filled);

    void render(cairo_t *cr, Rect at_rect) override;
    void layout_actors() override;

    void mouse_down(int x, int y) override;
    void mouse_up(int x, int y) override;

    boost::signals2::signal<void()> clicked;

private:
    LabelActorPtr _label;

    bool       _filled = false;
    bool       _highlighted = false;
    Color      _foreground_color;
};

