/**
 * @file collision_handlers.h
 * @brief Simple collision response handlers
 *
 * Contains all collision response logic in straightforward functions.
 * No patterns, no abstractions - just direct game logic.
 */

#ifndef COLLISION_HANDLERS_H
#define COLLISION_HANDLERS_H

#include "game.h"
#include <stdbool.h>

/**
 * @brief Handle popcorn hitting a crab
 * @param game Game state
 * @param popcorn Popcorn projectile
 * @param crab Target crab
 * @return true if collision occurred
 */
bool handle_popcorn_crab_collision(game_ptr game, popcorn_ptr popcorn, crab_ptr crab);

/**
 * @brief Handle popcorn hitting a jellyfish (reflects popcorn)
 * @param game Game state
 * @param popcorn Popcorn projectile
 * @param jellyfish Target jellyfish
 * @return true if collision occurred
 */
bool handle_popcorn_jellyfish_collision(game_ptr game, popcorn_ptr popcorn, jellyfish_ptr jellyfish);

/**
 * @brief Handle reflected popcorn hitting the duck
 * @param game Game state
 * @param popcorn Reflected popcorn
 * @param duck Target duck
 * @return true if collision occurred
 */
bool handle_popcorn_duck_collision(game_ptr game, popcorn_ptr popcorn, duck_ptr duck);

/**
 * @brief Handle falling brick hitting the duck
 * @param game Game state
 * @param brick Falling brick
 * @param duck Target duck
 * @return true if collision occurred
 */
bool handle_brick_duck_collision(game_ptr game, brick_ptr brick, duck_ptr duck);

/**
 * @brief Check if duck would collide with landed bricks at position
 * @param game Game state
 * @param duck_x Duck's x position to check
 * @return true if duck would collide with landed bricks
 */
bool check_duck_brick_landing_collision(game_ptr game, float duck_x);

#endif // COLLISION_HANDLERS_H
