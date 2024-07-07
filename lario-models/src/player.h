#pragma once

#include "bn_core.h"
#include "bn_point.h"
#include "bn_fixed_point.h"
#include "bn_sprite_animate_actions.h"
#include "bn_keypad.h"
#include "bn_regular_bg_map_item.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"
#include "bn_regular_bg_builder.h"
#include "bn_sprite_text_generator.h"
#include "bn_regular_bg_animate_actions.h"
#include "bn_log.h"
#include "bn_vector.h"


#include "bn_sprite_items_rein_lario.h"
#include "bn_sprite_items_jochem_lario.h"
#include "bn_sprite_items_lario_lario.h"


#include "../include/utils.h"


struct character_animations {
    bn::sprite_animate_action<50> idle;
    bn::sprite_animate_action<40> run;

    bn::sprite_animate_action<20> jump_up;
    bn::sprite_animate_action<2> jump_stay;
    bn::sprite_animate_action<60> jump_down;
};


namespace multiplayer 
{
    // The union allows you to convert the keypress data
    // to a single 'int' to be able to send it over link.
    union keypad_data {
        struct keypad_data_struct {
            int l_pressed : 1;
            int r_pressed : 1;
            int a_pressed : 1;

            int left_held : 1;
            int right_held : 1;
        } keypad_data;
        
        int data;
    };
}





struct player {

    // fields
    bn::fixed_point position;
    bn::fixed_point velocity;
    bn::fixed gravity;
    bn::fixed jump_velocity = -7.0;
    bn::fixed run_speed = 4;

    bool is_jumping;
    bool is_running;
    bool flipped;
    bool is_landing;
    bool is_falling;

    // sprites
    bn::sprite_item sprite_item = bn::sprite_items::jochem_lario;
    bn::sprite_ptr sprite_ptr = sprite_item.create_sprite(0, 0);


    enum Character {
        Rein, Jochem, Lario
    };

    int current_character = Rein;

    character_animations character_anims[3] = {
        character_animations {
            idle: bn::create_sprite_animate_action_forever(sprite_ptr, 1, bn::sprite_items::rein_lario.tiles_item(), 
                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37
            ),
            run: bn::create_sprite_animate_action_forever(sprite_ptr, 1, bn::sprite_items::rein_lario.tiles_item(), 
                38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55
            ),
            jump_up: bn::create_sprite_animate_action_once(sprite_ptr, 1, bn::sprite_items::rein_lario.tiles_item(), 
                56, 57, 58, 59, 60
            ),
            jump_stay: bn::create_sprite_animate_action_forever(sprite_ptr, 1, bn::sprite_items::rein_lario.tiles_item(), 
                61, 61
            ),
            jump_down: bn::create_sprite_animate_action_once(sprite_ptr, 1, bn::sprite_items::rein_lario.tiles_item(), 
                62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83
            )
        },

        character_animations {
            idle: bn::create_sprite_animate_action_forever(sprite_ptr, 1, bn::sprite_items::jochem_lario.tiles_item(), 
                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
                21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 
                41, 42, 43, 44, 45, 46, 47, 48
            ),
            run: bn::create_sprite_animate_action_forever(sprite_ptr, 1, bn::sprite_items::jochem_lario.tiles_item(), 
                50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64
            ),
            jump_up: bn::create_sprite_animate_action_once(sprite_ptr, 1, bn::sprite_items::jochem_lario.tiles_item(), 
                65, 66, 67, 68, 69
            ),
            jump_stay: bn::create_sprite_animate_action_forever(sprite_ptr, 1, bn::sprite_items::jochem_lario.tiles_item(), 
                70, 70
            ),
            jump_down: bn::create_sprite_animate_action_once(sprite_ptr, 1, bn::sprite_items::jochem_lario.tiles_item(), 
                71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85
            )
        },

        character_animations {
            idle: bn::create_sprite_animate_action_forever(sprite_ptr, 1, bn::sprite_items::lario_lario.tiles_item(), 
                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
                21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 
                41, 42, 43, 44, 45, 46, 47
            ),
            run: bn::create_sprite_animate_action_forever(sprite_ptr, 1, bn::sprite_items::lario_lario.tiles_item(), 
                48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62
            ),
            jump_up: bn::create_sprite_animate_action_once(sprite_ptr, 1, bn::sprite_items::lario_lario.tiles_item(), 
                63, 64, 65, 66, 67, 68
            ),
            jump_stay: bn::create_sprite_animate_action_forever(sprite_ptr, 1, bn::sprite_items::lario_lario.tiles_item(), 
                69, 69
            ),
            jump_down: bn::create_sprite_animate_action_once(sprite_ptr, 1, bn::sprite_items::lario_lario.tiles_item(), 
                70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96
            )
        }
    };


