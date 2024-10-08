#pragma once

#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"
#include "bn_format.h"
#include "bn_log.h"

#include "bn_sprite_items_splash_1.h"
#include "bn_sprite_items_splash_2.h"
#include "bn_sprite_items_splash_3.h"
#include "bn_sprite_items_splash_4.h"
#include "bn_sprite_items_splash_5.h"
#include "bn_sprite_items_splash_6.h"
#include "bn_sprite_items_lipje.h"

#include "bn_music_items.h"

#include "globals.h"
#include "scene.h"


namespace splash 
{
    // important
    bool is_done;

    // KP6 balloons: 
    // 6 sprites (64x32 pixels)
    constexpr bn::sprite_item sprite_items[] {
        bn::sprite_items::splash_1,
        bn::sprite_items::splash_2,
        bn::sprite_items::splash_3,
        bn::sprite_items::splash_4,
        bn::sprite_items::splash_5,
        bn::sprite_items::splash_6
    };

    bn::point balloons_offset = bn::point(10, 15);

    bn::point sprite_positions[] = {
        bn::point(-64, -64), bn::point(64, -64),
        bn::point(-64, 0), bn::point(64, 0),
        bn::point(-64, 64), bn::point(64, 64),
    };
    
   


    // lipje
    const int splash_spr_count = 6;
    bn::optional<bn::sprite_ptr> splash_sprite_ptrs[6];
    bn::optional<bn::sprite_animate_action<82>> splash_anim_actions[6];

    bn::optional<bn::sprite_ptr> lipje;
    bn::optional<bn::sprite_animate_action<120>> lipje_anim;


    // 'a softpacksix production' text
    bn::fixed text_y = 80.0;
    bool generated = false;
    bool splash_sprite_prs_removed = false;

    next_scene run() 
    {
        

        for (size_t i = 0; i < splash_spr_count; i++) {
            splash_sprite_ptrs[i] = sprite_items[i].create_sprite(sprite_positions[i] + balloons_offset);
            splash_anim_actions[i] = bn::create_sprite_animate_action_once(*splash_sprite_ptrs[i], 1, sprite_items[i].tiles_item(), 
                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
                16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 
                32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
                48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
                64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 
                80, 81
            );

            splash_sprite_ptrs[i]->set_scale(2);
        }

        lipje = bn::sprite_items::lipje.create_sprite(0, 0);
        lipje->set_scale(2);
        lipje_anim = bn::create_sprite_animate_action_once(*lipje.get(), 1, bn::sprite_items::lipje.tiles_item(), 
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
            21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 
            40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 
            59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 
            78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 93, 93, 93, 
            93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93
        );

        lipje_anim->reset();


        bn::music_items::splashscreen.play();

        while(true) 
        {
            // Update balloons
            for (auto &anim : splash_anim_actions) {
                if (anim->done()) {
                    if (!generated) {
                        generated = true;
                        printer->print("a softpacksix production");
                        text_y = 80.0;
                    }
                } else {
                    anim->update();
                }
            }

            // 'KP6' balloons are done, update text and lipje
            if (splash_anim_actions[0]->done() && !lipje_anim->done()) {
                lipje_anim->update();

                if (lipje_anim->current_index() < 20) {
                    text_y -= 0.5;
                } else if (lipje_anim->current_index() > 60) {
                    text_y += 0.5;
                }

                for (auto &spr : printer->info_text_sprites) {
                    spr.set_y(text_y);
                }
            }

            // Everything is done
            if (lipje_anim->done()) {
                // Unload everything, animations before ...!
                lipje.reset();
                lipje_anim.reset();
                
                for (size_t i = 0; i < splash_spr_count; i++) {
                    splash_sprite_ptrs[i]->~sprite_ptr();
                    splash_sprite_ptrs[i].reset();
                }

                bn::music::stop();
                return next_scene::main_menu;
            }

            bn::core::update(); 
        }
    }
}


