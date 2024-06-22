#include "bn_core.h"

struct player {
    enum mode {
        idle,
        walk,
        jump,
        kick
    };

    // methods
    player() {
        position = bn::point(0,0);
        velocity_x = 0.f;
        velocity_y = 0.f;
        current_mode = mode::idle;
    }

    ~player() {}

    // fields
    bn::point position;
    float velocity_x;
    float velocity_y;
    mode current_mode;
};