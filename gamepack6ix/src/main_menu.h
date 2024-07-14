#pragma once

#include "bn_core.h"
#include "bn_keypad.h"
#include "globals.h"

#include "scene.h"

#include "bn_sprite_items_menu_casette.h"
#include "bn_sprite_items_menu_thuisbezorgd.h"
#include "bn_sprite_items_menu_platforming.h"
#include "bn_sprite_palette_actions.h"

namespace main_menu 
{
    struct menu_item
    {
        next_scene scene;
        bn::sprite_item sprite_item;
        int y_offset = 0;
        int target_x = 0;
        bn::optional<bn::sprite_ptr> spr;
    };

    menu_item menu_items[] = 
    {
        menu_item 
        {
            next_scene::casette,
            bn::sprite_items::menu_casette,
            0
        },
        menu_item 
        {
            next_scene::platforming,
            bn::sprite_items::menu_platforming,
            34
        },
        menu_item 
        {
            next_scene::thuisbezorgd,
            bn::sprite_items::menu_thuisbezorgd,
            68
        },
    };

    bool first_update;


    next_scene run() 
    {
        bn::bg_palettes::set_transparent_color(bn::color(0,0,0));

        int y_offset = -50;
        int x_offset = -70;
        int selected_menu_item = 0;

        // create sprites
        for (menu_item &item : menu_items) {
            item.spr = item.sprite_item.create_sprite(0, y_offset + item.y_offset);
        }

        printer->print("main menu");
        
        bn::optional<bn::sprite_palette_fade_loop_action> fade_action;


        while (true) {
            log_memory_usage();


      
            if (bn::keypad::up_pressed() || bn::keypad::down_pressed() || !first_update) {
                first_update = true;
                bn::sprite_palette_ptr p = menu_items[selected_menu_item].spr->palette();
                // p.set_fade_intensity(0);

                if (bn::keypad::up_pressed())
                    selected_menu_item--;
                if (bn::keypad::down_pressed())
                    selected_menu_item++;

                selected_menu_item = mod(selected_menu_item, 3);
                // for (size_t i = 0; i < 3; i++) {
                //     menu_items[i].spr->set_x(i == selected_menu_item ? 10 : 0);

                //     if (i == selected_menu_item) {
                //         menu_item mi = menu_items[i];
                //         bn::sprite_palette_ptr pal = mi.spr->palette();
                //         fade_action = bn::sprite_palette_fade_loop_action(pal, 30, .5);
                //     }
                // }
            }

            for (size_t i = 0; i < 3; i++) {
                menu_items[i].target_x = i == selected_menu_item ? 10 : 0;
                menu_items[i].spr->set_x(lerp(menu_items[i].target_x + x_offset, menu_items[i].spr->x(), 0.2));
            }


            printer->print(bn::format<20>("{}",selected_menu_item));

            // fade_action->update();

            // if (bn::keypad::left_pressed())
            //     return next_scene::platforming;
            // if (bn::keypad::right_pressed())
            //     return next_scene::casette;
            // if (bn::keypad::up_pressed())
            //     return next_scene::thuisbezorgd;
            

            bn::core::update();
        }

        return next_scene::platforming;
    }
}


