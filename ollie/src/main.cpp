#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_bg_palettes.h"
#include "bn_bg_palette_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"
#include "bn_regular_bg_builder.h"
#include "bn_sprite_text_generator.h"
#include "bn_regular_bg_animate_actions.h"
#include "bn_point.h"
#include "bn_log.h"
#include "bn_format.h"
#include "bn_math.h"
#include "bn_display.h"
#include "bn_camera_ptr.h"
#include "bn_sprites_mosaic.h"
#include "bn_bgs_mosaic.h"
#include "bn_random.h"

#include "bn_sprite_items_fruitvliegje.h"
#include "../../include/utils.h"


using namespace bn;


fixed grav = 1;
fixed_point vel;
bool jumping = false;
fixed jump_angle = 30.0;

// void jump(sprite_ptr ollie)
// {
// if (keypad::up_pressed()) {
//             ollie.set_y(ollie.x() + grav);
//             while()
//             grav
//         }
// }

int main()
{

    core::init();

    bg_palettes::set_transparent_color(bn::color(20, 20, 20));

    sprite_ptr ollie =  sprite_items::fruitvliegje.create_sprite(0,48);
    
    random rrrr;


    while(true)
    {   
        if (keypad::left_held()) {
            ollie.set_x(ollie.x() - 3);
        }

        if (keypad::right_held()) {
            ollie.set_x(ollie.x() + 3);
        }

        if (keypad::a_pressed() && !jumping) {
            vel.set_y(-10);
            jumping = true;
            jump_angle = rrrr.get_fixed(40.0) + -20;
        }

        // apply velocity and if 
        // if (ollie.y() < 49 && jumping) {
        ollie.set_y(ollie.y() + vel.y());
        if (jumping)  {
            vel.set_y(vel.y() + grav);
        }

        ollie.set_rotation_angle(jumping ? mod((int)rrrr.get_fixed(5.0) + (int) jump_angle, 360) : 0);
        if (ollie.y() >= 48 && jumping == true) {
            vel.set_y(0);
            jumping = false;
        }

        core::update();
    }
}
