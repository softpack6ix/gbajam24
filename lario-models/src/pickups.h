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

#include "player.h"






namespace Pickups 
{
    bn::sound_item pickup_sounds[4] = {
        bn::sound_items::pickup_1,
        bn::sound_items::pickup_2,
        bn::sound_items::pickup_3,
        bn::sound_items::pickup_4
    };

    int pickup_i; 


    struct Lipje 
    {
        // variables
        const int respawn_after = 60 * 10;
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
        
        Lipje(bn::fixed x, bn::fixed y, bn::camera_ptr camera) {
            original_position = bn::fixed_point(x, y);

            spr.set_position(original_position);
            twinkle_spr.set_position(original_position);
            spr.set_camera(camera);
            twinkle_spr.set_camera(camera);
        }

        void update(Player players[]) {
            anim.update();
            twinkle_anim.update();

            bn::fixed magnetic_range = 64;

            // Update for both players
            for (int i = 0; i < 2; i++) {
                bn::fixed dist = distance(spr.position(), players[i].sprite_ptr.position());
                if (dist < magnetic_range) {
                    bn::fixed x = lerp(spr.x(), players[i].sprite_ptr.x(), 0.2);
                    bn::fixed y = lerp(spr.y(), players[i].sprite_ptr.y(), 0.2);
                    spr.set_position(x, y);
                    twinkle_spr.set_position(x, y);
                    spr.set_scale(dist / magnetic_range);
                }

                if (dist < 8 && spr.visible()) {
                    spr.set_visible(false);
                    twinkle_spr.set_visible(false);
                    is_respawning = true;
                    pickup_sounds[pickup_i].play(1, 1.0, 0.0);
                    pickup_i++;
                    pickup_i = pickup_i % 4;
                }
            }

            // Respawn logic
            if (is_respawning) {
                respawn_t++;

                if (respawn_t >= respawn_after) {
                    respawn_t = 0;
                    is_respawning = false;
                    spr.set_scale(1);
                    spr.set_visible(true);
                    twinkle_spr.set_scale(1);
                    twinkle_spr.set_visible(true);
                    spr.set_position(original_position);
                    twinkle_spr.set_position(original_position);
                }
            }
        }
    };

}
