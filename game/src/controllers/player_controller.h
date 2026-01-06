/**
 * @file player_controller.h
 * @brief Player input controller for duck movement and shooting
 */

#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <stdbool.h>
#include <SDL.h>
#include "game.h"

/**
 * @brief Handle SDL events for player input
 * @param event SDL event to process
 * @param game Game state containing duck and projectiles
 * @return true if game should continue, false if quit requested
 */
bool player_handle_event(SDL_Event* event, game_ptr game);

/**
 * @brief Process all pending SDL events for player input
 * @param game Game state
 * @return true if game should continue, false if quit requested
 */
bool player_process_input(game_ptr game);

#endif // PLAYER_CONTROLLER_H
