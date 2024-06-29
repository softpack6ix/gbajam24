#include "bn_core.h"
#include "bn_point.h"
#include "bn_fixed_point.h"
#include "bn_sprite_animate_actions.h"
#include "bn_keypad.h"
#include "bn_regular_bg_map_item.h"
#include "bn_sprite_items_jochem.h"

#include "utils.h"



struct Player {
    enum Animation {
        idle,
        walk,
        jump,
        kick,
        charge_spell
    };


    // fields
    bn::fixed_point position;
    bn::fixed_point velocity;
    bn::fixed gravity;
    bn::fixed jump_velocity = -4.0;

    Animation current_anim;
    bool jumping;
    bool flipped;

    int kick_timer = 0;

    // bolt
    bn::fixed bolt_x = -25;
    bn::fixed bolt_vx = 8;
    int charge_x = -90;
    int bolt_start = charge_x - 25;


    // methods
    Player(bn::camera_ptr cam, bn::fixed grav) 
    {
        position = bn::fixed_point(0,0);
        velocity = bn::fixed_point(0,0);
        gravity = grav;
        current_anim = Animation::idle;

        // Set camera of sprites
        jochem_sprite.set_camera(cam);
        punch_sprite.set_camera(cam);
        charge_sprite.set_camera(cam);
        bolt_sprite.set_camera(cam);
    }


    ~Player() {}



    void update(bn::regular_bg_map_item map_item) 
    {
        // Watch for gravity
        int player_tile_index = get_map_tile_index_at_position(position, map_item);
       
        // jumping and gravity
        if (bn::keypad::a_pressed() && current_anim != Animation::jump) {
            jumping = true;
            current_anim = Animation::jump;
            velocity.set_y(jump_velocity);
        } else {
            // can fall
            if (player_tile_index == 0) {
                velocity.set_y(velocity.y() + gravity);
            }
            // no platform beneath player 
            else {
                jumping = false;
                // current_anim = Animation::idle;
                velocity.set_y(bn::min(bn::fixed(0.0), velocity.y()));
            }
        }
        
        // walking
        if (bn::keypad::left_held()) {
            velocity.set_x(-2);
            current_anim = Animation::walk;
            jochem_sprite.set_horizontal_flip(true);
        }
        if (bn::keypad::right_held()) {
            velocity.set_x(2);
            current_anim = Animation::walk;
            jochem_sprite.set_horizontal_flip(false);
        }
        
        if (!bn::keypad::left_held() && !bn::keypad::right_held()) {
            velocity.set_x(0);
            if (current_anim == Animation::walk) {
                current_anim = Animation::idle;
            }
        }

        // kicking
        if (bn::keypad::b_pressed()) {
            current_anim = Animation::kick;
            kick_timer = 0;
            kicking.reset();
        }

        if (!punch_action.done()) {
            punch_action.update();
        }

        if (current_anim == Animation::kick) {
            if (kick_timer == 20) {
                punch_action.reset();
                int offset = jochem_sprite.horizontal_flip() ? -35 : 35;
                punch_sprite.set_position(position.x()+offset, position.y() -12);
            }
            if (kicking.done()) {
                current_anim = Animation::idle;
            }
            kick_timer++;
        }

        // spell casting
        if (bn::keypad::r_pressed()) {
            charge_action.reset();
            charge_sustain_action.reset();
            charge_discharge_action.reset();
        }

        if (bn::keypad::r_held()) {
            charge_sprite.set_position(position);
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

        if (bn::keypad::r_released()) {
            if (charge_action.done()) {
                charge_action.reset();
                bolt_action.reset();
                bolt_sprite.set_visible(true);
                bolt_sprite.set_position(jochem_sprite.position());
                bolt_x = position.x();
                bolt_vx = jochem_sprite.horizontal_flip() ? -8 : 8;
            }
        }

        if (!bolt_action.done()) {
            // bolt_sprite.set_visible(false);
            bolt_action.update();
            bolt_x += bolt_vx;
            bolt_sprite.set_x(bolt_x);
        }




        // Apply velocity to position, test colliders
        position += velocity;
        
        // Update sprite position
        int kick_offset = current_anim == Animation::kick ? (jochem_sprite.horizontal_flip() ? -10 : 10) : 0;
        jochem_sprite.set_position(position.x() + kick_offset, position.y());

        // Sprite orientation

        // Update the right animation
        if (current_anim == Animation::walk) {
            walking.update();
        }
        if (current_anim == Animation::kick) {
            kicking.update();
        }
        if (current_anim == Animation::idle) {
            idling.update();
        }
    }



    // Map (0,0) is top-left but sprite (0,0) is center-center
    const bn::point get_map_point_at_position(bn::fixed_point pos) 
    {
        int map_x = mod(((pos.x() + bn::display::width() / 2) / 8).floor_integer(), 32);
        int map_y = mod(((pos.y() + bn::display::height() / 2 + 28) / 8).floor_integer(), 32);
        return bn::point(map_x, map_y);
    }

    int get_map_tile_index_at_position(bn::fixed_point pos, bn::regular_bg_map_item map_item) 
    {
        const bn::point p = get_map_point_at_position(pos);
        bn::regular_bg_map_cell map_cell = map_item.cell(p);
        int tile_index = bn::regular_bg_map_cell_info(map_cell).tile_index();

        BN_LOG(bn::format<60>("map tile: {}, {}: {}", p.x(), p.y(), tile_index));
        BN_LOG(bn::format<60>("player pos: {}, {}", position.x(), position.y()));

        return  tile_index;
    }







    // sprites
    bn::sprite_ptr jochem_sprite = bn::sprite_items::jochem.create_sprite(0, 0);
    bn::sprite_ptr punch_sprite = bn::sprite_items::punch.create_sprite(20, 50);
    bn::sprite_ptr charge_sprite = bn::sprite_items::charge.create_sprite(0, 0);
    bn::sprite_ptr bolt_sprite = bn::sprite_items::bolt.create_sprite(0, 0);


    // animations
    bn::sprite_animate_action<14> idling = bn::create_sprite_animate_action_forever(jochem_sprite, 1, bn::sprite_items::jochem.tiles_item(), 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13
    );    

    bn::sprite_animate_action<21> walking = bn::create_sprite_animate_action_forever(jochem_sprite, 1, bn::sprite_items::jochem.tiles_item(), 
        14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34
    );

    bn::sprite_animate_action<25> kicking = bn::create_sprite_animate_action_once(jochem_sprite, 1, bn::sprite_items::jochem.tiles_item(), 
        35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
        48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59
    );


    // charge
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

    // punch animation 
    bn::sprite_animate_action<16> punch_action = bn::create_sprite_animate_action_once(punch_sprite, 1, bn::sprite_items::punch.tiles_item(), 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
    );


    // bolt
    bn::sprite_animate_action<72> bolt_action = bn::create_sprite_animate_action_once(bolt_sprite, 1, bn::sprite_items::bolt.tiles_item(), 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 
        33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 
        50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 
        67, 68, 69, 70, 71
    );
};