#pragma once

#include "bn_core.h"
#include "bn_math.h"
#include "bn_color.h"
#include "bn_vector.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_ptr.h"

// includes
#include "../include/utils.h"
#include "scene.h"
#include "player.h"
#include "pickups.h"
#include "globals.h"
#include "platforming_level.h"


// backgrounds
#include "bn_regular_bg_items_soapgoreslides.h"


namespace platforming_level 
{
    struct dreft_land : level 
    {
        bn::color background_color = bn::color(bn::color(28, 26, 28));
        bn::regular_bg_item tilemap_item = bn::regular_bg_items::soapgoreslides;
        bn::fixed_point tilemap_position = bn::fixed_point(bn::display::width() / 2 + 256, bn::display::height() / 2 + 256);

        dreft_land() 
        {
           
        }

        void update()
        {

        }
    };
}