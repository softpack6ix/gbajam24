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
#include "bn_music.h"
#include "bn_audio.h"



// BGs
#include "bn_regular_bg_items_tilemap.h"
#include "bn_regular_bg_items_sky.h"
#include "bn_regular_bg_items_clouds.h"
#include "bn_regular_bg_items_soapgoreslides.h"

// Sprites

// Font

// Includes
#include "../include/utils.h"
#include "globals.h"
#include "player.h"
#include "pickups.h"
#include "splash.h"
#include "main_menu.h"
#include "casette_player.h"

// Sounds
#include "bn_sound_items.h"
#include "bn_music_items.h"

// levels
#include "level.h"
#include "level_default.h"
#include "level_clippy_boss.h"
#include "level_dreftland.h"
#include "level_kermis.h"
#include "level_michaelsoft.h"
#include "level_oceangate.h"
#include "level_reins_lab.h"
#include "level_the_tower.h"
#include "level_thuisbezorgd.h"




int main()
{
    bn::core::init();


    // The global camera and printer
    camera = bn::camera_ptr::create(0, 0);
    printer = info_printer();


    // Splash
    bn::music_items::splashscreen.play();
    splash::run();
    bn::music::stop();
    BN_LOG("splash done!");

    // Main menu
    main_menu::next_scene next = main_menu::run();

    if (next == main_menu::next_scene::casette) 
        casette_player::run();


    // The current
    levels::default_level level;
    bn::bg_palettes::set_transparent_color(level.background_color);


    // World
    const bn::fixed gravity = 0.3;


    // BG and map
    bn::regular_bg_ptr tilemap = level.tilemap_item.create_bg(level.tilemap_position);
    const bn::regular_bg_map_item& map_item = level.tilemap_item.map_item();
    tilemap.set_camera(*camera);


    // Player and other player, soon to be max. 4
    player you(*camera, gravity);
    player other_player(*camera, gravity);

    players.push_back(you);
    players.push_back(other_player);
    
    bn::fixed_point last_position;

    // Multiplayer
    int players_counter;
    int current_player_id;
    multiplayer::keypad_data last_keypad_data_to_send;


    while(true)
    {
        // Update level
        level.update();   

        // Update player, send keypad to other players
        multiplayer::keypad_data keypad_data_to_send {
            keypad_data: multiplayer::keypad_data::keypad_data_struct {
                l_pressed: bn::keypad::l_pressed(),
                r_pressed: bn::keypad::r_pressed(),
                a_pressed: bn::keypad::a_pressed(),
                left_held: bn::keypad::left_held(),
                right_held: bn::keypad::right_held()
            }
        };

        // Always update own player
        you.update(map_item , keypad_data_to_send.keypad_data);


        // Print map info when singleplayer
        if (players_counter == 0) {
            // map_info_printer.print_map_tiles_at_position(map_item, player.position);
            if (you.position != last_position) {
                last_position = you.position;
                // printer->print_map_tile_and_position(map_item, you.position);
            }
        }

        log_memory_usage();

        // Send if changed
        if (last_keypad_data_to_send.data != keypad_data_to_send.data) {
            bn::link::send(keypad_data_to_send.data);   
            last_keypad_data_to_send.data = keypad_data_to_send.data;
        }


        // Update other player, receive keypad from other players
        multiplayer::keypad_data other_player_keypad_data;

        if(bn::optional<bn::link_state> link_state = bn::link::receive())
        {
            const bn::link_player& first_other_player = link_state->other_players().front();
            other_player_keypad_data.data = first_other_player.data();

            BN_LOG(bn::format<40>("received: {}", other_player_keypad_data.data));                

            // Update multiplayer info text
            if (players_counter != link_state->player_count()) {
                players_counter = link_state->player_count();
                current_player_id = link_state->current_player_id();

                // Immediately refresh the other player as well
                bn::link::send(keypad_data_to_send.data);   
                BN_LOG("change in link");                
                bn::string<40> info_text = bn::format<40>("players: {}, player id: {}", players_counter, current_player_id);
                printer->print(info_text);
            }
        }
        
        // always update for animations
        other_player.update(map_item, other_player_keypad_data.keypad_data);


        // Smooth cam
        camera_follow_smooth(*camera, you.sprite_ptr.position());

        bn::core::update();
    }
}
