/**
 * @file player_controller.h
 * @brief Player input controller for duck movement and shooting
 */

#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <stdbool.h>
#include "game.h"

/**
 * @brief Process player input using engine input system
 * @param game Game state
 * @return true if game should continue, false if quit requested
 */
bool player_process_input(game_ptr game);

#endif // PLAYER_CONTROLLER_H
