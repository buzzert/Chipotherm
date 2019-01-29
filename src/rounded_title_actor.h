/*
 * Created on Mon Jan 28 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#pragma once

#include <bubbles/bubbles.h>
#include <string>

using namespace Bubbles;

class RoundedTitleActor : public Actor 
{
public:
    RoundedTitleActor(Rect r, std::string title);

    void render(cairo_t *cr, Rect in_rect) override;
    void layout_actors() override;

    void set_foreground_color(Color c) { _foreground_color = c; };
    std::shared_ptr<LabelActor> get_label() const { return _label; };
    
private:
    std::shared_ptr<LabelActor> _label;
    std::shared_ptr<LabelActor> _title_label;

    Color _foreground_color;
};