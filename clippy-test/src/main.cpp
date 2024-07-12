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


// bgs
#include "bn_regular_bg_items_healthbar.h"

// sprites
#include "bn_sprite_items_healthbar_frame.h"
#include "bn_sprite_items_healthbar_fill.h"
#include "bn_sprite_items_boss_frame.h"
#include "bn_sprite_items_clippy.h"




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



struct boss_healthbar
{
    int x = -bn::display::width() / 2 + 24;
    int y = -bn::display::height() / 2 + 24;
    
    bn::sprite_ptr boss_thumb;
    bn::sprite_ptr boss_frame = bn::sprite_items::boss_frame.create_sprite(x, y);
    bn::sprite_ptr healthbar_fill = bn::sprite_items::healthbar_fill.create_sprite(x + 48, y+3);
    bn::sprite_ptr healthbar_frame = bn::sprite_items::healthbar_frame.create_sprite(x + 48, y);

    boss_healthbar(bn::sprite_item boss_spr_item): 
        boss_thumb(boss_spr_item.create_sprite(x, y))
    {
        boss_thumb.set_z_order(-1);
    }


    // 0 - 1
    void set_health(bn::fixed health)
    {
        healthbar_fill.set_horizontal_scale(bn::max(health, bn::fixed(0.01)));
        healthbar_fill.set_x(x + map(health, 0, 1, 16, 48));
    }
};



int main()
{
    bn::core::init();
    bn::bg_palettes::set_transparent_color(rgb255(42, 54, 75));
        
    info_printer printer;

    boss_healthbar healthbar(bn::sprite_items::clippy);
    healthbar.set_health(1);

    auto clippy1 = bn::sprite_items::clippy.create_sprite(0,0);
    auto clippy2 = bn::sprite_items::clippy.create_sprite(40,0);
    
    while(true)
    {
        move_with_dpad(clippy2);
        printer.print(bn::format<20>("{}", sprites_touch(clippy1, clippy2)));

        bn::core::update();
    }
}


