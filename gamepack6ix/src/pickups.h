#pragma once


#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sound_items.h"
#include "bn_optional.h"
#include "bn_camera_ptr.h"


#include "bn_sprite_items_lipje_item.h"
#include "bn_sprite_items_twinkle.h"


// includes
#include "player.h"
#include "globals.h"

#include "bn_sound_items.h"



namespace pickups 
{   
    bn::sound_item pickup_sounds[4] = {
        bn::sound_items::pickup_1,
        bn::sound_items::pickup_2,
        bn::sound_items::pickup_3,
        bn::sound_items::pickup_4
    };

    int pickup_i;
    int reset_pickup_i_t = 0;
    int reset_pickup_i_after = 60 * 5;

    struct pickup 
    {
        virtual void update(bn::vector<player, 4> players);
    };


    struct lipje
    {
        // variables
        const int respawn_after = 60 * 3;
        int respawn_t = 0;
        bool is_respawning;
        bn::fixed_point original_position;

        // graphics
        bn::sprite_ptr twinkle_spr = bn::sprite_items::twinkle.create_sprite(bn::fixed_point(0, 0));
        bn::sprite_animate_action<60> twinkle_anim = bn::create_sprite_animate_action_forever(twinkle_spr, 1, bn::sprite_items::twinkle.tiles_item(), 
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
            21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 
            41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58
        );

        bn::sprite_ptr spr = bn::sprite_items::lipje_item.create_sprite(bn::fixed_point(0, 0));
        bn::sprite_animate_action<60> anim = bn::create_sprite_animate_action_forever(spr, 1, bn::sprite_items::lipje_item.tiles_item(), 
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
            21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 
            41, 42, 43, 44, 45, 46, 47, 48, 49, 50
        );
        
        lipje(bn::fixed x, bn::fixed y) {
            original_position = bn::fixed_point(x, y);

            spr.set_position(original_position);
            twinkle_spr.set_position(original_position);
            spr.set_camera(*camera);
            twinkle_spr.set_camera(*camera);
        }

        void update() {
            anim.update();
            twinkle_anim.update();

            bn::fixed magnetic_range = 64;

            // Respawn logic
            if (is_respawning) {
                respawn_t++;

                if (respawn_t >= respawn_after) {
                    respawn_t = 0;
                    is_respawning = false;
                    spr.set_visible(true);
                    twinkle_spr.set_visible(true);
                    spr.set_position(original_position);
                    twinkle_spr.set_position(original_position);
                }
            } else {
                bool close_to_a_player = false;

                // Update for both players
                for (player &p : players) {
                    bn::fixed dist = distance(spr.position(), p.sprite_ptr.position());

                    // Magnetic
                    if (dist < magnetic_range) {
                        close_to_a_player = true;
                        bn::fixed_point new_pos = lerp(spr.position(), p.sprite_ptr.position(), 0.2);
                        spr.set_position(new_pos);
                        twinkle_spr.set_position(new_pos);
                        spr.set_scale(dist / magnetic_range);
                    }

                    // Absorb
                    if (dist < 16 && spr.visible()) {
                        spr.set_visible(false);
                        twinkle_spr.set_visible(false);
                        is_respawning = true;
                        pickup_sounds[pickup_i].play(1, 1.0, 0.0);
                        pickup_i++;
                        // pickup_i = pickup_i % 4;
                        reset_pickup_i_t = 0;
                    }
                }

                // Grow back
                if (!close_to_a_player) {
                    spr.set_scale(lerp(1.0, spr.vertical_scale(), 0.8));
                    twinkle_spr.set_scale(lerp(1.0, twinkle_spr.vertical_scale(), 0.8));
                }
            }

            // Reset sound effects index;
            reset_pickup_i_t++;
            if (reset_pickup_i_t > reset_pickup_i_after) {
                pickup_i = 0;
                reset_pickup_i_t = 0;
            }
        }
    };

}
