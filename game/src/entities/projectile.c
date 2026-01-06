/**
 * @file projectile.c
 * @brief Popcorn projectile implementation
 */

#include "projectile.h"

void popcorn_pool_init(popcorn_t* popcorn, int count) {
    for (int i = 0; i < count; i++) {
        popcorn[i].active = false;
        popcorn[i].reflected = false;
        popcorn[i].x = 0.0f;
        popcorn[i].y = 0.0f;
        popcorn[i].vy = 0.0f;
    }
}

bool popcorn_spawn(popcorn_t* popcorn, int count, float x, float y) {
    for (int i = 0; i < count; i++) {
        if (!popcorn[i].active) {
            popcorn[i].active = true;
            popcorn[i].reflected = false;
            popcorn[i].x = x;
            popcorn[i].y = y;
            popcorn[i].vy = -POPCORN_SPEED;  // Move upward
            return true;
        }
    }
    return false;  // Pool is full
}

void popcorn_update_all(popcorn_t* popcorn, int count, int logical_height) {
    for (int i = 0; i < count; i++) {
        if (popcorn[i].active) {
            // Move popcorn (can be upward or downward if reflected)
            popcorn[i].y += popcorn[i].vy;

            // Deactivate if off screen (top or bottom)
            if (popcorn[i].y < 0 || popcorn[i].y > logical_height) {
                popcorn[i].active = false;
            }
        }
    }
}

void popcorn_reflect(popcorn_t* projectile) {
    projectile->vy = -projectile->vy;  // Reverse direction
    projectile->reflected = true;
}
