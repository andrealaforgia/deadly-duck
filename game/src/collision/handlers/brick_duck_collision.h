/**
 * @file brick_duck_collision.h
 * @brief Brick-Duck collision strategy
 *
 * Handles collisions between falling brick entities and the duck.
 * When collision occurs, duck is killed and brick is deactivated.
 * Only falling (not landed) bricks can harm the duck.
 */

#ifndef BRICK_DUCK_COLLISION_H
#define BRICK_DUCK_COLLISION_H

#include "collision_interface.h"
#include "brick.h"
#include "duck.h"

/**
 * @brief Check collision between falling brick and duck
 * @param brick Brick entity (must be falling)
 * @param duck Duck entity
 * @return Collision result
 */
collision_result_t check_brick_duck_collision(entity_ptr brick, entity_ptr duck);

/**
 * @brief Respond to brick-duck collision
 * @param game Game state
 * @param brick Brick entity
 * @param duck Duck entity
 * @param result Collision result
 */
void respond_brick_duck_collision(game_ptr game, entity_ptr brick, entity_ptr duck, collision_result_t result);

/**
 * @brief Get complete collision strategy for brick-duck
 * @return Collision strategy
 */
collision_strategy_t get_brick_duck_strategy(void);

#endif // BRICK_DUCK_COLLISION_H
