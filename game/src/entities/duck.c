/**
 * @file duck.c
 * @brief Duck entity implementation
 */

#include "duck.h"

void duck_init(duck_t* duck, float x, float y) {
    duck->x = x;
    duck->y = y;
    duck->vx = 0.0f;
    duck->facing_right = true;
    duck->shooting = false;
    duck->shoot_start_time = 0;
    duck->dead = false;
    duck->death_time = 0;
}

void duck_update(duck_t* duck) {
    if (duck->shooting) {
        Uint32 current_time = SDL_GetTicks();
        if (current_time - duck->shoot_start_time > 100) {
            duck->shooting = false;
        }
    }
}

void duck_respawn(duck_t* duck, float x, float y) {
    duck->x = x;
    duck->y = y;
    duck->vx = 0.0f;
    duck->facing_right = true;
    duck->shooting = false;
    duck->dead = false;
}
