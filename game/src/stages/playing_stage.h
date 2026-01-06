/**
 * @file playing_stage.h
 * @brief Main gameplay stage
 *
 * Handles all game logic including duck movement, enemy AI, collision detection,
 * scoring, and gameplay rendering.
 */

#ifndef GAME_SRC_STAGES_PLAYING_STAGE_H_
#define GAME_SRC_STAGES_PLAYING_STAGE_H_

#include <stdbool.h>
#include <SDL.h>

#include "game.h"
#include "stage.h"

/**
 * Playing stage state
 */
typedef struct {
    game_ptr game;
} playing_stage_state_t;

typedef playing_stage_state_t* playing_stage_state_ptr;

/**
 * Handle playing stage logic (input, update, render)
 *
 * @param state Playing stage state
 * @return Stage action (PROGRESS or QUIT)
 */
game_stage_action_t handle_playing_stage(playing_stage_state_ptr state);

#endif  // GAME_SRC_STAGES_PLAYING_STAGE_H_
