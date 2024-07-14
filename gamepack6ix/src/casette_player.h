#pragma once

#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_bg_palettes.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_item.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"


#include "../include/utils.h"
#include "level.h"

#include "bn_music_items.h"
#include "bn_music.h"

#include "bn_regular_bg_items_casette_bg.h"


namespace casette_player
{
    struct song 
    {
        const char name[20];
        const bn::music_item music_item;
    };


    constexpr song songs[] = 
    {
        song 
        {
            "space debris",
            bn::music_items::space_debris
        },
        song 
        {
            "hymn_to_aurora",
            bn::music_items::hymn_to_aurora
        },
        song 
        {
            "deadlock",
            bn::music_items::deadlock
        },
    };


    int current_song = 0;


    next_scene run()
    {
        auto casette_bg = bn::regular_bg_items::casette_bg.create_bg(0,0);
        songs[current_song].music_item.play();
                
        while(true)
        {
            if (bn::keypad::left_pressed() || bn::keypad::right_pressed()) {
                bn::music::stop();

                if (bn::keypad::left_pressed()) {
                    current_song--;
                }
                if (bn::keypad::right_pressed()) {
                    current_song++;
                }

                current_song = mod(current_song, 3);
                songs[current_song].music_item.play();
            }

            if (bn::keypad::l_held()) {
                bn::music::set_tempo(2);
                bn::music::set_pitch(2);
            }
            if (bn::keypad::r_held()) {
                bn::music::set_tempo(2);
                bn::music::set_pitch(2);
            }

            if (!bn::keypad::l_held() && !bn::keypad::r_held()) {
                bn::music::set_tempo(1);
                bn::music::set_pitch(1);
            }
            
            printer->print(songs[current_song].name);
            bn::core::update();
        }
    }
};