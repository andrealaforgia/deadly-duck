/**
 * @file popcorn_jellyfish_collision.h
 * @brief Popcorn-Jellyfish collision strategy
 *
 * Handles collisions between popcorn projectiles and jellyfish entities.
 * When collision occurs, popcorn is reflected downward and continues moving.
 */

#ifndef POPCORN_JELLYFISH_COLLISION_H
#define POPCORN_JELLYFISH_COLLISION_H

#include "collision_interface.h"
#include "projectile.h"
#include "jellyfish.h"

/**
 * @brief Check collision between popcorn and jellyfish
 * @param popcorn Popcorn projectile
 * @param jellyfish Jellyfish entity
 * @return Collision result
 */
collision_result_t check_popcorn_jellyfish_collision(entity_ptr popcorn, entity_ptr jellyfish);

/**
 * @brief Respond to popcorn-jellyfish collision
 * @param game Game state
 * @param popcorn Popcorn projectile
 * @param jellyfish Jellyfish entity
 * @param result Collision result
 */
void respond_popcorn_jellyfish_collision(game_ptr game, entity_ptr popcorn, entity_ptr jellyfish, collision_result_t result);

/**
 * @brief Get complete collision strategy for popcorn-jellyfish
 * @return Collision strategy
 */
collision_strategy_t get_popcorn_jellyfish_strategy(void);

#endif // POPCORN_JELLYFISH_COLLISION_H
