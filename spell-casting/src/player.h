#include "bn_core.h"
#include "bn_point.h"
#include "bn_fixed_point.h"
#include "bn_sprite_animate_actions.h"
#include "bn_keypad.h"

#include "bn_sprite_items_jochem.h"


struct Player {
    enum Animation {
        idle,
        walk,
        jump,
        kick,
        charge_spell
    };


    // methods
    Player() {
        position = bn::fixed_point(0,0);
        velocity = bn::fixed_point(0,0);
        current_anim = Animation::idle;
    }

    ~Player() {}



    void update(bn::regular_bg_map_item map_item) {

        
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

        if (bn::keypad::a_pressed()) {
            current_anim = Animation::kick;
            kick_timer = 0;
            kicking.reset();
        }

        // if (!punch_action.done()) {
        //     punch_action.update();
        // }




        if (current_anim == Animation::kick) {
            if (kick_timer == 20) {
                // punch_action.reset();
                // int offset = jochem_sprite.horizontal_flip() ? -35 : 35;
                // punch_sprite.set_position(x+offset, y-12);
            }
            if (kicking.done()) {
                current_anim = Animation::idle;
            }
            kick_timer++;
        }

        

        // Watch for gravity
        int player_tile_index = get_map_tile_index_at_position(position, map_item);
       

        // jumping and gravity
        if (bn::keypad::up_pressed() && current_anim != Animation::jump) {
            jumping = true;
            current_anim = Animation::jump;
            velocity.set_y(-5.5);
        } else {
            // can fall
            if (player_tile_index == 0) {
                velocity.set_y(velocity.y() + gravity);
            }
            // no platform beneath player 
            else {
                jumping = false;
                velocity.set_y(bn::min(bn::fixed(0.0), velocity.y()));
            }
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
    int get_map_tile_index_at_position(bn::fixed_point pos, bn::regular_bg_map_item map_item) 
    {
        int map_x = ((pos.x() + bn::display::width() / 2) / 8).floor_integer() % 32;
        int map_y = ((pos.y() + bn::display::height() / 2 + 28) / 8).floor_integer() % 32;


        bn::regular_bg_map_cell map_cell = map_item.cell(bn::point(map_x, map_y));
        int tile_index = bn::regular_bg_map_cell_info(map_cell).tile_index();

        BN_LOG(bn::format<60>("map tile: {}, {}: {}", map_x, map_y, tile_index));
        BN_LOG(bn::format<60>("player pos: {}, {}", jochem_sprite.x(), jochem_sprite.y()));

        return  tile_index;
    }



    // fields
    bn::fixed_point position;
    bn::fixed_point velocity;
    bn::fixed gravity = 0.1;

    Animation current_anim;
    bool jumping;
    bool flipped;

    int kick_timer = 0;



    // sprites
    bn::sprite_ptr jochem_sprite = bn::sprite_items::jochem.create_sprite(0, 0);

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
};