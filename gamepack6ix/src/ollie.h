#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_bg_palettes.h"
#include "bn_bg_palette_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"
#include "bn_regular_bg_builder.h"
#include "bn_sprite_text_generator.h"
#include "bn_regular_bg_animate_actions.h"
#include "bn_point.h"
#include "bn_log.h"
#include "bn_format.h"
#include "bn_math.h"
#include "bn_display.h"
#include "bn_camera_ptr.h"
#include "bn_sprites_mosaic.h"
#include "bn_bgs_mosaic.h"
#include "bn_random.h"

#include "bn_sprite_items_fruitvliegje.h"
#include "bn_sprite_items_skateboard.h"

#include "../../include/utils.h"
#include "../../common/include/common_variable_16x16_sprite_font.h"
#include <iostream>


#include "./scene.h"

using namespace bn;




namespace ollie 
{
    fixed grav = 1;
    fixed_point vel;
    bool jumping = false;
    bool kickflipping = false;
    bool shoviting = false;
    bool tricking = false;
    bool braking = false;
    bool pushing = false;
    bool crouching = false;
    bool dead = false;

    fixed jump_angle = 30.0;
    int rand_face = 19;
    int base_y = 24;
    fixed jump_vel = 8.0;

    // void jump(sprite_ptr ollie)
    // {
    // if (keypad::up_pressed()) {
    //             ollie.set_y(ollie.x() + grav);
    //             while()
    //             grav
    //         }
    // }


    optional<sprite_ptr> skateboard;
    optional<sprite_ptr> ollie;

    void set_ollie_x(fixed x)
    {
        ollie->set_x(x);
        // set the skateboard x as well
        skateboard->set_x(x);
    }

    void set_ollie_y(fixed y)
    {
        ollie->set_y(y);
        // set the skateboard y as well
        skateboard->set_y(y + 12);
    }


    vector<string<20>, 20> tricks;


