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


#include "../../include/utils.h"
#include "scene.h"
#include "globals.h"


namespace thuisbezorgd 
{
    struct van 
    {
        // fields
        bn::fixed_point position;
        bn::fixed acceleration;
        bn::fixed max_acceleration = 3;
        bn::fixed drag = 0.95;
        bn::fixed direction;

        // sprite
        bn::sprite_ptr sprite = bn::sprite_items::van_80.create_sprite(0, 0);

        // methods
        void update() 
        {
            log_memory_usage();
            
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





    next_scene run()
    {
        van v;

        // bg
        bn::regular_bg_ptr grid_bg = bn::regular_bg_items::grid.create_bg(0,0);


        // Camera    
        bn::camera_ptr camera = bn::camera_ptr::create(0, 0);
        v.sprite.set_camera(camera);
        grid_bg.set_camera(camera);

        while(true)
        {
            v.update();

            camera_follow_smooth(camera, v.position);
            
            bn::core::update();

            if (bn::keypad::start_pressed()) {
                return next_scene::main_menu;
            }
        }
    }
}
