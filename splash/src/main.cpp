#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_blending.h"
#include "bn_keypad.h"
#include "bn_colors.h"
#include "bn_display.h"
#include "bn_color_effect.h"
#include "bn_bgs_mosaic.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"
#include "bn_bg_palette_actions.h"
#include "bn_bg_palettes_actions.h"
#include "bn_sprite_text_generator.h"
// #include "bn_sprite_palette_actions.h"
// #include "bn_sprite_palettes_actions.h"
#include "bn_format.h"

#include "bn_sprite_items_splash_1.h"
#include "bn_sprite_items_splash_2.h"
#include "bn_sprite_items_splash_3.h"
#include "bn_sprite_items_splash_4.h"
#include "bn_sprite_items_splash_5.h"
#include "bn_sprite_items_splash_6.h"

#include "bn_sprite_items_lipje.h"


#include "common_info.h"
#include "common_variable_8x8_sprite_font.h"
// #include "common_fixed_8x8_sprite_font.h"


#include "bn_music_items.h"


bn::sprite_animate_action<81> splash_action(const bn::sprite_ptr &sprite, const bn::sprite_item &sprite_item) 
{
    bn::sprite_animate_action<81> splash_action = bn::create_sprite_animate_action_once(sprite, 1, sprite_item.tiles_item(), 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 
        32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
        48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
        64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 
        80
    );

    return splash_action;
}



int main()
{
    bn::core::init();

    bn::bg_palettes::set_transparent_color(bn::color(0, 0, 0));

    // 6 sprites (64x32 pixels)
    int x_offset = 10;
    int y_offset = 15;

    bn::sprite_item sprite_items[] {
        bn::sprite_items::splash_1,
        bn::sprite_items::splash_2,
        bn::sprite_items::splash_3,
        bn::sprite_items::splash_4,
        bn::sprite_items::splash_5,
        bn::sprite_items::splash_6
    };

    int sprite_ys[] = {
        -64, -64,
        0, 0, 
        64, 64
    };

    bn::sprite_ptr splash_sprite_ptrs[] {
        bn::sprite_items::splash_1.create_sprite(-64+x_offset, sprite_ys[0]+y_offset),
        bn::sprite_items::splash_2.create_sprite(64+x_offset, sprite_ys[1]+y_offset),
        bn::sprite_items::splash_3.create_sprite(-64+x_offset, sprite_ys[2]+y_offset),
        bn::sprite_items::splash_4.create_sprite(64+x_offset, sprite_ys[3]+y_offset),
        bn::sprite_items::splash_5.create_sprite(-64+x_offset, sprite_ys[4]+y_offset),
        bn::sprite_items::splash_6.create_sprite(64+x_offset, sprite_ys[5]+y_offset)
    };

    bn::sprite_animate_action<81> splash_anim_actions[6] {
        splash_action(splash_sprite_ptrs[0], sprite_items[0]),
        splash_action(splash_sprite_ptrs[1], sprite_items[1]),
        splash_action(splash_sprite_ptrs[2], sprite_items[2]),
        splash_action(splash_sprite_ptrs[3], sprite_items[3]),
        splash_action(splash_sprite_ptrs[4], sprite_items[4]),
        splash_action(splash_sprite_ptrs[5], sprite_items[5])
    };



    // lipje
    bn::sprite_ptr lipje = bn::sprite_items::lipje.create_sprite(0, 0);
    bn::sprite_animate_action<120> lipje_anim = bn::create_sprite_animate_action_once(lipje, 1, bn::sprite_items::lipje.tiles_item(), 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
        21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 
        40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 
        59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 
        78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 93, 93, 93, 
        93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93
    );


    for (size_t i = 0; i < 6; i++) {
        splash_sprite_ptrs[i].set_scale(2);
    }

    lipje.set_scale(2);


    // bn::music_items::splashscreen.play();

    // bn::music_item brkz = bn::music_items::breakagez;
    // brkz.play();

    // bn::sprite_palette_ptr palette =  splash_sprite_ptrs[0].palette();
    // palette.set_fade_color(bn::colors::black);
    // palette.set_fade_intensity(1.0);
    // bn::sprite_palette_fade_to_action fade_action(palette, 120, 0.0);

    bn::sprite_text_generator text_generator(common::variable_8x8_sprite_font);
    // bn::string<200> info_text =  bn::format<60>("A SOFTPACK6IX PRODUCTION");
    bn::string<200> info_text =  bn::format<60>("a softpacksix production");
    bn::vector<bn::sprite_ptr, 64> info_text_sprites;
    text_generator.set_center_alignment();
    
    
    bool generated = false;

    bn::fixed y = 80.0;

    while(true)
    {
        for (size_t i = 0; i < 6; i++) {
            if (splash_anim_actions[i].done()) {
                if (!generated) {
                    text_generator.generate(0, 55, info_text, info_text_sprites);
                    generated = true;
                    y = 80.0;
                }
            } else {
                splash_anim_actions[i].update();
            }
        }


        if (splash_anim_actions[0].done() && !lipje_anim.done()) {
            lipje_anim.update();

            if (lipje_anim.current_index() < 20) {
                y -= 0.5;
            } else if (lipje_anim.current_index() > 60) {
                y += 0.5;
            }

            for (int i = 0; i < info_text_sprites.size(); i++) {
                info_text_sprites.at(i).set_y(y);
            }
        }


        if (lipje_anim.done() && splash_anim_actions[0].done()) {
            for (size_t i = 0; i < 6; i++) {
                info_text_sprites.clear();
                splash_anim_actions[i].reset();
                lipje_anim.reset();
                generated = false;
                // fade_action.reset();
                // palette.set_fade_intensity(1.0);
            }
        }

        bn::core::update();
    }
}