/**
 * @file duck_landed_brick_collision.c
 * @brief Duck-Landed Brick collision strategy implementation
 */

#include "duck_landed_brick_collision.h"
#include "collision.h"
#include "constants.h"
#include "brick.h"
#include "geometry.h"

collision_result_t check_duck_position_brick_collision(entity_ptr a, entity_ptr b) {
    point_ptr duck_pos = (point_ptr)a;
    brick_ptr brick = (brick_ptr)b;
    
    // Only check landed bricks
    if (!duck_pos || !brick || !brick->landed) {
        return no_collision_result();
    }
    
    // AABB collision detection with duck at new position
    bool collision = aabb_collision(duck_pos->x, duck_pos->y, DUCK_WIDTH, DUCK_HEIGHT,
                                   brick->x, brick->y, BRICK_WIDTH, BRICK_HEIGHT);
    
    if (collision) {
        // This is a blocking collision - no entities destroyed, just movement prevented
        collision_result_t result = {
            .collision_detected = true,
            .entity_a_destroyed = false,  // Duck not destroyed
            .entity_b_destroyed = false,  // Brick not destroyed
            .entity_a_modified = false,   // Duck position not changed (move blocked)
            .entity_b_modified = false    // Brick unaffected
        };
        return result;
    }
    
    return no_collision_result();
}

void respond_duck_position_brick_collision(game_ptr game, entity_ptr duck_position, entity_ptr brick, collision_result_t result) {
    // No response needed - this collision is used purely for movement blocking
    // The calling code will handle preventing the movement based on collision result
    (void)game;
    (void)duck_position;
    (void)brick;
    (void)result;
}

collision_strategy_t get_duck_position_brick_strategy(void) {
    collision_strategy_t strategy = {
        .check = check_duck_position_brick_collision,
        .respond = respond_duck_position_brick_collision,
        .name = "Duck-Landed Brick Movement Blocking"
    };
    return strategy;
}
