/**
 * @file popcorn_jellyfish_collision.c
 * @brief Popcorn-Jellyfish collision strategy implementation
 */

#include "popcorn_jellyfish_collision.h"
#include "collision.h"
#include "constants.h"

collision_result_t check_popcorn_jellyfish_collision(entity_ptr popcorn_ptr, entity_ptr jellyfish_ptr) {
    popcorn_ptr popcorn = (popcorn_ptr)popcorn_ptr;
    jellyfish_ptr jellyfish = (jellyfish_ptr)jellyfish_ptr;
    
    if (!popcorn || !jellyfish || !popcorn->active || popcorn->reflected) {
        return no_collision_result();
    }
    
    // AABB collision detection
    bool collision = aabb_collision(popcorn->x, popcorn->y, POPCORN_WIDTH, POPCORN_HEIGHT,
                                   jellyfish->x, jellyfish->y, JELLYFISH_WIDTH, JELLYFISH_HEIGHT);
    
    if (collision) {
        collision_result_t result = {
            .collision_detected = true,
            .entity_a_destroyed = false,
            .entity_b_destroyed = false,
            .entity_a_modified = true,   // Popcorn will be reflected
            .entity_b_modified = false   // Jellyfish unaffected
        };
        return result;
    }
    
    return no_collision_result();
}

void respond_popcorn_jellyfish_collision(game_ptr game, entity_ptr popcorn_ptr, entity_ptr jellyfish_ptr, collision_result_t result) {
    (void)game;       // Not used in this collision response
    (void)jellyfish_ptr; // Jellyfish remains unaffected
    
    if (!result.collision_detected) {
        return;
    }
    
    popcorn_ptr popcorn = (popcorn_ptr)popcorn_ptr;
    
    // Reflect popcorn downward
    popcorn_reflect(popcorn);
}

collision_strategy_t get_popcorn_jellyfish_strategy(void) {
    collision_strategy_t strategy = {
        .check = check_popcorn_jellyfish_collision,
        .respond = respond_popcorn_jellyfish_collision,
        .name = "Popcorn-Jellyfish Collision"
    };
    return strategy;
}
