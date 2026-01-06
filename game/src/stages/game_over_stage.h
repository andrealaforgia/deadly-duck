/**
 * @file game_over_stage.h
 * @brief Game over stage showing final score
 *
 * Displays "GAME OVER" text scrolling up to center of screen.
 */

#ifndef GAME_SRC_STAGES_GAME_OVER_STAGE_H_
#define GAME_SRC_STAGES_GAME_OVER_STAGE_H_

#include <stdbool.h>
#include "types.h"

#include "game.h"
#include "stage.h"

/**
 * Game over stage state
 */
typedef struct {
    game_ptr game;
    float game_over_y;    // Y position of GAME OVER text
    timestamp_ms_t start_time;    // When stage started
} game_over_stage_state_t;

typedef game_over_stage_state_t* game_over_stage_state_ptr;

/**
 * Handle game over stage logic (input, update, render)
 *
 * @param state Game over stage state
 * @return Stage action (PROGRESS or QUIT)
 */
game_stage_action_t handle_game_over_stage(game_over_stage_state_ptr state);

#endif  // GAME_SRC_STAGES_GAME_OVER_STAGE_H_
