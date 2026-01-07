/**
 * @file texture_loader.h
 * @brief Texture resource loading and management
 *
 * Handles loading and freeing of texture assets including sprite sheets
 * and image resources with color key support.
 */

#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <stdbool.h>
#include "game.h"
#include "graphics_context.h"

/**
 * @brief Load all texture resources for the game
 * @param game Game state to load textures into
 * @param graphics_context Graphics context for texture loading
 * @return true if all textures loaded successfully, false otherwise
 */
bool load_game_textures(game_ptr game, const graphics_context_ptr graphics_context);

/**
 * @brief Free all texture resources
 * @param game Game state containing textures to free
 */
void free_game_textures(game_ptr game);

#endif // TEXTURE_LOADER_H