    next_scene run()
    {

        bg_palettes::set_transparent_color(bn::color(20, 20, 20));

        skateboard = sprite_items::skateboard.create_sprite(0,48);
        ollie =  sprite_items::fruitvliegje.create_sprite(0,base_y);

        sprite_animate_action kickflip_anim = create_sprite_animate_action_once(*skateboard, 2, sprite_items::skateboard.tiles_item(), 
            0, 1, 2, 4);

        sprite_animate_action shovit_anim = create_sprite_animate_action_once(*skateboard, 2, sprite_items::skateboard.tiles_item(), 
            5, 6, 7, 8, 0);

        sprite_animate_action<4> idle = create_sprite_animate_action_forever(
            *ollie, 12, sprite_items::fruitvliegje.tiles_item(), 2, 3, 4, 5);

        sprite_animate_action<3> brake_anim = create_sprite_animate_action_forever(
        *ollie, 6, sprite_items::fruitvliegje.tiles_item(), 16, 17, 18);

        sprite_animate_action<7> pushing_anim = create_sprite_animate_action_forever(
        *ollie, 12, sprite_items::fruitvliegje.tiles_item(), 9, 10, 11, 12, 13, 14, 15);

        sprite_animate_action<7> crouching_anim = create_sprite_animate_action_forever(
        *ollie, 5, sprite_items::fruitvliegje.tiles_item(), 6, 7, 8);

        random rrrr;

        info_printer pr;

        while(true)
        {   
            if (keypad::start_pressed()) {
                ollie.reset();
                skateboard.reset();
                return next_scene::main_menu;
            }


            if(!jumping && !dead && !braking && !pushing && !crouching) {
                idle.update();
            }

            if (keypad::left_held() && !dead && ollie->x() > -display::width() / 2) {
                set_ollie_x(ollie->x() - 3);
                braking = true;
                if(!crouching){brake_anim.update();}
            }
            else {
                braking = false;
            }

            if (keypad::right_held() && !dead && ollie->x() < display::width() / 2) {
                set_ollie_x(ollie->x() + 3);
                pushing = true;
                if(!crouching){pushing_anim.update();} 
            }
            else {
                pushing = false;
            }

            if (keypad::a_held() && !jumping) {
                jump_vel += 0.2;
                crouching = true;
                crouching_anim.update();
                if (jump_vel > 15.0) {
                    jump_vel = 15.0;
                }
            }

            // pr.print(format<20>("{}", jump_vel));

            if (keypad::a_released() && !jumping) {
                if (dead) {
                    dead = false;
                    // Set the skateboard to the first graphics frame
                    skateboard->set_tiles(sprite_items::skateboard.tiles_item().create_tiles(0));
                } else {
                    vel.set_y(-jump_vel); // Apply the jump velocity
                    jumping = true; // Set jumping to true to prevent further jump_vel increments
                    crouching = false;
                    jump_angle = rrrr.get_fixed(40.0) + -20;
                    rand_face += rrrr.get_int(4);
                }
                jump_vel = 8.0; // Reset jump_vel for the next jump
            }

            if (keypad::left_held() && keypad::b_pressed() && jumping) {
                kickflipping = true;
                shoviting = false;
                tricking = true;
                kickflip_anim.reset();

                tricks.push_back("kickflip");
            }

            if (keypad::down_held() && keypad::b_pressed() && jumping) {
                shoviting = true;
                kickflipping = false;
                tricking = true;
                shovit_anim.reset();

                tricks.push_back("shovit");
            }
            
            string<60> tricks_text;

            for (const auto& trick : tricks) {
                tricks_text.append(trick);
                tricks_text.append(" ");
            }

            pr.print(tricks_text);

            for (auto& spr : pr.info_text_sprites) {
                spr.set_bg_priority(0);
            }


            // make the skateboard bigger and in the foreground when flipping
            if (tricking) {
                skateboard->set_scale(1.3);
                skateboard->set_bg_priority(0);
            } else {
                skateboard->set_scale(1);
                skateboard->set_bg_priority(1);
            }

            if (kickflipping) {
                if (kickflip_anim.done()) {
                    kickflipping = false;
                    tricking = false;
                } else {
                    // don't update when dead ok ?
                    if (!dead) kickflip_anim.update();
                }
            }

            if (shoviting) {
                if (shovit_anim.done()) {
                    shoviting = false;
                    tricking = false;
                } else {
                    if (!dead) shovit_anim.update();
                }
            }

            // apply velocity and if 
            // if (ollie.y() < 49 && jumping) {
            set_ollie_y(ollie->y() + vel.y());
            if (jumping)  {
                vel.set_y(vel.y() + grav);
            }

            
            // landing
            if (ollie->y() >= base_y && jumping == true) {
                if (tricking) {
                    dead = true;
                    // set the tricks to false
                    kickflipping = false;
                    shoviting = false;
                    tricking = false;
                }
                vel.set_y(0);
                jumping = false;
                rand_face = 19;
                tricks.clear();
            }

            if (dead) {
                bg_palettes::set_transparent_color(lerp(*bg_palettes::transparent_color(), color(0, 0, 0), 0.1));
                ollie->set_tiles(sprite_items::fruitvliegje.tiles_item().create_tiles(1));
            } else {
                bg_palettes::set_transparent_color(lerp(*bg_palettes::transparent_color(), color(27, 27, 27), 0.1));
                if (braking){
                    brake_anim.update();
                    }
                else if (pushing){
                    pushing_anim.update();
                    }
                else if (crouching){
                    crouching_anim.update();
                    }
                else{
                    idle.update();
                }
                
            }

            if (jumping) {
                ollie->set_tiles(sprite_items::fruitvliegje.tiles_item().create_tiles(rand_face));
            } else {
                if (braking){
                    brake_anim.update();
                    }
                else if (pushing){
                    pushing_anim.update();
                    }
                else if (crouching){
                    crouching_anim.update();
                    }
                else if (!dead){
                    idle.update();
                }
            }

            ollie->set_rotation_angle(jumping ? mod((int)rrrr.get_fixed(5.0) + (int) jump_angle, 360) : 0);

            core::update();
        }
    }

}
