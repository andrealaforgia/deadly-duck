/**
 * @file font_loader.h
 * @brief Font resource loading and management
 *
 * Handles loading and freeing of bitmap font resources for text rendering.
 */

#ifndef FONT_LOADER_H
#define FONT_LOADER_H

#include <stdbool.h>
#include "game.h"
#include "graphics_context.h"

/**
 * @brief Load all font resources for the game
 * @param game Game state to load fonts into
 * @param graphics_context Graphics context for font loading
 * @return true if all fonts loaded successfully, false otherwise
 */
bool load_game_fonts(game_ptr game, const graphics_context_ptr graphics_context);

/**
 * @brief Free all font resources
 * @param game Game state containing fonts to free
 */
void free_game_fonts(game_ptr game);

#endif // FONT_LOADER_H
