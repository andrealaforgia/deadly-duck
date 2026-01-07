/**
 * @file popcorn_crab_collision.h
 * @brief Popcorn-Crab collision strategy
 *
 * Handles collisions between popcorn projectiles and crab entities.
 * When collision occurs, crab is destroyed, popcorn is deactivated, 
 * sound is played, and score event is published.
 */

#ifndef POPCORN_CRAB_COLLISION_H
#define POPCORN_CRAB_COLLISION_H

#include "collision_interface.h"
#include "projectile.h"
#include "crab.h"

/**
 * @brief Check collision between popcorn and crab
 * @param popcorn Popcorn projectile
 * @param crab Crab entity
 * @return Collision result
 */
collision_result_t check_popcorn_crab_collision(entity_ptr popcorn, entity_ptr crab);

/**
 * @brief Respond to popcorn-crab collision
 * @param game Game state
 * @param popcorn Popcorn projectile
 * @param crab Crab entity
 * @param result Collision result
 */
void respond_popcorn_crab_collision(game_ptr game, entity_ptr popcorn, entity_ptr crab, collision_result_t result);

/**
 * @brief Get complete collision strategy for popcorn-crab
 * @return Collision strategy
 */
collision_strategy_t get_popcorn_crab_strategy(void);

#endif // POPCORN_CRAB_COLLISION_H
