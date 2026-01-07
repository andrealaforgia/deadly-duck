/**
 * @file popcorn_duck_collision.h
 * @brief Reflected Popcorn-Duck collision strategy
 *
 * Handles collisions between reflected popcorn projectiles and the duck.
 * When collision occurs, duck is killed and popcorn is deactivated.
 * Only reflected popcorn can harm the duck.
 */

#ifndef POPCORN_DUCK_COLLISION_H
#define POPCORN_DUCK_COLLISION_H

#include "collision_interface.h"
#include "projectile.h"
#include "duck.h"

/**
 * @brief Check collision between reflected popcorn and duck
 * @param popcorn Popcorn projectile (must be reflected)
 * @param duck Duck entity
 * @return Collision result
 */
collision_result_t check_popcorn_duck_collision(entity_ptr popcorn, entity_ptr duck);

/**
 * @brief Respond to reflected popcorn-duck collision
 * @param game Game state
 * @param popcorn Popcorn projectile
 * @param duck Duck entity
 * @param result Collision result
 */
void respond_popcorn_duck_collision(game_ptr game, entity_ptr popcorn, entity_ptr duck, collision_result_t result);

/**
 * @brief Get complete collision strategy for reflected popcorn-duck
 * @return Collision strategy
 */
collision_strategy_t get_popcorn_duck_strategy(void);

#endif // POPCORN_DUCK_COLLISION_H
