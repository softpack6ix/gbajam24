#include <iostream>

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"
#include "bn_bg_palettes.h"

#include "bn_sprite_items_jochem.h"
#include "bn_sprite_items_punch.h"
#include "bn_music_items.h"

#include "bn_sprite_items_rein.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

int main()
{
    bn::core::init();

    bn::music_items::kp6minibeat.play();

    const int width = 120;
    const int height = 80;

    const float gravity = 0.1f;

    // zet text om naar sprite
    constexpr bn::string_view info_text_lines[] = {
        "beweging test"};

    // achtergrond
    bn::bg_palettes::set_transparent_color(bn::color(0, 0, 31));

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    common::info info("0.0.0", info_text_lines, text_generator);

    // maak rein sprite aan
    bn::sprite_ptr rein_sprite = bn::sprite_items::rein.create_sprite(0, 0);
    bn::point rein_position(40, 0);

    // maak jochem sprite aan
    bn::sprite_ptr jochem_sprite = bn::sprite_items::jochem.create_sprite(0, 0);
    bn::sprite_ptr punch_sprite = bn::sprite_items::punch.create_sprite(20, 50);
    bn::point jochem_position(-40, 0);

    // rein animatie
    bn::sprite_animate_action<96> action = bn::create_sprite_animate_action_forever(rein_sprite, 1, bn::sprite_items::rein.tiles_item(),
                                                                                    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                                                                                    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
                                                                                    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
                                                                                    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
                                                                                    64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
                                                                                    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95);

    // jochem
    // idle
    bn::sprite_animate_action<27> idle = bn::create_sprite_animate_action_forever(jochem_sprite, 1, bn::sprite_items::jochem.tiles_item(),
                                                                                  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                                                                                  16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26);

    // walking
    bn::sprite_animate_action<50> walking = bn::create_sprite_animate_action_forever(jochem_sprite, 1, bn::sprite_items::jochem.tiles_item(),
                                                                                     27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
                                                                                     48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
                                                                                     60, 61, 62, 63, 64, 65, 66, 67, 68);

    // kicking
    bn::sprite_animate_action<62> kicking = bn::create_sprite_animate_action_once(jochem_sprite, 1, bn::sprite_items::jochem.tiles_item(),
                                                                                  69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94,
                                                                                  95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116,
                                                                                  117, 118, 118);

    // punch action
    bn::sprite_animate_action<16> punch_action = bn::create_sprite_animate_action_once(punch_sprite, 1, bn::sprite_items::punch.tiles_item(),
                                                                                       0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);

    // init velocity
    float jochem_velocity_x = 0;
    float jochem_velocity_y = 0;

    float rein_velocity_x = 0;
    float rein_velocity_y = 0;

    bool jochem_kicking = false;
    bool jochem_moving = false;
    int kick_timer = 0;
    int kick_offset = jochem_sprite.horizontal_flip() ? -10 : 10;

    while (true)
    {
        // x position
        if (bn::keypad::left_held() && jochem_position.x() > -width)
        {
            jochem_velocity_x = -4.f;
            jochem_sprite.set_horizontal_flip(true);
            jochem_moving = true;
        }
        else if (bn::keypad::right_held() && jochem_position.x() < width)
        {
            jochem_velocity_x = 4.f;
            jochem_sprite.set_horizontal_flip(false);
            jochem_moving = true;
        }
        else
        {
            jochem_velocity_x = 0.f;
            jochem_moving = false;
        }

        // y position
        if (jochem_position.y() > height - 30)
        {
            jochem_velocity_y = 0.f;
        }
        else
        {
            jochem_velocity_y += gravity;
        }

        // jumping
        if (bn::keypad::up_pressed())
        {
            jochem_velocity_y = -4.f;
        }
        else
        {
            jochem_velocity_y += gravity;
        }

        // kicking
        if (bn::keypad::a_pressed())
        {
            jochem_kicking = true;
        } else if (kicking.done()) {
            kicking.reset();
            jochem_kicking = false;
        }

        // jochem update
        if (jochem_kicking)
        {
            jochem_moving = false;
            kicking.update();
        }
        else if (jochem_moving)
        {
            jochem_kicking = false;
            walking.update();
        }
        else
        {
            idle.update();
        }

        // rein
        if (jochem_position.x() - rein_position.x() > 0) {
            rein_velocity_x = 1;
        } else {
            rein_velocity_x = -1;
        }
        // rein_velocity_x = jochem_position.x() - rein_position.x();

        if ( rein_position.y() > height - 30)
        {
            rein_velocity_y = 0.f;
        }
        else
        {
            rein_velocity_y += gravity;
        }

        if (rein_position.x() != jochem_position.x()) {
            rein_position.set_x( rein_position.x() + rein_velocity_x );
        }

        rein_position.set_y( rein_position.y() + rein_velocity_y );

        jochem_position.set_x(jochem_position.x() + jochem_velocity_x);
        jochem_position.set_y(jochem_position.y() + jochem_velocity_y);

        rein_sprite.set_position(rein_position);
        jochem_sprite.set_position(jochem_position);

        info.update();
        action.update();
        bn::core::update();
    }
}
