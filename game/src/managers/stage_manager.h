/**
 * @file stage_manager.h
 * @brief Stage lifecycle and transition management
 *
 * Manages creation, initialization, transitions, and cleanup of game stages.
 */

#ifndef STAGE_MANAGER_H
#define STAGE_MANAGER_H

#include "game.h"
#include "stage.h"

/**
 * @brief Stage manager context
 */
typedef struct {
    stage_ptr tribute_stage;
    stage_ptr playing_stage;
    stage_ptr game_over_stage;
    stage_ptr current_stage;
    game_screen_t previous_screen;
} stage_manager_t;

typedef stage_manager_t *stage_manager_ptr;

/**
 * @brief Initialize stage manager and create all stages
 * @param manager Stage manager to initialize
 * @param game Game context
 * @return true if initialization successful, false otherwise
 */
bool stage_manager_init(stage_manager_ptr manager, game_ptr game);

/**
 * @brief Update current stage and handle transitions
 * @param manager Stage manager
 * @param game Game context
 * @return Stage action (PROGRESS or QUIT)
 */
game_stage_action_t stage_manager_update(stage_manager_ptr manager, game_ptr game);

/**
 * @brief Clean up stage manager and destroy all stages
 * @param manager Stage manager to clean up
 */
void stage_manager_cleanup(stage_manager_ptr manager);

#endif // STAGE_MANAGER_H
