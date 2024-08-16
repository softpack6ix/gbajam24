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


// Includes
#include "../include/utils.h"
#include "globals.h"
#include "player.h"
#include "pickups.h"


// scenes
#include "scene.h"
#include "platforming_level.h"
#include "splash.h"
#include "main_menu.h"
#include "casette_player.h"
#include "thuisbezorgd.h"



int main()
{
    bn::core::init();

    // The global camera and printer
    camera = bn::camera_ptr::create(0, 0);
    printer = info_printer();

    // Splash
    splash::run();

    // Main menu
    next_scene next = main_menu::run();

    while (true)
    {
        if (next == next_scene::casette) 
            next = casette_player::run();
        else if (next == next_scene::platforming)
            next = platforming_level::run();
        else if (next == next_scene::main_menu)
            next = main_menu::run();
        else if (next == next_scene::thuisbezorgd)
            next = thuisbezorgd::run();
    }
}
