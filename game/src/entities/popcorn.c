/**
 * @file popcorn.c
 * @brief Popcorn implementation
 */

#include "popcorn.h"

bool popcorn_spawn(object_pool_t *pool, float x, float y) {
    size_t index;
    popcorn_ptr popcorn = (popcorn_ptr)pool_acquire(pool, &index);
    if (!popcorn) {
        return false; // Pool is full
    }

    popcorn->active = true;
    popcorn->reflected = false;
    popcorn->x = x;
    popcorn->y = y;
    popcorn->vy = -POPCORN_SPEED; // Move upward
    return true;
}

void popcorn_update_all(object_pool_t *pool, int logical_height) {
    // Manual iteration since we need to potentially release objects
    for (size_t i = 0; i < pool->capacity; i++) {
        if (!pool_is_active(pool, i))
            continue;

        popcorn_ptr popcorn = (popcorn_ptr)pool_get_at(pool, i);
        if (!popcorn || !popcorn->active)
            continue;

        // Move popcorn (can be upward or downward if reflected)
        popcorn->y += popcorn->vy;

        // Deactivate if off screen (top or bottom)
        if (popcorn->y < 0 || popcorn->y > logical_height) {
            popcorn->active = false;
            pool_release(pool, i);
        }
    }
}

void popcorn_reflect(popcorn_ptr popcorn) {
    popcorn->vy = -popcorn->vy; // Reverse direction
    popcorn->reflected = true;
}
