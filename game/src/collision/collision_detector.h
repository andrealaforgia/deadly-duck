/**
 * @file collision_detector.h
 * @brief Collision detection for game entities
 */

#ifndef COLLISION_DETECTOR_H
#define COLLISION_DETECTOR_H

#include <stdbool.h>
#include "game.h"

/**
 * @brief Check and handle collisions between popcorn and jellyfish (reflection)
 * @param game Game state
 */
void check_popcorn_jellyfish_collisions(game_ptr game);

/**
 * @brief Check and handle collisions between popcorn and crabs
 * @param game Game state
 */
void check_popcorn_crab_collisions(game_ptr game);

/**
 * @brief Check and handle collisions between reflected popcorn and duck
 * @param game Game state
 */
void check_reflected_popcorn_duck_collisions(game_ptr game);

/**
 * @brief Check and handle collisions between falling bricks and duck
 * @param game Game state
 */
void check_brick_duck_collisions(game_ptr game);

/**
 * @brief Check if duck collides with any landed bricks at given position
 * @param game Game state
 * @param new_duck_x New X position to test
 * @return true if collision detected, false otherwise
 */
bool check_duck_landed_brick_collision(game_ptr game, float new_duck_x);

/**
 * @brief Process all collision detection for the game
 * @param game Game state
 */
void process_all_collisions(game_ptr game);

#endif // COLLISION_DETECTOR_H