    // methods
    player(bn::camera_ptr cam, bn::fixed grav) 
    {
        position = bn::fixed_point(0,0);
        velocity = bn::fixed_point(0,0);
        gravity = grav;

        // Set camera of sprites
        sprite_ptr.set_camera(cam);
    }


    ~player() {}



    void update(bn::regular_bg_map_item map_item, multiplayer::keypad_data::keypad_data_struct keypad_data) 
    {
        // character switching 
        if (keypad_data.l_pressed) {
            current_character -= 1;
        }
        
        if (keypad_data.r_pressed) {
            current_character += 1;
        }
        
        if (current_character > 2) current_character = 0;
        if (current_character < 0) current_character = 2;


        // Watch for gravity
        int player_tile_index = get_map_tile_index_at_position(position, map_item);
        int ground_tiles[] = {1, 2, 3, 4, 16, 17, 18, 19, 20, 21, 22, 23 };
        bool on_ground = false;

        // BN_LOG(bn::format<20>("tile: {}", player_tile_index));
       
    //    for (int tile_index: ground_tiles) {
    //         if (player_tile_index == tile_index) {
    //             on_ground = true;
    //         }
    //    }

        if (player_tile_index != 0) {
            on_ground = true;
        }
       

        // platform beneath player 
        if (on_ground) {
            if ((is_jumping) && velocity.y() > 0) {
                is_jumping = false;
                is_landing = true;
            }

            if (is_falling) {
                is_falling = false;
                is_landing = true;
            }

            velocity.set_y(bn::min(bn::fixed(0.0), velocity.y()));
        }
        // can fall
        else {
            velocity.set_y(velocity.y() + gravity);
            is_falling = true;
        }

        // jumping and gravity
        if (keypad_data.a_pressed && !is_jumping && on_ground) {
            is_jumping = true;
            is_landing = false;
            velocity.set_y(jump_velocity);
            character_anims[current_character].jump_down.reset();
            character_anims[current_character].jump_up.reset();
        }
        // running
        if (keypad_data.left_held) {
            is_landing = false;
            velocity.set_x(-run_speed);
            sprite_ptr.set_horizontal_flip(true);
            if (on_ground) {
                is_running = true;
                is_landing = false;
            }
        }

        if (keypad_data.right_held) {
            velocity.set_x(run_speed);
            if (on_ground) {
                is_running = true;
                is_landing = false;
            }
            sprite_ptr.set_horizontal_flip(false);
        }
        
        if (!keypad_data.left_held && !keypad_data.right_held) {
            velocity.set_x(0);
            if (is_running) {
                is_running = false;
            }
        }


        // Apply velocity to position
        position += velocity;
        
        // Update sprite position
        sprite_ptr.set_position(position.x(), position.y());

        // Update the right animation
        if (is_falling && !is_jumping && !is_landing) {
            character_anims[current_character].jump_stay.update();
        }
        else if (is_running && !is_jumping) {
            character_anims[current_character].run.update();
        }
        else if (is_jumping) {
            if (character_anims[current_character].jump_up.done()) {
                character_anims[current_character].jump_stay.update();
            } else {
                character_anims[current_character].jump_up.update();
            }
        } else if (is_landing && !character_anims[current_character].jump_down.done()) {
            character_anims[current_character].jump_down.update();
        }
        else {
            character_anims[current_character].idle.update();
        }
    }
};







// Global
bn::vector<player, 4> players;



