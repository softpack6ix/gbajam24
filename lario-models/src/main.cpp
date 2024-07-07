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
#include "bn_regular_bg_items_soapgoreslides.h"

// Sprites
#include "bn_sprite_items_twinkle.h"


// Font
#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

// Includes
#include "../include/utils.h"
#include "player.h"
#include "pickups.h"
#include "level.h"



int main()
{
    bn::core::init();
    

    // The global camera    
    camera = bn::camera_ptr::create(0, 0);


    levels::dreft_land level;
    bn::bg_palettes::set_transparent_color(level.background_color);


    // World
    const bn::fixed gravity = 0.3;


    // BG and map
    bn::regular_bg_ptr tilemap = level.tilemap_item.create_bg(level.tilemap_position);
    const bn::regular_bg_map_item& map_item = level.tilemap_item.map_item();
    tilemap.set_camera(*camera);


    // Player and other player
    player you(*camera, gravity);
    player other_player(*camera, gravity);


    player players[] = {
        you, other_player
    };



    // Multiplayer
    int players_counter;
    int current_player_id;
    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::vector<bn::sprite_ptr, 64> info_text_sprites;
    text_generator.set_center_alignment();


    multiplayer_keypad_data last_keypad_data_to_send;

    
    // Map info
    MapInfoPrinter map_info_printer(common::variable_8x16_sprite_font);


    while(true)
    {
        // Update level
        level.update(players);   

        // Update player, send keypad to other players
        multiplayer_keypad_data keypad_data_to_send {
            keypad_data: multiplayer_keypad_data::keypad_data_struct {
                l_pressed: bn::keypad::l_pressed(),
                r_pressed: bn::keypad::r_pressed(),
                a_pressed: bn::keypad::a_pressed(),
                left_held: bn::keypad::left_held(),
                right_held: bn::keypad::right_held()
            }
        };

        // Always update own player
        you.update(map_item, keypad_data_to_send.keypad_data);

        // map_info_printer.print_map_tiles_at_position(map_item, player.position);
        map_info_printer.print_map_tile_and_position(map_item, you.position);


        // Send if changed
        if (last_keypad_data_to_send.data != keypad_data_to_send.data) {
            bn::link::send(keypad_data_to_send.data);   
            last_keypad_data_to_send.data = keypad_data_to_send.data;
        }


        // Update other player, receive keypad from other players
        multiplayer_keypad_data other_player_keypad_data;

        if(bn::optional<bn::link_state> link_state = bn::link::receive())
        {
            const bn::link_player& first_other_player = link_state->other_players().front();
            other_player_keypad_data.data = first_other_player.data();


            BN_LOG(bn::format<60>("received: {}", other_player_keypad_data.data));                

            // Update multiplayer info text
            if (players_counter != link_state->player_count()) {
                players_counter = link_state->player_count();
                current_player_id = link_state->current_player_id();

                // Immediately refresh the other player as well
                bn::link::send(keypad_data_to_send.data);   
                BN_LOG(bn::format<60>("change in link"));                
                bn::string<60> info_text = bn::format<60>("players: {}, player id: {}", players_counter, current_player_id);
                info_text_sprites.clear();
                text_generator.generate(0, 65, info_text, info_text_sprites);
            }
        }

        other_player.update(map_item, other_player_keypad_data.keypad_data);


        // Smooth cam
        camera_follow_smooth(*camera, you.sprite_ptr.position());

        bn::core::update();
    }
}
