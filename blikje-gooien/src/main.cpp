#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
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

#include "../../include/utils.h"


#include "bn_sprite_items_klok.h"
#include "bn_sprite_items_klok_small.h"



int main()
{
    bn::core::init();

    int start_x = -32;
    int distance = 48;

    


    bn::sprite_ptr klok = bn::sprite_items::klok.create_sprite(-distance,0);
    bn::sprite_ptr klok2 = bn::sprite_items::klok.create_sprite(0,0);
    bn::sprite_ptr klok3 = bn::sprite_items::klok.create_sprite(distance,0);
    bn::sprite_ptr klok4 = bn::sprite_items::klok.create_sprite(distance * 2,0);

    bn::sprite_ptr kloks[] = {
        klok, klok2, klok3, klok4
    };


    while(true)
    {
        for (auto &klok : kloks) {
            klok.set_x(klok.x() - 1);
        }

        bn::core::update();
    }
}
