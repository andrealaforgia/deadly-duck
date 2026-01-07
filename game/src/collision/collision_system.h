/**
 * @file collision_system.h
 * @brief Main collision system orchestration
 *
 * Simple collision system that replaces the overengineered pattern-heavy
 * implementation with straightforward collision detection and response.
 */

#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "game.h"
#include <stdbool.h>

/**
 * @brief Initialize the collision system
 * @param game Game state
 * @return true if successful
 */
bool collision_system_init(game_ptr game);

/**
 * @brief Process all collisions for the current frame
 * @param game Game state
 */
void collision_system_update(game_ptr game);

/**
 * @brief Check if duck would collide with landed bricks at position
 * @param game Game state
 * @param duck_x Duck's x position to check
 * @return true if duck would collide with landed bricks
 */
bool collision_system_check_duck_landing(game_ptr game, float duck_x);

/**
 * @brief Cleanup the collision system
 */
void collision_system_cleanup(void);

#endif // COLLISION_SYSTEM_H
