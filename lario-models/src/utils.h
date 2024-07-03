#ifndef UTILS_H
#define UTILS_H

#include "bn_fixed_point.h"
#include "bn_log.h"


/**
 * Math
 */
bn::fixed lerp(bn::fixed a, bn::fixed b, bn::fixed t)
{
    return a + t * (b - a);
}

int mod(int a, int b) 
{
    return ((a % b) + b) % b;
}


bn::fixed distance(bn::fixed_point a, bn::fixed_point b) 
{
    return abs(a.x() - b.x()) + abs(a.y() - b.y());
}



/**
 * Camera
 */
bool camera_moving_to_point = false;
int cam_follow_margin = 3;

void camera_follow_smooth(bn::camera_ptr cam, bn::fixed_point position) 
{
    if (bn::abs(cam.x() - position.x()) > bn::display::width() / cam_follow_margin || 
        bn::abs(cam.y() - position.y()) > bn::display::height() / cam_follow_margin) {
        camera_moving_to_point = true;
    }

    if (camera_moving_to_point) {
        cam.set_x(lerp(position.x(), cam.x(), 0.95));
        cam.set_y(lerp(position.y(), cam.y(), 0.95));

        if (bn::abs(cam.x() - position.x()) < 1  &&  bn::abs(cam.y() - position.y()) < 1) {
            camera_moving_to_point = false;
        }
    }
}


/**
 * Map utils
 * Map (0,0) is top-left but sprite (0,0) is center-center
 */
const bn::point get_map_point_at_position(bn::fixed_point pos, bn::regular_bg_map_item map_item) 
{
    bn::size dimensions = map_item.dimensions();
    int map_x = mod(((pos.x() + bn::display::width() / 2 + 16) / 8).floor_integer(), dimensions.width());
    int map_y = mod(((pos.y() + bn::display::height() / 2 + 30 + 96) / 8).floor_integer(), dimensions.height());
    return bn::point(map_x, map_y);
}


int get_map_tile_index_at_position(bn::fixed_point pos, bn::regular_bg_map_item map_item) 
{
    const bn::point p = get_map_point_at_position(pos, map_item);
    bn::regular_bg_map_cell map_cell = map_item.cell(p);
    int tile_index = bn::regular_bg_map_cell_info(map_cell).tile_index();

    // BN_LOG(bn::format<60>("map tile: ({}, {}): {}", p.x(), p.y(), tile_index));
    // BN_LOG(bn::format<60>("player pos: {}, {}", pos.x(), pos.y()));

    return  tile_index;
}


#endif