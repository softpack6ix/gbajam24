#include "bn_core.h"
#include "bn_point.h"
#include "bn_fixed_point.h"
#include "bn_sprite_animate_actions.h"
#include "bn_keypad.h"
#include "bn_regular_bg_map_item.h"
#include "bn_sprite_items_jochem.h"
#include "bn_sprite_items_dust_sliding.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"
#include "bn_regular_bg_builder.h"
#include "bn_sprite_text_generator.h"
#include "bn_regular_bg_animate_actions.h"

#include "utils.h"

#include "bn_sprite_items_rein_lario.h"





struct CharacterAnimations {
    bn::sprite_animate_action<40> idle;
    bn::sprite_animate_action<40> run;

    bn::sprite_animate_action<20> jump_up;
    bn::sprite_animate_action<2> jump_stay;
    bn::sprite_animate_action<20> jump_down;
};




struct Player {

    // fields
    bn::fixed_point position;
    bn::fixed_point velocity;
    bn::fixed gravity;
    bn::fixed jump_velocity = -7.0;
    bn::fixed run_speed = 4;

    bool is_jumping;
    bool is_running;
    bool flipped;


    // sprites
    bn::sprite_item sprite_item = bn::sprite_items::jochem;
    bn::sprite_ptr sprite_ptr = sprite_item.create_sprite(0, 0);


    enum Character {
        Rein, Jochem, Lario
    };

    Character current_character = Rein;

    CharacterAnimations character_animations[1] = {
        CharacterAnimations {
            idle: bn::create_sprite_animate_action_forever(sprite_ptr, 1, bn::sprite_items::rein_lario.tiles_item(), 
                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37
            ),
            run: bn::create_sprite_animate_action_forever(sprite_ptr, 1, bn::sprite_items::rein_lario.tiles_item(), 
                38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55
            ),
            jump_up: bn::create_sprite_animate_action_once(sprite_ptr, 1, bn::sprite_items::rein_lario.tiles_item(), 
                56, 57, 58, 59, 60, 61, 62
            ),
            jump_stay: bn::create_sprite_animate_action_forever(sprite_ptr, 1, bn::sprite_items::rein_lario.tiles_item(), 
                63, 63
            ),
            jump_down: bn::create_sprite_animate_action_once(sprite_ptr, 1, bn::sprite_items::rein_lario.tiles_item(), 
                64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75
            )
        }
    };


    // methods
    Player(bn::camera_ptr cam, bn::fixed grav) 
    {
        position = bn::fixed_point(0,0);
        velocity = bn::fixed_point(0,0);
        gravity = grav;

        // Set camera of sprites
        sprite_ptr.set_camera(cam);
    }


    ~Player() {}



    void switch_character()
    {

    }


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

             if (is_jumping && velocity.y() > 0) {
                is_jumping = false;
            }
        }
       

        // platform beneath player 
        if (on_ground) {
            velocity.set_y(bn::min(bn::fixed(0.0), velocity.y()));
        }
        // can fall
        else {
            velocity.set_y(velocity.y() + gravity);
        }

        // jumping and gravity
        if (bn::keypad::a_pressed() && !is_jumping) {
            is_jumping = true;
            velocity.set_y(jump_velocity);
            character_animations[current_character].jump_down.reset();
            character_animations[current_character].jump_up.reset();
        }
        // running
        if (bn::keypad::left_held()) {
            velocity.set_x(-run_speed);
            sprite_ptr.set_horizontal_flip(true);
            if (on_ground) {
                is_running = true;
            }
        }

        if (bn::keypad::right_held()) {
            velocity.set_x(run_speed);
            if (on_ground) {
                is_running = true;
            }
            sprite_ptr.set_horizontal_flip(false);
        }
        
        if (!bn::keypad::left_held() && !bn::keypad::right_held()) {
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
        if (is_running && !is_jumping) {
            character_animations[current_character].run.update();
        }

        else if (is_jumping) {
            if (character_animations[current_character].jump_up.done()) {
                character_animations[current_character].jump_stay.update();
            } else {
                character_animations[current_character].jump_up.update();
            }
        } else if (on_ground && !character_animations[current_character].jump_down.done()) {
            character_animations[current_character].jump_down.update();
        }
        else {
            character_animations[current_character].idle.update();
        }
    }
};