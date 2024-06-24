#include "bn_core.h"

#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"
#include "bn_regular_bg_actions.h"
#include "bn_regular_bg_builder.h"
#include "bn_regular_bg_attributes.h"
#include "bn_regular_bg_map_cell_info.h"
#include "bn_sprite_text_generator.h"
#include "bn_regular_bg_animate_actions.h"
#include "bn_regular_bg_position_hbe_ptr.h"
#include "bn_regular_bg_attributes_hbe_ptr.h"
#include "bn_point.h"
#include "bn_log.h"
#include "bn_format.h"
#include "bn_math.h"
#include "bn_display.h"

// BGs
#include "bn_regular_bg_items_tilemap.h"


// Sprites
#include "bn_sprite_items_charge.h"
#include "bn_sprite_items_bolt.h"
#include "bn_sprite_items_wormhole.h"
#include "bn_sprite_items_jochem.h"
#include "bn_sprite_items_punch.h"
#include "bn_sprite_items_dog.h"


// Font
#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"



bn::fixed lerp(bn::fixed a, bn::fixed b, bn::fixed t)
{
    return a + t * (b - a);
}

    
int main()
{
    bn::core::init();

    // constexpr bn::string_view info_text_lines[] = {
    //     "PAD: move dog"
    // };

    // bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    // common::info info("Map collision", info_text_lines, text_generator);



    bn::bg_palettes::set_transparent_color(bn::color(16, 24, 29));


    // BG
    bn::regular_bg_ptr tilemap = bn::regular_bg_items::tilemap.create_bg(8, 48);


    // Map
    const bn::regular_bg_map_item& map_item = bn::regular_bg_items::tilemap.map_item();
    bn::regular_bg_map_cell valid_map_cell = map_item.cell(0, 0);
    int valid_tile_index = bn::regular_bg_map_cell_info(valid_map_cell).tile_index();


    
    bn::camera_ptr camera = bn::camera_ptr::create(0, 0);

   

    // Jochem
    bn::sprite_ptr jochem_sprite = bn::sprite_items::jochem.create_sprite(0, 0);
    bn::sprite_ptr punch_sprite = bn::sprite_items::punch.create_sprite(20, 50);

    bn::sprite_animate_action<14> idle = bn::create_sprite_animate_action_forever(jochem_sprite, 1, bn::sprite_items::jochem.tiles_item(), 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13
    );    

    bn::sprite_animate_action<21> walking = bn::create_sprite_animate_action_forever(jochem_sprite, 1, bn::sprite_items::jochem.tiles_item(), 
        14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34
    );

    bn::sprite_animate_action<25> kicking = bn::create_sprite_animate_action_once(jochem_sprite, 1, bn::sprite_items::jochem.tiles_item(), 
        35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
        48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59
    );


    // Punch
    bn::sprite_animate_action<16> punch_action = bn::create_sprite_animate_action_once(punch_sprite, 1, bn::sprite_items::punch.tiles_item(), 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
    );


    // Charge
    int charge_x = -90;
    int bolt_start = charge_x - 25;
    bn::sprite_ptr charge_sprite = bn::sprite_items::charge.create_sprite(charge_x, 0);
    bn::sprite_ptr bolt_sprite = bn::sprite_items::bolt.create_sprite(bolt_start, 0);
    

    bn::sprite_animate_action<43> charge_action = bn::create_sprite_animate_action_once(charge_sprite, 1, bn::sprite_items::charge.tiles_item(), 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 
        33, 34, 35, 36, 37, 38, 39, 40, 41, 42
    );

    bn::sprite_animate_action<5> charge_sustain_action = bn::create_sprite_animate_action_forever(charge_sprite, 1, bn::sprite_items::charge.tiles_item(), 
        43, 44, 45, 46, 47
    );

    bn::sprite_animate_action<7> charge_discharge_action = bn::create_sprite_animate_action_once(charge_sprite, 1, bn::sprite_items::charge.tiles_item(), 
        48, 49, 50, 51, 52, 53, 54
    );


    // Bolt
    bn::sprite_animate_action<72> bolt_action = bn::create_sprite_animate_action_once(bolt_sprite, 1, bn::sprite_items::bolt.tiles_item(), 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 
        33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 
        50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 
        67, 68, 69, 70, 71
    );

    bn::sprite_ptr dog_sprite = bn::sprite_items::dog.create_sprite(20, 50);



    charge_sprite.set_camera(camera);
    bolt_sprite.set_camera(camera);
    jochem_sprite.set_camera(camera);
    tilemap.set_camera(camera);
    punch_sprite.set_camera(camera);
    dog_sprite.set_camera(camera);


    const bn::fixed gravity = 0.1;

    bn::fixed bolt_x = -25;
    bn::fixed bolt_vx = 8;

    bn::fixed x = 0;
    bn::fixed y = 0;
    bn::fixed velocity_x = 0.0;
    bn::fixed velocity_y = 0.0;

    int height = 80;
    int width = 120;

    bool wasMoving = false;
    bool moving = false;
    bool isKicking = false;
    int kickTimer = 0;
    bool jumping = false;

    // Cam
    bool camera_moving_to_player = false;
    int cam_follow_margin = 3;

    bn::fixed_point point;


    while(true)
    {
        moving = false;

        // Spell casting
        if (bn::keypad::b_pressed()) {
            charge_action.reset();
            charge_sustain_action.reset();
            charge_discharge_action.reset();
        }

        if (bn::keypad::b_held()) {
            charge_sprite.set_position(x, y);
            if (charge_action.done()) {
                charge_sustain_action.update();
            } else {
                charge_action.update();
            }
        } else {
            if (!charge_discharge_action.done()) {
                charge_discharge_action.update();
            }
        }

        if (bn::keypad::b_released()) {
            if (charge_action.done()) {
                charge_action.reset();
                bolt_action.reset();
                bolt_sprite.set_visible(true);
                bolt_sprite.set_position(jochem_sprite.position());
                bolt_x = jochem_sprite.x();
                bolt_vx = jochem_sprite.horizontal_flip() ? -8 : 8;
            }
        }

        if (!bolt_action.done()) {
            // bolt_sprite.set_visible(false);
            bolt_action.update();
            bolt_x += bolt_vx;
            bolt_sprite.set_x(bolt_x);
        }




        // Regular character

      


        // jumping
        if (bn::keypad::up_pressed() && !jumping)
        {
            jumping = true;
            velocity_y = -5.5;
        }
        else
        {
            velocity_y += gravity;
        }

        if(bn::keypad::left_held()) {
            x -= 2;
            moving = true;
            jochem_sprite.set_horizontal_flip(true);
        }
        if(bn::keypad::right_held()) {
            x+=2;
            moving = true;
            jochem_sprite.set_horizontal_flip(false);
        }

        if(bn::keypad::a_pressed()) {
            isKicking = true;
            kickTimer = 0;
            kicking.reset();
        }

        if (!punch_action.done()) {
            punch_action.update();
        }

        int kick_offset = isKicking ? (jochem_sprite.horizontal_flip() ? -10 : 10) : 0;


        if (!moving && !isKicking) {    
            idle.update();
            // jochem_sprite.set_position(x, y);
        }

        if (moving) {
            if (isKicking) {
                // jochem_sprite.set_position(x, y);
            } else {
                walking.update();
                // jochem_sprite.set_position(x, y);
            }
        } 

        if (isKicking) {
            // jochem_sprite.set_position(x + kick_offset, y);

            if (kickTimer == 20) {
                punch_action.reset();
                int offset = jochem_sprite.horizontal_flip() ? -35 : 35;
                punch_sprite.set_position(x+offset, y-12);
            }
            if (!kicking.done()) {
                kicking.update();
            } else {
                isKicking = false;
            }


            kickTimer++;

        }



        // Dog
            // Dog control
        // if (bn::keypad::left_held()) {
        //     dog_sprite.set_x(dog_sprite.x() - 1);
        // } 
        // if (bn::keypad::right_held()) {
        //     dog_sprite.set_x(dog_sprite.x() + 1);

        // }
        // if (bn::keypad::up_held()) {
        //     dog_sprite.set_y(dog_sprite.y() - 1);
        // }
        // if (bn::keypad::down_held()) {
        //     dog_sprite.set_y(dog_sprite.y() + 1);
        // }
        


        // int map_x = bn::max(((dog_sprite.x() + 120) / 8).floor_integer(), 0);
        // int map_y = bn::max(((dog_sprite.y() + 80) / 8).floor_integer(), 0);
        

        // Map (0,0) is top-left but sprite (0,0) is center-center
        int map_x = ((x + bn::display::width() / 2) / 8).floor_integer();
        int map_y = ((y + bn::display::height() / 2 + 28) / 8).floor_integer();

        bn::regular_bg_map_cell player_map_cell = map_item.cell(bn::point(map_x, map_y));
        int player_tile_index = bn::regular_bg_map_cell_info(player_map_cell).tile_index();

        // if (player_tile_index != 1) {
            BN_LOG(bn::format<60>("map tile: {}, {}: {}", map_x, map_y, player_tile_index));
            // BN_LOG(bn::format<60>("dog pos: {}, {}", dog_sprite.x(), dog_sprite.y()));
            BN_LOG(bn::format<60>("player pos: {}, {}", jochem_sprite.x(), jochem_sprite.y()));
        // }

        // gravity
        if (player_tile_index == 0) {
            velocity_y += gravity;
        } else {
            jumping = false;
            velocity_y = bn::min(bn::fixed(0.0), velocity_y);
        }
        




        // Update position, test colliders
        y = y + velocity_y;
        jochem_sprite.set_position(x + kick_offset, y);


        // Smooth cam
        if (bn::abs(camera.x() - jochem_sprite.x()) > bn::display::width() / cam_follow_margin || 
            bn::abs(camera.y() - jochem_sprite.y()) > bn::display::height() / cam_follow_margin) {
            camera_moving_to_player = true;
        }

        if (camera_moving_to_player) {
            camera.set_x(lerp(jochem_sprite.x(), camera.x(), 0.95));
            camera.set_y(lerp(jochem_sprite.y(), camera.y(), 0.95));

            if (bn::abs(camera.x() - jochem_sprite.x()) < 1  &&  bn::abs(camera.y() - jochem_sprite.y()) < 1) {
                camera_moving_to_player = false;
            }
        }



        // camera.set_y(dog_sprite.y());
        // camera.set_x(dog_sprite.x());




        bn::core::update();
    }
}
