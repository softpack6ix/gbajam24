#ifndef UTILS_H
#define UTILS_H

#include "bn_fixed_point.h"
#include "bn_log.h"

// Utils
bn::fixed lerp(bn::fixed a, bn::fixed b, bn::fixed t)
{
    return a + t * (b - a);
}


int mod(int a, int b) 
{
    return ((a % b) + b) % b;
}



// map utils 
// Map (0,0) is top-left but sprite (0,0) is center-center
const bn::point get_map_point_at_position(bn::fixed_point pos) 
{
    int map_x = mod(((pos.x() + bn::display::width() / 2) / 8).floor_integer(), 32);
    int map_y = mod(((pos.y() + bn::display::height() / 2 + 30) / 8).floor_integer(), 32);
    return bn::point(map_x, map_y);
}

int get_map_tile_index_at_position(bn::fixed_point pos, bn::regular_bg_map_item map_item) 
{
    const bn::point p = get_map_point_at_position(pos);
    bn::regular_bg_map_cell map_cell = map_item.cell(p);
    int tile_index = bn::regular_bg_map_cell_info(map_cell).tile_index();

    BN_LOG(bn::format<60>("map tile: ({}, {}): {}", p.x(), p.y(), tile_index));
    BN_LOG(bn::format<60>("player pos: {}, {}", pos.x(), pos.y()));

    return  tile_index;
}


#endif