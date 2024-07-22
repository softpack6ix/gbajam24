#include "bn_core.h"
#include "bn_bg_palettes.h"
#include "bn_music.h"
#include "bn_log.h"

#include "splash.h"

#include "bn_music_items.h"


int main()
{
    bn::core::init();
    bn::bg_palettes::set_transparent_color(bn::color(0, 0, 0));
    bn::music_items::splashscreen.play();

    splash::setup();

    while(true)
    {
        if (!splash::is_done) {
            splash::update();
        } else {
            bn::music::stop();
            BN_LOG("DONE!");
        }

        bn::core::update();
    }
}