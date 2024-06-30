#include "bn_core.h"
#include "bn_point.h"
#include "bn_fixed_point.h"
#include "bn_sprite_animate_actions.h"
#include "bn_keypad.h"
#include "bn_regular_bg_map_item.h"
#include "bn_sprite_items_jochem.h"
#include "bn_sprite_items_dust_sliding.h"

#include "utils.h"



struct Player {

    // fields
    bn::fixed_point position;
    bn::fixed_point velocity;
    bn::fixed gravity;
    bn::fixed jump_velocity = -7.0;
    bn::fixed run_speed = 4;

    bool is_jumping;
    bool is_kicking;
    bool is_running;
    bool is_crouching;
    bool is_sliding;
    
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
        position = bn::fixed_point(-8,0);
        velocity = bn::fixed_point(0,0);
        gravity = grav;

        // Set camera of sprites
        jochem_sprite.set_camera(cam);
        punch_sprite.set_camera(cam);
        charge_sprite.set_camera(cam);
        bolt_sprite.set_camera(cam);
        dust_sliding_sprite.set_camera(cam);
    }


    ~Player() {}



    void update(bn::regular_bg_map_item map_item) 
    {
        // Watch for gravity
        int player_tile_index = get_map_tile_index_at_position(position, map_item);
        // int ground_tiles[] = {15, 16, 17, 18, 19, 1, 2};
        int ground_tiles[] = {17, 18, 40, 41, 42, 43};
        bool on_ground = false;
       
    //    for (size_t i = 0; i < 7; i++) {
    //         if (player_tile_index == ground_tiles[i]) {
    //             on_ground = true;
    //         }
    //    }

        if (player_tile_index != 0) {
            on_ground = true;
        }
       

        // can fall
        if (on_ground) {
            if (is_jumping && velocity.y() > 0) {
                is_jumping = false;
                anim_jump_down.reset();
            }
            
            velocity.set_y(bn::min(bn::fixed(0.0), velocity.y()));
        }
        // platform beneath player 
        else {
            velocity.set_y(velocity.y() + gravity);
        }

        // jumping and gravity
        if (bn::keypad::a_pressed() && !is_jumping) {
            is_jumping = true;
            velocity.set_y(jump_velocity);
            anim_jump_up.reset();
            anim_jump_down.reset();
        }
        

        // sliding
        if (is_running && bn::keypad::b_pressed()) {
            is_sliding = true;
            anim_slide.reset();
            dust_sliding_sprite.set_visible(true);
        }

        if (is_sliding) {
            if (anim_slide.done()) {
                is_sliding = false;
            } else {
                bool flip = jochem_sprite.horizontal_flip();
                dust_sliding_sprite.set_position(position + bn::point(flip ? -45 : 45,14));
                dust_sliding_sprite.set_horizontal_flip(flip);
            }
        }

        if (bn::keypad::b_released()) {
            is_sliding = false;
            dust_sliding_action.reset();
            dust_sliding_sprite.set_visible(false);
        }

        
        // running
        if (bn::keypad::left_held()) {
            velocity.set_x(-run_speed);
            jochem_sprite.set_horizontal_flip(true);
            if (on_ground && !is_sliding) {
                is_running = true;
            }
        }

        if (bn::keypad::right_held()) {
            velocity.set_x(run_speed);
            if (on_ground && !is_sliding) {
                is_running = true;
            }
            jochem_sprite.set_horizontal_flip(false);
        }
        
        if (!bn::keypad::left_held() && !bn::keypad::right_held()) {
            velocity.set_x(0);
            if (is_running) {
                is_running = false;
            }
        }

        // kicking
        if (!is_running && bn::keypad::b_pressed()) {
            is_kicking = true;
            kick_timer = 0;
            anim_kick.reset();
        }

        if (!punch_action.done()) {
            punch_action.update();
        }

        if (is_kicking) {
            if (kick_timer == 20) {
                punch_action.reset();
                int offset = jochem_sprite.horizontal_flip() ? -35 : 35;
                punch_sprite.set_position(position.x()+offset, position.y() -12);
            }
            if (anim_kick.done()) {
                is_kicking = false;
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




        // Apply velocity to position
        position += velocity;
        
        // Update sprite position
        int kick_offset = is_kicking ? (jochem_sprite.horizontal_flip() ? -10 : 10) : 0;
        jochem_sprite.set_position(position.x() + kick_offset, position.y());



        // Update the right animation
        if (is_running && !is_jumping) {
            if (is_sliding && !anim_slide.done()) {
                anim_slide.update();
                dust_sliding_action.update();
            } else {
                anim_run.update();
            }
        } 
        else if (is_kicking && !anim_kick.done()) {
            anim_kick.update();
        } 
        else if (is_jumping) {
            if (anim_jump_up.done()) {
                anim_jump_stay.update();
            } else {
                anim_jump_up.update();
            }
        } else if (on_ground && !anim_jump_down.done()) {
            anim_jump_down.update();
        }
         else {
            anim_idle.update();
        }
    }





    // sprites
    bn::sprite_ptr jochem_sprite = bn::sprite_items::jochem.create_sprite(0, 0);
    bn::sprite_ptr punch_sprite = bn::sprite_items::punch.create_sprite(20, 50);
    bn::sprite_ptr charge_sprite = bn::sprite_items::charge.create_sprite(0, 0);
    bn::sprite_ptr bolt_sprite = bn::sprite_items::bolt.create_sprite(0, 0);
    bn::sprite_ptr dust_sliding_sprite = bn::sprite_items::dust_sliding.create_sprite(0, 0);


    // animations
    bn::sprite_animate_action<14> anim_idle = bn::create_sprite_animate_action_forever(jochem_sprite, 1, bn::sprite_items::jochem.tiles_item(), 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13
    );    

    bn::sprite_animate_action<21> anim_walk = bn::create_sprite_animate_action_forever(jochem_sprite, 1, bn::sprite_items::jochem.tiles_item(), 
        14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34
    );

    bn::sprite_animate_action<25> anim_kick = bn::create_sprite_animate_action_once(jochem_sprite, 1, bn::sprite_items::jochem.tiles_item(), 
        35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
        48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59
    );

    bn::sprite_animate_action<13> anim_jump_up = bn::create_sprite_animate_action_once(jochem_sprite, 1, bn::sprite_items::jochem.tiles_item(), 
        60, 61, 62, 63, 64, 65
    );

    bn::sprite_animate_action<2> anim_jump_stay = bn::create_sprite_animate_action_forever(jochem_sprite, 1, bn::sprite_items::jochem.tiles_item(), 
        66, 66
    );

    bn::sprite_animate_action<10> anim_jump_down = bn::create_sprite_animate_action_once(jochem_sprite, 1, bn::sprite_items::jochem.tiles_item(), 
        //67, 68, 69, 70, 71, 72, 
        74, 75, 76, 77, 78, 79, 80, 81, 82
    );

    bn::sprite_animate_action<20> anim_run = bn::create_sprite_animate_action_forever(jochem_sprite, 1, bn::sprite_items::jochem.tiles_item(), 
        83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 
        98, 99, 100, 101, 102
    );

    bn::sprite_animate_action<40> anim_slide = bn::create_sprite_animate_action_once(jochem_sprite, 1, bn::sprite_items::jochem.tiles_item(), 
        103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 
        116, 117, 118, 119, 120, 121, 122, 121, 120, 119, 118, 117, 116, 
        115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103
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

    // dust sliding
    bn::sprite_animate_action<42> dust_sliding_action = bn::create_sprite_animate_action_once(dust_sliding_sprite, 1, bn::sprite_items::dust_sliding.tiles_item(), 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 
        33, 34, 35, 36, 37, 38, 39, 40, 41
    );

};