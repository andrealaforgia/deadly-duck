/**
 * @file game_renderer.h
 * @brief Game rendering system
 */

#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include "game.h"

/**
 * @brief Render the complete game scene
 * @param game Game state to render
 */
void render_game(game_ptr game);

/**
 * @brief Render the lake gradient at bottom of screen
 * @param game Game state
 */
void render_lake(game_ptr game);

/**
 * @brief Render all game entities (duck, crabs, jellyfish, projectiles, bricks)
 * @param game Game state
 */
void render_entities(game_ptr game);

/**
 * @brief Render the duck sprite
 * @param game Game state
 */
void render_duck(game_ptr game);

/**
 * @brief Render popcorn projectiles
 * @param game Game state
 */
void render_popcorn(game_ptr game);

/**
 * @brief Render crabs
 * @param game Game state
 */
void render_crabs(game_ptr game);

/**
 * @brief Render jellyfish with animation
 * @param game Game state
 */
void render_jellyfish(game_ptr game);

/**
 * @brief Render falling bricks
 * @param game Game state
 */
void render_bricks(game_ptr game);

/**
 * @brief Render UI elements (lives, score)
 * @param game Game state
 */
void render_ui(game_ptr game);

#endif // GAME_RENDERER_H
