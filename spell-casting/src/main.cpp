#include "bn_core.h"

#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"
#include "bn_regular_bg_actions.h"
#include "bn_regular_bg_builder.h"
#include "bn_regular_bg_attributes.h"
#include "bn_regular_bg_map_cell_info.h"
#include "bn_sprite_text_generator.h"
#include "bn_regular_bg_animate_actions.h"
#include "bn_regular_bg_position_hbe_ptr.h"
#include "bn_regular_bg_attributes_hbe_ptr.h"
#include "bn_point.h"
#include "bn_log.h"
#include "bn_format.h"
#include "bn_math.h"
#include "bn_display.h"

// BGs
#include "bn_regular_bg_items_tilemap.h"

// Sprites
#include "bn_sprite_items_charge.h"
#include "bn_sprite_items_bolt.h"
#include "bn_sprite_items_wormhole.h"
#include "bn_sprite_items_jochem.h"
#include "bn_sprite_items_punch.h"
#include "bn_sprite_items_dog.h"

// Font
#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

// Test
#include "player.h"
#include "utils.h"






bool camera_moving_to_point = false;
int cam_follow_margin = 3;


void camera_follow_smooth(bn::camera_ptr cam, bn::fixed_point position) 
{
    if (bn::abs(cam.x() - position.x()) > bn::display::width() / cam_follow_margin || 
        bn::abs(cam.y() - position.y()) > bn::display::height() / cam_follow_margin) {
        camera_moving_to_point = true;
    }

    if (camera_moving_to_point) {
        cam.set_x(lerp(position.x(), cam.x(), 0.95));
        cam.set_y(lerp(position.y(), cam.y(), 0.95));

        if (bn::abs(cam.x() - position.x()) < 1  &&  bn::abs(cam.y() - position.y()) < 1) {
            camera_moving_to_point = false;
        }
    }
}


int main()
{
    bn::core::init();
    bn::bg_palettes::set_transparent_color(bn::color(16, 24, 29));

    // World
    const bn::fixed gravity = 0.1;

    // BG and map
    bn::regular_bg_ptr tilemap = bn::regular_bg_items::tilemap.create_bg(8, 48);
    const bn::regular_bg_map_item& map_item = bn::regular_bg_items::tilemap.map_item();

    // Camera    
    bn::camera_ptr camera = bn::camera_ptr::create(0, 0);
    tilemap.set_camera(camera);


    // Jochem player
    Player player(camera, gravity);


    while(true)
    {
        player.update(map_item);
        
        // Smooth cam
        camera_follow_smooth(camera, player.jochem_sprite.position());

        bn::core::update();
    }
}
