#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_bg_palettes.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"

#include "bn_sprite_items_jochem.h"
#include "bn_sprite_items_punch.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

#include "../include/utils.h"

int main()
{
    bn::core::init();
    bn::bg_palettes::set_transparent_color( bn::color(42/8, 54/8, 75/8));
    
    while(true)
    {
        bn::core::update();
    }
}
