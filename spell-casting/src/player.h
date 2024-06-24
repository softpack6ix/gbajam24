
#include "bn_core.h"
#include "bn_point.h"
#include "bn_fixed_point.h"


struct player {
    enum animation {
        idle,
        walk,
        jump,
        kick,
        charge_spell
    };

    // methods
    player() {
        position = bn::fixed_point(0,0);
        velocity = bn::fixed_point(0,0);
        current_anim = animation::idle;
    }

    ~player() {}


    // fields
    bn::fixed_point position;
    bn::fixed_point velocity;
    animation current_anim;
};