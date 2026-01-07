/**
 * @file duck_landed_brick_collision.h
 * @brief Duck-Landed Brick collision strategy
 *
 * Handles collision detection between duck and landed bricks for movement blocking.
 * This is used to prevent duck from moving into positions occupied by landed bricks.
 * This is a pure collision detection strategy without response actions.
 */

#ifndef DUCK_LANDED_BRICK_COLLISION_H
#define DUCK_LANDED_BRICK_COLLISION_H

#include "collision_interface.h"
#include "duck.h"
#include "brick.h"

/**
 * @brief Check if duck at new position would collide with landed brick
 * @param duck_position Duck position data (x,y coordinates)
 * @param brick Brick entity (must be landed)
 * @return Collision result
 */
collision_result_t check_duck_landed_brick_collision(entity_ptr duck_position, entity_ptr brick);

/**
 * @brief No response needed for movement blocking collision
 * @param game Game state (unused)
 * @param duck_position Duck position (unused)
 * @param brick Brick entity (unused)
 * @param result Collision result (unused)
 */
void respond_duck_landed_brick_collision(game_ptr game, entity_ptr duck_position, entity_ptr brick, collision_result_t result);

/**
 * @brief Get complete collision strategy for duck-landed brick
 * @return Collision strategy
 */
collision_strategy_t get_duck_landed_brick_strategy(void);

/**
 * @brief Simple position structure for duck position collision checks
 */
typedef struct {
    float x;
    float y;
} duck_position_t;

#endif // DUCK_LANDED_BRICK_COLLISION_H
