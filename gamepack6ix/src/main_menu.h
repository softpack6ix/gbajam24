#pragma once

#include "bn_core.h"
#include "bn_keypad.h"
#include "globals.h"

#include "scene.h"

#include "bn_sprite_items_menu_casette.h"
#include "bn_sprite_items_menu_steef.h"
#include "bn_sprite_items_menu_skater.h"
#include "bn_sprite_items_menu_thuisbezorgd.h"
#include "bn_sprite_items_menu_platforming.h"
#include "bn_sprite_items_menu_selector.h"
#include "bn_sprite_palette_actions.h"

#include "bn_regular_bg_items_menu_bg.h"

#include "bn_sound_items.h"



namespace main_menu 
{
    struct menu_item
    {
        next_scene scene;
        bn::sprite_item sprite_item;
        int x_offset = 0;
        int y_offset = 0;
        int target_x = 0;
        int target_y = 0;
        bn::optional<bn::sprite_ptr> spr;
    };

    const int menu_item_count = 4;


    const int menu_items_x = 2;
    const int menu_items_y = 2;



    menu_item menu_items[menu_items_x][menu_items_y] = 
    {
        {
            menu_item 
            {
                next_scene::platforming,
                bn::sprite_items::menu_platforming,
                0,
                0
            },
            menu_item 
            {
                next_scene::ollie,
                bn::sprite_items::menu_skater,
                72,
                0
            },
        },
        {
            menu_item 
            {
                next_scene::steef,
                bn::sprite_items::menu_steef,
                0,
                72
            },
            menu_item 
            {
                next_scene::casette,
                bn::sprite_items::menu_casette,
                72,
                72,
            },
        }
    };

    next_scene run() 
    {
        bn::bg_palettes::set_transparent_color(bn::color(0,0,0));

        bn::sound_items::pause.play();

        
        // BG
        bn::regular_bg_ptr bg = bn::regular_bg_items::menu_bg.create_bg(12,45);


        // Pickup
        bn::sprite_ptr twinkle_spr = bn::sprite_items::twinkle.create_sprite(bn::fixed_point(-105, -60));
        bn::sprite_animate_action<60> twinkle_anim = bn::create_sprite_animate_action_forever(twinkle_spr, 1, bn::sprite_items::twinkle.tiles_item(), 
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
            21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 
            41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58
        );

        bn::sprite_ptr spr = bn::sprite_items::lipje_item.create_sprite(bn::fixed_point(-105, -60));
        bn::sprite_animate_action<60> anim = bn::create_sprite_animate_action_forever(spr, 1, bn::sprite_items::lipje_item.tiles_item(), 
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
            21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 
            41, 42, 43, 44, 45, 46, 47, 48, 49, 50
        );
        



        // menu selector
        bn::sprite_ptr menu_selector_spr = bn::sprite_items::menu_selector.create_sprite(0,0);
        menu_selector_spr.set_scale(1.1);

        int menu_selector_target_x = 0;
        int menu_selector_target_y = 0;

        int x_offset = -30;
        int y_offset = -35;
        int selected_menu_item_x = 0;
        int selected_menu_item_y = 0;

        // create sprites
        for (int x = 0; x < menu_items_x; x++) {
            for (int y = 0; y < menu_items_x; y++) {
                menu_item *item = &menu_items[x][y];        
                item->spr = item->sprite_item.create_sprite(x_offset - 64 + item->x_offset, y_offset + item->y_offset);
                item->spr->set_x(lerp(item->target_x + x_offset + item->x_offset, item->spr->x(), 0.2));

            }
        }

        printer->print("");

        while (true) 
        {
            // log_memory_usage();

            anim.update();
            twinkle_anim.update();

            if (bn::keypad::up_pressed() || bn::keypad::down_pressed() || bn::keypad::right_pressed() || bn::keypad::left_pressed()) {
                bn::sound_items::cursor.play();

                if (bn::keypad::up_pressed())
                    selected_menu_item_x--;
                if (bn::keypad::down_pressed())
                    selected_menu_item_x++;
                if (bn::keypad::left_pressed())
                    selected_menu_item_y--;
                if (bn::keypad::right_pressed())
                    selected_menu_item_y++;
                

                selected_menu_item_x = mod(selected_menu_item_x, menu_items_x);
                selected_menu_item_y = mod(selected_menu_item_y, menu_items_y);
            }

            for (int x = 0; x < menu_items_x; x++) {
                for (int y = 0; y < menu_items_x; y++) {
                    menu_item *item = &menu_items[x][y];
                    item->target_y = (selected_menu_item_x == x && selected_menu_item_y == y) ? -3 : 0;
                    item->spr->set_y(lerp(item->target_y + y_offset + item->y_offset, item->spr->y(), 0.2));

                    if (x == selected_menu_item_x && y == selected_menu_item_y) {
                        menu_selector_target_x = item->target_x + x_offset + item->x_offset - 12;
                        menu_selector_target_y = item->target_y + y_offset + item->y_offset;
                    }
                }
            }
            

            menu_selector_spr.set_x(lerp(menu_selector_target_x, menu_selector_spr.x(), 0.6));
            menu_selector_spr.set_y(lerp(menu_selector_target_y, menu_selector_spr.y(), 0.6));


            if (bn::keypad::a_pressed()) {
                bn::sound_items::into.play();
                // bn::sound_items::shield.play();

                

                for (int x = 0; x < menu_items_x; x++) {
                    for (int y = 0; y < menu_items_x; y++) {
                        menu_item *item = &menu_items[x][y];
                        item->spr.reset();
                    }
                }

                return menu_items[selected_menu_item_x][selected_menu_item_y].scene;
            }


            bn::core::update();
        }

        return next_scene::platforming;
    }
}


