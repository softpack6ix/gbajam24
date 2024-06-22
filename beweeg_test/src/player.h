#include "bn_core.h"

struct player {
    enum mode {
        walk,
        jump,
        attack
    };

    // methods
    player() {
        position = bn::point(0,0);
        velocity = bn::point(0,0);
        current_mode = mode::walk;
    }

    ~player() {}

    // fields
    bn::point position;
    bn::point velocity; // velocity has (x, y) values
    mode current_mode;
};