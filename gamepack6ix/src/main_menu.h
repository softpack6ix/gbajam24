#pragma once

#include "bn_core.h"
#include "bn_keypad.h"
#include "globals.h"



namespace main_menu 
{
    // important
    bool is_done;


    enum class next_scene
    {
        casette,
        platforming
    };


    next_scene run() 
    {
        printer->print("main menu");
        
        while (!is_done) {
            log_memory_usage();


            if (bn::keypad::left_pressed())
                return next_scene::platforming;
            if (bn::keypad::right_pressed())
                return next_scene::casette;
            

            bn::core::update();
        }

        return next_scene::platforming;
    }
}


