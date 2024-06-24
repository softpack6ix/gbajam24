#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"
#include "bn_log.h"
#include "bn_sprite_items_guave.h"
#include "bn_format.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

#include "vector"


int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(13, 14, 21));


    // constexpr bn::string_view info_text_lines[] = {
    //     "PAD: change sprite's direction",
    //     "",
    //     "START: go to next scene",
    // };

    // common::info info("Sprites animation actions", info_text_lines  , text_generator);

    bn::sprite_ptr guave_sprite = bn::sprite_items::guave.create_sprite(0, 0);

    bn::sprite_animate_action<12> anim = bn::create_sprite_animate_action_forever(guave_sprite, 32, bn::sprite_items::guave.tiles_item(), 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
    );    

    guave_sprite.set_scale(1);



    int frame = 0;

    while(true)
    {
        frame++;
        
        BN_LOG(bn::format<32>("frame: {}", frame));    



        anim.update();

        // info.update();        
        bn::core::update();
    }
}
