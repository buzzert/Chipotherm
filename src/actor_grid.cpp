/*
 * Created on Sun Dec 30 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#include "actor_grid.h"

ActorGrid::ActorGrid(Rect r, unsigned cols)
    : Actor(r), _cols(cols)
{

}

ActorGrid::~ActorGrid()
{

}

void ActorGrid::each_grid_item(const GridItemIter &iter)
{
    for (unsigned col = 0; col < _cols; col++) {
        std::vector<GridItem> col_items = _grid[col];
        for (GridItem &item : col_items) {
            iter(col, item);
        }
    }
}

void ActorGrid::each_actor(const ActorIter &iter)
{
    each_grid_item([&] (unsigned col, GridItem item) {
        iter(item.actor);
    });
}

void ActorGrid::update(SDL_Renderer *renderer)
{
    layout_if_needed();
    each_actor([&] (ActorPtr actor) {
        actor->update(renderer);
    });
}

void ActorGrid::render(SDL_Renderer *renderer)
{
    each_actor([&] (ActorPtr actor) {
        actor->render(renderer);
    });
}

void ActorGrid::stack_actor(ActorPtr actor, unsigned atColumn, float height)
{
    GridItem item {
        .actor = actor,
        .requested_height = height
    };

    _grid[atColumn].push_back(item);
    _needs_layout = true;
}

void ActorGrid::layout_if_needed()
{
    if (!_needs_layout) return;

    // Currently columns are evenly spaced
    float column_width = rect.width / _cols;

    // Foreach col
    float x_offset = 0.0;
    for (std::vector<GridItem> col : _grid) {
        float flexible_space_remaining = rect.height;

        // Compute ahead of time: subtract grid items with defined height
        unsigned num_flexible_items = 0;
        for (GridItem &item : col) {
            if (item.requested_height > 0) {
                flexible_space_remaining -= item.requested_height;
            } else {
                num_flexible_items++;
            }
        }

        float y_offset = 0.0;
        float flexible_item_height = (flexible_space_remaining / num_flexible_items);
        for (GridItem &item : col) {
            Rect r;
            r.x = x_offset;
            r.y = y_offset;
            r.width = column_width;
            if (item.requested_height > 0) {
                r.height = item.requested_height;
            } else {
                r.height = flexible_item_height;
            }

            item.actor->rect = rect;
            y_offset += r.height;
        }

        x_offset += column_width;
    }
}
