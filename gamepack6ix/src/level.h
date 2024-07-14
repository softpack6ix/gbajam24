#pragma once

#include "bn_core.h"
#include "bn_math.h"
#include "bn_color.h"
#include "bn_vector.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_ptr.h"

// includes
#include "../include/utils.h"
#include "player.h"
#include "pickups.h"
#include "globals.h"

// backgrounds
#include "bn_regular_bg_items_tilemap.h"

enum class next_scene
{
    casette,
    platforming
};


namespace levels 
{
    struct level
    {
        bn::color background_color = bn::color(16, 24, 29);
        bn::regular_bg_item tilemap_item = bn::regular_bg_items::tilemap;
        bn::fixed_point tilemap_position = bn::fixed_point(bn::display::width() / 2, bn::display::height() / 2);

        virtual void update();
    };

}