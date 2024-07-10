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


// bgs
#include "bn_regular_bg_items_healthbar.h"


struct eye
{
    bn::fixed_point position;
    bn::fixed_point pupil_offset;
};

struct
{
    bn::fixed_point position;
    bn::vector<eye, 20> eyes;
} clippy;




int main()
{
    bn::core::init();
    bn::bg_palettes::set_transparent_color( bn::color(42/8, 54/8, 75/8));

    bn::regular_bg_ptr healthbar = bn::regular_bg_items::healthbar.create_bg(0,0);
    
    while(true)
    {
        bn::core::update();
    }
}
