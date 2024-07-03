#include "bn_core.h"
#include "bn_bg_palettes.h"
#include "bn_music.h"

#include "splash.h"

#include "bn_music_items.h"



int main()
{
    bn::core::init();

    bn::bg_palettes::set_transparent_color(bn::color(0, 0, 0));
    bn::music_items::splashscreen.play();

    Splash::setup();
    
    while(true)
    {
        bool is_done = Splash::update();
        if (is_done) {
            bn::music::stop();
        }
        
        bn::core::update();
    }
}