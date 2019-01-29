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

    void set_foreground_color(Color c);
    std::shared_ptr<LabelActor> get_label() const { return _label; };

    bool get_filled() const { return _filled; };
    void set_filled(bool filled);
    
private:
    std::shared_ptr<LabelActor> _label;
    std::shared_ptr<LabelActor> _title_label;

    bool  _filled;
    Color _foreground_color;
};
