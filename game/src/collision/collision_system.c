/**
 * @file collision_system.c
 * @brief Main collision system orchestration implementation
 */

#include "collision_system.h"
#include "collision_handlers.h"
#include "projectile.h"
#include "crab.h"
#include "jellyfish.h"
#include "duck.h"
#include "brick.h"

static bool system_initialized = false;

bool collision_system_init(game_ptr game) {
    if (!game) {
        return false;
    }
    
    system_initialized = true;
    return true;
}

void collision_system_update(game_ptr game) {
    if (!game || !system_initialized) {
        return;
    }
    
    // Process popcorn collisions with crabs
    for (size_t i = 0; i < game->popcorn_pool.capacity; i++) {
        if (!pool_is_active(&game->popcorn_pool, i)) continue;
        
        popcorn_ptr popcorn = (popcorn_ptr)pool_get_at(&game->popcorn_pool, i);
        if (!popcorn || !popcorn->active) continue;
        
        // Check collision with all crabs
        for (size_t j = 0; j < game->crab_pool.capacity; j++) {
            if (!pool_is_active(&game->crab_pool, j)) continue;
            
            crab_ptr crab = (crab_ptr)pool_get_at(&game->crab_pool, j);
            if (handle_popcorn_crab_collision(game, popcorn, crab)) {
                break; // Popcorn destroyed, no more collisions
            }
        }
        
        // If popcorn is still active and not reflected, check jellyfish
        if (popcorn->active && !popcorn->reflected) {
            for (size_t j = 0; j < game->jellyfish_pool.capacity; j++) {
                if (!pool_is_active(&game->jellyfish_pool, j)) continue;
                
                jellyfish_ptr jellyfish = (jellyfish_ptr)pool_get_at(&game->jellyfish_pool, j);
                if (handle_popcorn_jellyfish_collision(game, popcorn, jellyfish)) {
                    break; // Popcorn reflected, check next popcorn
                }
            }
        }
        
        // If popcorn is reflected, check collision with duck
        if (popcorn->active && popcorn->reflected) {
            handle_popcorn_duck_collision(game, popcorn, &game->duck);
        }
    }
    
    // Process brick collisions with duck
    for (size_t i = 0; i < game->brick_pool.capacity; i++) {
        if (!pool_is_active(&game->brick_pool, i)) continue;
        
        brick_ptr brick = (brick_ptr)pool_get_at(&game->brick_pool, i);
        if (handle_brick_duck_collision(game, brick, &game->duck)) {
            break; // Duck died, no need to check more bricks
        }
    }
}

bool collision_system_check_duck_landing(game_ptr game, float duck_x) {
    if (!system_initialized) {
        return false;
    }
    
    return check_duck_brick_landing_collision(game, duck_x);
}

void collision_system_cleanup(void) {
    system_initialized = false;
}
