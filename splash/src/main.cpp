#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_blending.h"
#include "bn_bgs_mosaic.h"
#include "bn_bg_palettes.h"
#include "bn_regular_bg_actions.h"
#include "bn_regular_bg_builder.h"
#include "bn_regular_bg_attributes.h"
#include "bn_sprite_text_generator.h"
#include "bn_regular_bg_animate_actions.h"
#include "bn_regular_bg_position_hbe_ptr.h"
#include "bn_regular_bg_attributes_hbe_ptr.h"


#include "bn_regular_bg_items_dragon.h"
#include "bn_regular_bg_items_splash.h"
#include "bn_sprite_animate_actions.h"

#include "bn_sprite_items_splash_1.h"
#include "bn_sprite_items_splash_2.h"
#include "bn_sprite_items_splash_3.h"
#include "bn_sprite_items_splash_4.h"
#include "bn_sprite_items_splash_5.h"
#include "bn_sprite_items_splash_6.h"


#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

#include "bn_music_items.h"



int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(0, 0, 0));

    int x_offset = 10;
    int y_offset = 15;

    bn::sprite_ptr splash_1 = bn::sprite_items::splash_1.create_sprite(-64+x_offset, -64+y_offset);
    bn::sprite_ptr splash_2 = bn::sprite_items::splash_2.create_sprite(64+x_offset, -64+y_offset);

    bn::sprite_ptr splash_3 = bn::sprite_items::splash_3.create_sprite(-64+x_offset, 0+y_offset);
    bn::sprite_ptr splash_4 = bn::sprite_items::splash_4.create_sprite(64+x_offset, 0+y_offset);

    bn::sprite_ptr splash_5 = bn::sprite_items::splash_5.create_sprite(-64+x_offset, 64+y_offset);
    bn::sprite_ptr splash_6 = bn::sprite_items::splash_6.create_sprite(64+x_offset, 64+y_offset);
    

    splash_1.set_scale(2);
    splash_2.set_scale(2);
    splash_3.set_scale(2);
    splash_4.set_scale(2);
    splash_5.set_scale(2);
    splash_6.set_scale(2);

    
    bn::sprite_animate_action<81> splash_1_action = bn::create_sprite_animate_action_forever(splash_1, 1, bn::sprite_items::splash_1.tiles_item(), 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 
        32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
        48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
        64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 
        80
    );

    bn::sprite_animate_action<81> splash_2_action = bn::create_sprite_animate_action_forever(splash_2, 1, bn::sprite_items::splash_2.tiles_item(), 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 
        32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
        48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
        64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 
        80
    );

    bn::sprite_animate_action<81> splash_3_action = bn::create_sprite_animate_action_forever(splash_3, 1, bn::sprite_items::splash_3.tiles_item(), 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 
        32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
        48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
        64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 
        80
    );

    bn::sprite_animate_action<81> splash_4_action = bn::create_sprite_animate_action_forever(splash_4, 1, bn::sprite_items::splash_4.tiles_item(), 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 
        32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
        48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
        64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 
        80
    );

    bn::sprite_animate_action<81> splash_5_action = bn::create_sprite_animate_action_forever(splash_5, 1, bn::sprite_items::splash_5.tiles_item(), 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 
        32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
        48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
        64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 
        80
    );

    bn::sprite_animate_action<81> splash_6_action = bn::create_sprite_animate_action_forever(splash_6, 1, bn::sprite_items::splash_6.tiles_item(), 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 
        32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
        48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
        64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 
        80
    );

    bn::music_items::breakagez.play();



    while(true)
    {
        splash_1_action.update();
        splash_2_action.update();
        splash_3_action.update();
        splash_4_action.update();
        splash_5_action.update();
        splash_6_action.update();

        bn::core::update();
    }
}
