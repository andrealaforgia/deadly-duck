/**
 * @file tribute_stage.h
 * @brief Tribute stage showing credits to original creator
 *
 * Displays scrolling tribute text and cover image before transitioning
 * to the playing stage.
 */

#ifndef GAME_SRC_STAGES_TRIBUTE_STAGE_H_
#define GAME_SRC_STAGES_TRIBUTE_STAGE_H_

#include "types.h"
#include <stdbool.h>

#include "game.h"
#include "stage.h"

/**
 * Tribute stage state
 */
typedef struct {
    game_ptr game;
    float scroll_y;            // Current scroll position
    timestamp_ms_t start_time; // When stage started
    bool waiting_for_space;    // Waiting for user to press space
} tribute_stage_state_t;

typedef tribute_stage_state_t *tribute_stage_state_ptr;

/**
 * Handle tribute stage logic (input, update, render)
 *
 * @param state Tribute stage state
 * @return Stage action (PROGRESS or QUIT)
 */
game_stage_action_t handle_tribute_stage(tribute_stage_state_ptr state);

#endif // GAME_SRC_STAGES_TRIBUTE_STAGE_H_
