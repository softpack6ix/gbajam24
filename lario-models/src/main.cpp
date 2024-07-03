#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"
#include "bn_regular_bg_map_cell_info.h"
#include "bn_sprite_text_generator.h"
#include "bn_point.h"
#include "bn_log.h"
#include "bn_format.h"
#include "bn_math.h"
#include "bn_display.h"


// Sounds
#include "bn_audio.h"
#include "bn_sound_items.h"

// BGs
#include "bn_regular_bg_items_tilemap.h"
#include "bn_regular_bg_items_sky.h"
#include "bn_regular_bg_items_clouds.h"

// Sprites
#include "bn_sprite_items_lipje_item.h"



// Font
#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

// Includes
#include "player.h"
#include "utils.h"


bn::sprite_animate_action<60> lipje_animation(bn::sprite_ptr spr) 
{
    return bn::create_sprite_animate_action_forever(spr, 1, bn::sprite_items::lipje_item.tiles_item(),
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
        21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50
    );
}


bn::fixed distance(bn::fixed_point a, bn::fixed_point b) 
{
    return abs(a.x() - b.x()) + abs(a.y() - b.y());
}


int main()
{
    bn::core::init();
    bn::bg_palettes::set_transparent_color(bn::color(16, 24, 29));

    // World
    const bn::fixed gravity = 0.3;

    // BG and map
    bn::regular_bg_ptr sky = bn::regular_bg_items::sky.create_bg(0, 0);
    bn::regular_bg_ptr clouds = bn::regular_bg_items::clouds.create_bg(0, 0);
    bn::regular_bg_ptr tilemap = bn::regular_bg_items::tilemap.create_bg(bn::display::width() / 2, bn::display::height() / 2);
    const bn::regular_bg_map_item& map_item = bn::regular_bg_items::tilemap.map_item();

    // Camera    
    bn::camera_ptr camera = bn::camera_ptr::create(0, 0);
    tilemap.set_camera(camera);
    // clouds.set_camera(camera);
    
    // Jochem player
    Player player(camera, gravity);


    // Lipje pickup sounds
    int clouds_x = 0.0;
    int pickup_i;

    bn::sound_item pickups[] = {
        bn::sound_items::pickup_1,
        bn::sound_items::pickup_2,
        bn::sound_items::pickup_3,
        bn::sound_items::pickup_4
    };

    int x_offset = 140;
    int y_offset = 126;

    bn::sprite_ptr lipje_sprites[] = {
        bn::sprite_items::lipje_item.create_sprite(0+x_offset,y_offset),
        bn::sprite_items::lipje_item.create_sprite(32+x_offset,y_offset),
        bn::sprite_items::lipje_item.create_sprite(64+x_offset,y_offset),
        bn::sprite_items::lipje_item.create_sprite(96+x_offset,y_offset),

        bn::sprite_items::lipje_item.create_sprite(0+x_offset,-y_offset/2),
        bn::sprite_items::lipje_item.create_sprite(32+x_offset,-y_offset/2),
        bn::sprite_items::lipje_item.create_sprite(64+x_offset,-y_offset/2),
        bn::sprite_items::lipje_item.create_sprite(96+x_offset,-y_offset/2)
    };

    bn::sprite_animate_action<60> lipje_actions[] = {
        lipje_animation(lipje_sprites[0]),
        lipje_animation(lipje_sprites[1]),
        lipje_animation(lipje_sprites[2]),
        lipje_animation(lipje_sprites[3]),
        lipje_animation(lipje_sprites[4]),
        lipje_animation(lipje_sprites[5]),
        lipje_animation(lipje_sprites[6]),
        lipje_animation(lipje_sprites[7]),
    };

    for (int i = 0; i < 8; i++) {
        lipje_sprites[i].set_camera(camera);
    }


    while(true)
    {
        clouds_x -=  0.1;
        player.update(map_item);


        // Lipje
        for (int i = 0; i < 8; i++) {
            bn::sprite_ptr lip_spr = lipje_sprites[i];
            lipje_actions[i].update();

            if (distance(lip_spr.position(), player.sprite_ptr.position()) < 64) {
                lip_spr.set_x(lerp(lip_spr.x(), player.sprite_ptr.x(), 0.3));
                lip_spr.set_y(lerp(lip_spr.y(), player.sprite_ptr.y(), 0.3));
            }

            if (distance(lip_spr.position(), player.sprite_ptr.position()) < 8 && lip_spr.visible()) {
                lip_spr.set_visible(false);
                pickups[pickup_i].play(1, 1.0, 0.0);
                pickup_i++;
                pickup_i = pickup_i % 4;
            }
        }
        
        // Moving clouds
        clouds.set_x(clouds_x + player.position.x() / bn::fixed(40.0));

        // Smooth cam
        camera_follow_smooth(camera, player.sprite_ptr.position());

        // Move clouds/mountains bg parallax
        sky.set_position(-player.position / bn::fixed(50.0));

        bn::core::update();
    }
}
