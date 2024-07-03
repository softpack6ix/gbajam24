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
#include "bn_link.h"
#include "bn_link_state.h"

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



struct Pickup {
    bn::fixed_point position; 
    bn::sprite_ptr spr;
    bn::sprite_animate_action<60> anim;
    bn::sound_item sound;
};


struct Lipje : Pickup {
    
};


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
    

    // Player and other player
    Player player(camera, gravity);
    Player other_player(camera, gravity);

    Player players[] = {
        player, other_player
    };


    // Lipje pickup sounds
    int clouds_x = 0.0;
    int pickup_i;

    bn::sound_item pickup_sounds[] = {
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
        lipje_animation(lipje_sprites[7])
    };

    for (int i = 0; i < 8; i++) {
        lipje_sprites[i].set_camera(camera);
    }

    // Multiplayer
    int players_counter;
    int current_player_id;
    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::vector<bn::sprite_ptr, 64> info_text_sprites;
    text_generator.set_center_alignment();


    MultiplayerKeypadData last_keypad_data_to_send;


    while(true)
    {
        // Update player
        MultiplayerKeypadData keypad_data_to_send;

        keypad_data_to_send.keypad_data.l_pressed = bn::keypad::l_pressed();
        keypad_data_to_send.keypad_data.r_pressed = bn::keypad::r_pressed();
        keypad_data_to_send.keypad_data.a_pressed = bn::keypad::a_pressed();
        keypad_data_to_send.keypad_data.left_held = bn::keypad::left_held();
        keypad_data_to_send.keypad_data.right_held = bn::keypad::right_held();

        player.update(map_item, keypad_data_to_send.keypad_data);

        if (last_keypad_data_to_send.data != keypad_data_to_send.data) {
            bn::link::send(keypad_data_to_send.data);   
            last_keypad_data_to_send.data = keypad_data_to_send.data;
        }


        // Update other player
        MultiplayerKeypadData other_player_keypad_data;

        if(bn::optional<bn::link_state> link_state = bn::link::receive())
        {
            const bn::link_player& first_other_player = link_state->other_players().front();
            other_player_keypad_data.data = first_other_player.data();


            BN_LOG(bn::format<60>("received: {}", other_player_keypad_data.data));                

            // Update multiplayer info text
            if (players_counter != link_state->player_count() || current_player_id != link_state->current_player_id()) {
                players_counter = link_state->player_count();
                current_player_id = link_state->current_player_id();
                BN_LOG(bn::format<60>("change in link"));                
                bn::string<200> info_text =  bn::format<60>("players: {}, player id: {}", players_counter, current_player_id);
                info_text_sprites.clear();
                text_generator.generate(0, 65, info_text, info_text_sprites);
            }
        }

        other_player.update(map_item, other_player_keypad_data.keypad_data);


        // Lipje pickup items
        for (int i = 0; i < 8; i++) {
            bn::sprite_ptr lip_spr = lipje_sprites[i];
            lipje_actions[i].update();

            bn::fixed magnetic_range = 64;
            
            // Update for both players
            for (int i = 0; i < 2; i++) {
                bn::fixed dist = distance(lip_spr.position(), players[i].sprite_ptr.position());
                if (dist < magnetic_range) {
                    lip_spr.set_x(lerp(lip_spr.x(), players[i].sprite_ptr.x(), 0.2));
                    lip_spr.set_y(lerp(lip_spr.y(), players[i].sprite_ptr.y(), 0.2));
                    lip_spr.set_scale(dist / magnetic_range);
                }

                if (dist < 8 && lip_spr.visible()) {
                    lip_spr.set_visible(false);
                    pickup_sounds[pickup_i].play(1, 1.0, 0.0);
                    pickup_i++;
                    pickup_i = pickup_i % 4;
                }
            }
        }
        
        // Moving clouds
        clouds_x -=  0.1;
        clouds.set_x(clouds_x + player.position.x() / bn::fixed(40.0));

        // Smooth cam
        camera_follow_smooth(camera, player.sprite_ptr.position());

        // Move clouds/mountains bg parallax
        sky.set_position(-player.position / bn::fixed(50.0));

        bn::core::update();
    }
}
