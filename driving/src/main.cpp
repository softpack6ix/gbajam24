#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_bg_palettes.h"
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

#include "bn_sprite_items_van_80.h"
#include "bn_regular_bg_items_grid.h"


#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"



// utils
bn::fixed mod(bn::fixed a, bn::fixed b) 
{
    return (a.modulo(b.floor_integer()) + b).modulo(b.floor_integer());
}

bn::fixed lerp(bn::fixed a, bn::fixed b, bn::fixed t)
{
    return a + t * (b - a);
}


void camera_follow_smooth(bn::camera_ptr cam, bn::fixed_point position) 
{
    cam.set_x(lerp(position.x(), cam.x(), 0.9));
    cam.set_y(lerp(position.y(), cam.y(), 0.9));
}






struct Van 
{
    // fields
    bn::fixed_point position;
    bn::fixed acceleration;
    bn::fixed max_acceleration = 6;
    bn::fixed drag = 0.95;
    bn::fixed direction;

    // sprite
    bn::sprite_ptr sprite = bn::sprite_items::van_80.create_sprite(0, 0);

    // methods
    void update() 
    {
        if (bn::keypad::left_held()) {
            direction-=.01;
        }
        if (bn::keypad::right_held()) {
            direction+=.01;
        }

        if (bn::keypad::a_held() && acceleration < max_acceleration) {
            acceleration += .5;   
        }

        if (bn::keypad::b_held() && acceleration > -max_acceleration) {
            acceleration -= .5;   
        }
        
        bn::pair<bn::fixed, bn::fixed> sin_cos = bn::sin_and_cos(direction - .75);
        position += bn::fixed_point(sin_cos.second, sin_cos.first) * acceleration;
        
        acceleration = acceleration.multiplication(drag);

        if (direction > 1.0) direction = 0.0;
        if (direction < 0.0) direction = 1.0;

        sprite.set_position(position);
        sprite.set_tiles(bn::sprite_items::van_80.tiles_item().create_tiles(bn::min((direction * 64).floor_integer(), 63)));
    }
};





int main()
{
    bn::core::init();

    Van van;


    // bg
    bn::regular_bg_ptr grid_bg = bn::regular_bg_items::grid.create_bg(0,0);


    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(0, 15, 15));
    constexpr bn::string_view info_text_lines[] = {
        "PAD: change sprite's direction",
        ""
    };
    common::info info("Driving demo", info_text_lines, text_generator);


    // Camera    
    bn::camera_ptr camera = bn::camera_ptr::create(0, 0);
    van.sprite.set_camera(camera);
    grid_bg.set_camera(camera);

    while(true)
    {
        van.update();
        info.update();        

        camera_follow_smooth(camera, van.position);
        
        bn::core::update();
    }
}
