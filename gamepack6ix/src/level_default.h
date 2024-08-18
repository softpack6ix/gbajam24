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
#include "platforming_level.h"


// backgrounds
#include "bn_regular_bg_items_tilemap.h"
#include "bn_regular_bg_items_sky.h"
#include "bn_regular_bg_items_clouds.h"



namespace platforming_level 
{
    struct default_level {
        bn::color background_color = bn::color(16, 24, 29);
        bn::regular_bg_item tilemap_item = bn::regular_bg_items::tilemap;
        bn::fixed_point tilemap_position = bn::fixed_point(bn::display::width() / 2, bn::display::height() / 2);

        // sky and mountains background
        bn::regular_bg_ptr sky = bn::regular_bg_items::sky.create_bg(0, 0);
        bn::regular_bg_ptr clouds = bn::regular_bg_items::clouds.create_bg(0, 0);
        
        int clouds_x = 0.0;


        // pickups
        const bn::fixed x_offset = 120;
        const bn::fixed y_offset = 126;

        pickups::lipje lipjes[7] = 
        {
            pickups::lipje(0+x_offset,-y_offset/2),
            pickups::lipje(32+x_offset,-y_offset/2),
            pickups::lipje(64+x_offset,-y_offset/2),
            pickups::lipje(96+x_offset,-y_offset/2),
            pickups::lipje(32+x_offset,y_offset),
            pickups::lipje(64+x_offset,y_offset),
            pickups::lipje(96+x_offset,y_offset)
        };


        default_level() 
        {
            // TODO: scan the map for pickups

            // clouds.set_camera(camera);
        }

        void update()
        {
            // Lipje pickup items
            for (size_t i = 0; i < 7; i++)
            {
                lipjes[i].update();
            }
            


            // Moving clouds
            clouds_x -=  0.1;
            // clouds.set_x(clouds_x + player.position.x() / bn::fixed(40.0));


            // Move clouds/mountains bg parallax
            // sky.set_position(-player.position / bn::fixed(50.0));
        }
    };
}