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

#include "bn_sprite_items_mannetje.h"
#include "bn_regular_bg_items_bg.h"


#include "../../include/utils.h"

#include "bn_sound_items.h"


struct man {
    bn::fixed_point pos= bn::fixed_point(0,0);
    bn::fixed_point vel= bn::fixed_point(0,0);
    bn::fixed spd = 0.5;
    bn::fixed drg = 0.85;

    bn::fixed nrmspd = 1;
    bn::fixed nrmdrg = 0.85;
    bn::fixed icyspd = 0.5;
    bn::fixed icydrg = 0.99;
    bn::fixed jmpspd = -6;
    bn::fixed gravity = 0.25;
    bool flip = false;
    bool jumping = false;
    bn::sprite_ptr spr = bn::sprite_items::mannetje.create_sprite(pos);
    bn::sprite_animate_action<2> idle = bn::create_sprite_animate_action_forever(spr, 15, bn::sprite_items::mannetje.tiles_item(), 3, 4);
    bn::sprite_animate_action<3> walk = bn::create_sprite_animate_action_forever(spr, 4, bn::sprite_items::mannetje.tiles_item(), 0, 1, 2);
    bn::sprite_animate_action<2> jump = bn::create_sprite_animate_action_forever(spr, 4, bn::sprite_items::mannetje.tiles_item(), 5, 5);
};

int main()
{
    bn::core::init();

    man mannetje;

    auto bg = bn::regular_bg_items::bg.create_bg(0,-30);
    bn::camera_ptr cam = bn::camera_ptr::create(mannetje.pos);

    bg.set_camera(cam);
    mannetje.spr.set_camera(cam);
   // auto idle = bn::create_sprite_animate_action_forever(spr, 30, bn::sprite_items::mannetje.tiles_item(), 3, 4);

    mannetje.spr.set_scale(2);

    mannetje.spr.set_mosaic_enabled(true);
    bg.set_mosaic_enabled(true);
  //  bn::bgs_mosaic::set_stretch(0.7);

    info_printer pr;
    

    bn::color icy = bn::color(10, 10, 31);

    bn::bg_palette_ptr pal = bg.palette();
    pal.set_fade_color(icy);

    while(true)
    {
        if (bn::keypad::b_pressed()) {
            bn::sound_items::ice.play();
        }
        
        if (bn::keypad::b_held()){
            bn::sprites_mosaic::set_stretch(bn::min(bn::fixed(1.0), bn::abs(mannetje.vel.x()/40)));
            pal.set_fade_intensity(bn::min(bn::fixed(1.0), bn::abs(mannetje.vel.x()/40)));
            mannetje.drg=mannetje.icydrg;
            mannetje.spd=mannetje.icyspd;
        }else{
            mannetje.drg=mannetje.nrmdrg;
            mannetje.spd=mannetje.nrmspd;
        }
        cam.set_x(lerp(cam.x(), mannetje.pos.x(), .2));

        mannetje.spr.set_position(mannetje.pos + bn::fixed_point(0, 20));
        mannetje.pos += mannetje.vel;
        mannetje.vel.set_x(mannetje.vel.x() * mannetje.drg);
       
        mannetje.flip = mannetje.vel.x() < 0.0;
        
        mannetje.pos.set_y(bn::min(bn::fixed(0.0), mannetje.pos.y()));
        
        ///WALKING
        if (bn::keypad::right_held()) 
        {
            mannetje.vel.set_x(mannetje.vel.x()+mannetje.spd);
           if(!mannetje.jumping) mannetje.walk.update(); 
        }
        if (bn::keypad::left_held()) 
        {
            mannetje.vel.set_x(mannetje.vel.x()-mannetje.spd);
            if(!mannetje.jumping) mannetje.walk.update(); 
        }
        if (bn::abs(mannetje.vel.x()) < 1) {
            mannetje.idle.update();
        }
        ///JUMPING
        if (mannetje.pos.y() > -0.1&& mannetje.jumping == true) {
            mannetje.vel.set_y(0);
            mannetje.jumping = false;
        }
        if (bn::keypad::a_pressed()&&!mannetje.jumping || bn::keypad::up_pressed()&&!mannetje.jumping) 
        {
            mannetje.vel.set_y(mannetje.vel.y() = mannetje.jmpspd);
            mannetje.jumping = true;   
        }
    
        if (mannetje.jumping == true)
        {
            mannetje.vel.set_y(mannetje.vel.y() += mannetje.gravity);
            mannetje.jump.update();
        }
       
        mannetje.spr.set_horizontal_flip(mannetje.flip);

        pr.print(bn::format<40>("vel: {}, {}", mannetje.vel.x(), mannetje.vel.y()));

        bn::core::update();
    }
}
