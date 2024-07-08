#include "bn_core.h"
#include "bn_bg_palettes.h"
#include "bn_music.h"
#include "bn_log.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_map_ptr.h"

#include "splash.h"

#include "bn_music_items.h"
#include "bn_regular_bg_items_zwembad.h"



int main()
{
    bn::core::init();
    bn::bg_palettes::set_transparent_color(bn::color(0, 0, 0));
    bn::music_items::splashscreen.play();

    splash::setup();

    // bn::regular_bg_ptr zwembad = bn::regular_bg_items::zwembad.create_bg(8,50);

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