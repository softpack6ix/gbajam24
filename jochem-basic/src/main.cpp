#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"

#include "bn_sprite_items_jochem.h"
#include "bn_sprite_items_punch.h"
#include "bn_music_items.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

#include "vector"








int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(0, 15, 15));

    bn::music_items::kp6minibeat.play();

    constexpr bn::string_view info_text_lines[] = {
        "PAD: change sprite's direction",
        "",
        "START: go to next scene",
    };

    common::info info("Sprites animation actions", info_text_lines  , text_generator);

    bn::sprite_ptr jochem_sprite = bn::sprite_items::jochem.create_sprite(0, 0);
    bn::sprite_ptr punch_sprite = bn::sprite_items::punch.create_sprite(20, 50);

    bn::sprite_animate_action<27> idle = bn::create_sprite_animate_action_forever(jochem_sprite, 1, bn::sprite_items::jochem.tiles_item(), 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26
    );    

    bn::sprite_animate_action<50> walking = bn::create_sprite_animate_action_forever(jochem_sprite, 1, bn::sprite_items::jochem.tiles_item(), 
        27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
        48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
        60, 61, 62, 63, 64, 65, 66, 67, 68
    );

    bn::sprite_animate_action<62> kicking = bn::create_sprite_animate_action_once(jochem_sprite, 1, bn::sprite_items::jochem.tiles_item(), 
        69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 
        95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 
        117, 118, 118

    );


    bn::sprite_animate_action<16> punch_action = bn::create_sprite_animate_action_once(punch_sprite, 1, bn::sprite_items::punch.tiles_item(), 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
    );


    


    int x = 0;
    int y = 0;
    bool wasMoving = false;
    bool moving = false;
    bool isKicking = false;

    int kickTimer = 0;
    
    while(true)
    {
        moving = false;
        

        if(bn::keypad::left_held()) {
            x--;
            moving = true;
            jochem_sprite.set_horizontal_flip(true);
        }
        if(bn::keypad::right_held()) {
            x++;
            moving = true;
            jochem_sprite.set_horizontal_flip(false);
        }
        if(bn::keypad::up_held()) {
            y--;
            moving = true;
        }
        if(bn::keypad::down_held()) {
            y++;
            moving = true;
        }

        if(bn::keypad::a_pressed()) {
            isKicking = true;
            kickTimer = 0;
            kicking.reset();
        }

        if (!punch_action.done()) {
            punch_action.update();
        }

        int kick_offset = jochem_sprite.horizontal_flip() ? -10 : 10;


        if (moving) {
            if (isKicking) {
                jochem_sprite.set_position(x, y);
            } else {
                walking.update();
                jochem_sprite.set_position(x, y);
            }
        } 

        if (isKicking) {
            jochem_sprite.set_position(x + kick_offset, y);

            if (kickTimer == 40) {
                punch_action.reset();
                int offset = jochem_sprite.horizontal_flip() ? -35 : 35;
                punch_sprite.set_position(x+offset, y-12);
            }
            if (!kicking.done()) {
                kicking.update();
            } else {
                isKicking = false;
            }


            kickTimer++;
        }


        if (!moving && !isKicking) {    
            idle.update();
            jochem_sprite.set_position(x, y);
        }



        
        // idle.update();



        info.update();        
        bn::core::update();

        wasMoving = moving;
    }
}
