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
#include "bn_link.h"
#include "bn_link_state.h"

// BGs
#include "bn_regular_bg_items_tilemap.h"
#include "bn_regular_bg_items_sky.h"

// Sprites
#include "bn_sprite_items_charge.h"
#include "bn_sprite_items_bolt.h"
#include "bn_sprite_items_wormhole.h"
#include "bn_sprite_items_jochem.h"
#include "bn_sprite_items_jochem_evil.h"
#include "bn_sprite_items_punch.h"
#include "bn_sprite_items_dog.h"

// Font
#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

// Test
#include "player.h"
#include "utils.h"


#define BN_CFG_LINK_BAUD_RATE BN_LINK_BAUD_RATE_115200_BPS
// #define LOG


namespace multiplayer {
    enum MultiplayerVariables {
        x, y, graphics_index, flip
    };    

    // A struct and union to send data that a single 
    // value has changed
    union value_changed_union {
        struct {
            int var : 4;
            bool negative : 1;
            int value : 10;
        } value_changed_struct;

        int data = 0;
    };

    int last_x;
    int last_y;
    int last_flip;
    int last_graphics_index;
}




int send_link_value_if_changed(int var, int last_val, int new_val) 
{
    multiplayer::value_changed_union data_to_send;

    if (new_val != last_val) {
        bool is_negative = new_val < 0;
        
        data_to_send.value_changed_struct = {
            var: var, 
            negative: is_negative,
            value: abs(new_val)
        };
        
        #ifdef LOG
        BN_LOG(bn::format<60>("SENT var: {}, negative: {}, value: {}", data_to_send.value_changed_struct.var, data_to_send.value_changed_struct.negative, data_to_send.value_changed_struct.value));
        #endif

        bn::link::send(data_to_send.data);

        return new_val;
    }

    return last_val;     
}



int main()
{
    bn::core::init();
    bn::bg_palettes::set_transparent_color(bn::color(16, 24, 29));

    // World
    const bn::fixed gravity = 0.3;

    // BG and map
    bn::regular_bg_ptr sky = bn::regular_bg_items::sky.create_bg(0, 0);
    bn::regular_bg_ptr tilemap = bn::regular_bg_items::tilemap.create_bg(bn::display::width() / 2, bn::display::height() / 2);
    const bn::regular_bg_map_item& map_item = bn::regular_bg_items::tilemap.map_item();

    // Camera    
    bn::camera_ptr camera = bn::camera_ptr::create(0, 0);
    tilemap.set_camera(camera);
    
    // Jochem player
    Player player(camera, gravity);

    // Other player
    bn::sprite_ptr other_player_sprite = bn::sprite_items::jochem_evil.create_sprite(20, 0);
    other_player_sprite.set_camera(camera);
    bool sent_initial_data;
    int frame = 0;


    // Multiplayer
    int players_counter;
    int current_player_id;
    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::vector<bn::sprite_ptr, 64> info_text_sprites;
    text_generator.set_center_alignment();

    while(true)
    {
        frame++;

        player.update(map_item);
        
        // Smooth cam
        camera_follow_smooth(camera, player.jochem_sprite.position());

        // Move clouds/mountains bg parallax
        sky.set_position(-player.position / bn::fixed(50.0));





        // Multiplayer
        if (frame % 2 == 0) {
            multiplayer::last_x = send_link_value_if_changed(multiplayer::x, multiplayer::last_x, player.jochem_sprite.x().floor_integer());
            multiplayer::last_y = send_link_value_if_changed(multiplayer::y, multiplayer::last_y, player.jochem_sprite.y().floor_integer());
            multiplayer::last_flip = send_link_value_if_changed(multiplayer::flip, multiplayer::last_flip, player.jochem_sprite.horizontal_flip());
        }

        // Send animation updates less frequently?
        if (frame % 4 == 0) {
            multiplayer::last_graphics_index = send_link_value_if_changed(multiplayer::graphics_index, multiplayer::last_graphics_index, player.current_anim_frame);
        }
        
        if(bn::optional<bn::link_state> link_state = bn::link::receive())
        {
            // Update multiplayer info text
            if (players_counter != link_state->player_count() || current_player_id != link_state->current_player_id()) {
                players_counter = link_state->player_count();
                current_player_id = link_state->current_player_id();

                bn::string<200> info_text =  bn::format<60>("players: {}, player id: {}", players_counter, current_player_id);
                info_text_sprites.clear();
                text_generator.generate(0, 65, info_text, info_text_sprites);
            }

            // Apply data received
            const bn::link_player& first_other_player = link_state->other_players().front();
            multiplayer::value_changed_union data_received;
            data_received.data = first_other_player.data();

            #ifdef LOG
            BN_LOG(bn::format<60>("RECEIVED var: {}, negative: {}, value: {}", data_received.value_changed_struct.var, data_received.value_changed_struct.negative, data_received.value_changed_struct.value));
            #endif

            int val = data_received.value_changed_struct.negative ? -data_received.value_changed_struct.value : data_received.value_changed_struct.value;
            
            if (data_received.value_changed_struct.var == multiplayer::x) {
                other_player_sprite.set_x(val);
            }

            if (data_received.value_changed_struct.var == multiplayer::y) {
                other_player_sprite.set_y(val);
            }

            if (data_received.value_changed_struct.var == multiplayer::flip) {
                other_player_sprite.set_horizontal_flip(val);
            }

            if (data_received.value_changed_struct.var == multiplayer::graphics_index) {
                other_player_sprite.set_tiles(bn::sprite_items::jochem_evil.tiles_item().create_tiles(val)); 
            }
        }
        

        bn::core::update();
    }
}
