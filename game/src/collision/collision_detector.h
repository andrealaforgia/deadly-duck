/**
 * @file collision_detector.h
 * @brief Collision detection for game entities
 */

#ifndef COLLISION_DETECTOR_H
#define COLLISION_DETECTOR_H

#include <stdbool.h>
#include "game.h"


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
