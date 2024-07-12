#pragma once

#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_log.h"
#include "bn_camera_ptr.h"
#include "bn_display.h"
#include "bn_math.h"
#include "bn_regular_bg_map_item.h"
#include "bn_regular_bg_map_cell_info.h"
#include "bn_sprite_text_generator.h"
#include "bn_format.h"
#include "bn_rect.h"

#include "../../common/include/common_info.h"
#include "../../common/include/common_variable_8x16_sprite_font.h"


/**
 * Math
 */
bn::fixed lerp(bn::fixed a, bn::fixed b, bn::fixed t)
{
    return a + t * (b - a);
}

bn::fixed_point lerp(bn::fixed_point a, bn::fixed_point b, bn::fixed t)
{
    return bn::fixed_point(
        lerp(a.x(), b.x(), t),
        lerp(a.y(), b.y(), t)
    );
}

bn::color lerp(bn::color a, bn::color b, bn::fixed t)
{
    return bn::color(
        lerp(a.red(), b.red(), t).floor_integer(),
        lerp(a.green(), b.green(), t).floor_integer(),
        lerp(a.blue(), b.blue(), t).floor_integer()
    );
}

bn::fixed map(bn::fixed value, bn::fixed istart, bn::fixed istop, bn::fixed ostart, bn::fixed ostop)
{
    return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}


int mod(int a, int b) 
{
    return ((a % b) + b) % b;
}


bn::fixed distance(bn::fixed_point a, bn::fixed_point b) 
{
    return abs(a.x() - b.x()) + abs(a.y() - b.y());
}

bn::fixed clamp(bn::fixed v, bn::fixed min, bn::fixed max)
{
    return bn::max(bn::min(v, max), min);
}


bn::rect sprite_rect(bn::sprite_ptr spr)
{
    return bn::rect(spr.x().floor_integer(), spr.y().floor_integer(), spr.dimensions().width(), spr.dimensions().height());
}


bool sprites_touch(bn::sprite_ptr a, bn::sprite_ptr b)
{
    bn::rect a_rect = sprite_rect(a);
    bn::rect b_rect = sprite_rect(b);
    return a_rect.touches(b_rect);
}



/**
 * Colors
 */
bn::color rgb255(int r, int g, int b)
{
    return bn::color(r / 8, g / 8, b / 8);
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
 * Arrows navigation
 */
void move_with_dpad(bn::sprite_ptr spr, int speed)
{
    if (bn::keypad::left_held())
        spr.set_x(spr.x() - speed);
    if (bn::keypad::right_held())
        spr.set_x(spr.x() + speed);
    if (bn::keypad::up_held())
        spr.set_y(spr.y() - speed);
    if (bn::keypad::down_held())
        spr.set_y(spr.y() + speed);
}

void move_with_dpad(bn::sprite_ptr spr)
{
    move_with_dpad(spr, 1);
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
    return  tile_index;
}




class info_printer 
{
    bn::optional<bn::sprite_text_generator> text_generator;
    bn::vector<bn::sprite_ptr, 64> info_text_sprites;

    public: 

    info_printer(bn::sprite_font font) 
    {
        text_generator = bn::sprite_text_generator(font);
        text_generator->set_center_alignment();
    }

    info_printer() 
    {
        text_generator = bn::sprite_text_generator(common::variable_8x16_sprite_font);
        text_generator->set_center_alignment();
    }

    void print_map_tiles_at_position(bn::regular_bg_map_item map_item, bn::fixed_point position) 
    {
        int tile_left = get_map_tile_index_at_position(position + bn::fixed_point(-8,0), map_item);
        int tile_center = get_map_tile_index_at_position(position, map_item);
        int tile_right = get_map_tile_index_at_position(position + bn::fixed_point(8,0), map_item);
        print(bn::format<60>("{} | {} | {}", tile_left, tile_center, tile_right));
    }

    void print_map_tile_and_position(bn::regular_bg_map_item map_item, bn::fixed_point position) 
    {
        int tile_center = get_map_tile_index_at_position(position, map_item);
        print(bn::format<60>("({}, {}): {}", position.x(), position.y(), tile_center));
    }

    void print(bn::string<200> str)
    {
        info_text_sprites.clear();
        text_generator->generate(0, 65, str, info_text_sprites);
    }
};
