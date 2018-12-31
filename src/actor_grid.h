/*
 * Created on Sun Dec 30 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#pragma once

#include "actor.h"

#include <functional>
#include <memory>
#include <vector>

class ActorGrid : public Actor
{
public:
    ActorGrid(Rect r, unsigned cols);
    ~ActorGrid() override;

    void render(SDL_Renderer *renderer) override;
    void update(SDL_Renderer *renderer) override;

    /// Use height = -1 for "automatic" height
    void stack_actor(ActorPtr actor, unsigned atColumn, float height);

private:
    struct GridItem {
        ActorPtr actor;
        float    requested_height;
    };

    std::vector<std::vector<GridItem>> _grid;
    bool     _needs_layout = true;
    unsigned _cols;

    using ActorIter = std::function<void(ActorPtr)>;
    void each_actor(const ActorIter &iter);

    using GridItemIter = std::function<void(unsigned, GridItem)>;
    void each_grid_item(const GridItemIter &iter);

    void layout_if_needed();
};
