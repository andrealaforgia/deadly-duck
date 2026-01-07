/**
 * @file collision_interface.c
 * @brief Collision detection interface implementation
 */

#include "collision_interface.h"
#include "collision.h"

bool check_collision(entity_ptr a, entity_ptr b) {
    // Generic collision check - implementation depends on entity types
    // This is a placeholder that would need entity type information
    // to perform actual collision detection
    (void)a;
    (void)b;
    return false;
}

bool apply_collision_strategy(game_ptr game, collision_strategy_t* strategy, entity_ptr a, entity_ptr b) {
    if (!strategy || !strategy->check || !strategy->respond) {
        return false;
    }

    collision_result_t result = strategy->check(a, b);
    
    if (result.collision_detected) {
        strategy->respond(game, a, b, result);
        return true;
    }
    
    return false;
}

collision_result_t no_collision_result(void) {
    collision_result_t result = {
        .collision_detected = false,
        .entity_a_destroyed = false,
        .entity_b_destroyed = false,
        .entity_a_modified = false,
        .entity_b_modified = false
    };
    return result;
}

collision_result_t simple_collision_result(bool collision_occurred) {
    collision_result_t result = {
        .collision_detected = collision_occurred,
        .entity_a_destroyed = false,
        .entity_b_destroyed = false,
        .entity_a_modified = collision_occurred,
        .entity_b_modified = collision_occurred
    };
    return result;
}
