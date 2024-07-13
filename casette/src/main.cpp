#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_bg_palettes.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

#include "../include/utils.h"

#include "bn_music_items.h"


struct song 
{
    bn::string<20> name;
    bn::music_item music_item;
};


song songs[] = 
{
    song 
    {
        "space debris",
        bn::music_items::space_debris
    },
    song 
    {
        "deadlock",
        bn::music_items::deadlock
    },
    song 
    {
        "hymn_to_aurora",
        bn::music_items::hymn_to_aurora
    }
};


int current_song = 0;

int main()
{
    bn::core::init();
    bn::bg_palettes::set_transparent_color(rgb255(100,100,100));

    info_printer printer;


    songs[current_song].music_item.play();
            

    while(true)
    {
        if (bn::keypad::left_pressed()) {
            current_song--;
        }
        if (bn::keypad::right_pressed()) {
            current_song++;
        }

        if (bn::keypad::left_pressed() || bn::keypad::right_pressed()) {
            current_song = mod(current_song, 3);
            songs[current_song].music_item.play();
        }

        
        printer.print(songs[current_song].name);
        bn::core::update();
    }
}
