/**
 * @file audio_loader.h
 * @brief Audio resource loading and management
 *
 * Handles initialization of audio context and loading of sound effects
 * for the game.
 */

#ifndef AUDIO_LOADER_H
#define AUDIO_LOADER_H

#include <stdbool.h>
#include "game.h"

/**
 * @brief Load all audio resources for the game
 * @param game Game state to load audio into
 * @return true if all audio loaded successfully, false otherwise
 */
bool load_game_audio(game_ptr game);

/**
 * @brief Free all audio resources
 * @param game Game state containing audio to free
 */
void free_game_audio(game_ptr game);

#endif // AUDIO_LOADER_H
