#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"

#include "bn_sprite_items_rein.h"
#include "bn_sprite_items_punch.h"
#include "bn_sprite_items_mud.h"
#include "bn_sprite_items_wormhole.h"
#include "bn_music_items.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

#include "vector"


std::vector<bn::sprite_animate_action<15>> punches {};


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

    bn::sprite_ptr wormhole_sprite = bn::sprite_items::wormhole.create_sprite(75, 0);
    bn::sprite_ptr rein_sprite = bn::sprite_items::rein.create_sprite(0, 0);
    bn::sprite_ptr mud_sprite = bn::sprite_items::mud.create_sprite(-50, 0);
    bn::sprite_ptr punch_sprite = bn::sprite_items::punch.create_sprite(20, 50);

    bn::sprite_animate_action<96> action = bn::create_sprite_animate_action_forever(rein_sprite, 1, bn::sprite_items::rein.tiles_item(), 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 
        32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
        48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
        64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 
        80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95
    );


    bn::sprite_animate_action<90> mud_action = bn::create_sprite_animate_action_forever(mud_sprite, 1, bn::sprite_items::mud.tiles_item(), 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 
        32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
        48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
        64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 
        80, 81, 82, 83, 84, 85, 86, 87, 88, 89
    );

     bn::sprite_animate_action<64> wormhole_action = bn::create_sprite_animate_action_forever(wormhole_sprite, 1, bn::sprite_items::wormhole.tiles_item(), 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 
        32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
        48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62
    );


    bn::sprite_animate_action<15> punch_action = bn::create_sprite_animate_action_once(punch_sprite, 1, bn::sprite_items::punch.tiles_item(), 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14
    );



    int x = 0;
    int y = 0;
    
    while(true)
    {
        if(bn::keypad::pressed(bn::keypad::key_type::A)) {
            punch_action.reset();
            punch_sprite.set_position(x+10, y);
        }

        if(bn::keypad::left_held()) {
            x--;
        }
        if(bn::keypad::right_held()) {
            x++;
        }
        if(bn::keypad::up_held()) {
            y--;
        }
        if(bn::keypad::down_held()) {
            y++;
        }

        if (!punch_action.done()) {
            punch_action.update();
        }

        rein_sprite.set_position(x, y);

        action.update();
        mud_action.update();
        wormhole_action.update();
        info.update();        
        bn::core::update();
    }
}
