/**
 * @file stage_manager.c
 * @brief Stage lifecycle and transition management implementation
 */

#include "stage_manager.h"

#include <stdio.h>
#include "tribute_stage.h"
#include "playing_stage.h"
#include "game_over_stage.h"

bool stage_manager_init(stage_manager_ptr manager, game_ptr game) {
    // Create all stages
    manager->tribute_stage = create_tribute_stage_instance();
    manager->playing_stage = create_playing_stage_instance();
    manager->game_over_stage = create_game_over_stage_instance();

    if (!manager->tribute_stage || !manager->playing_stage || !manager->game_over_stage) {
        printf("Failed to create stages\n");
        return false;
    }

    // Initialize with tribute stage
    manager->current_stage = manager->tribute_stage;
    manager->current_stage->init(manager->current_stage, game);

    // Track previous screen for transitions
    manager->previous_screen = game->current_screen;

    return true;
}

game_stage_action_t stage_manager_update(stage_manager_ptr manager, game_ptr game) {
    // Check for screen transitions
    if (game->current_screen != manager->previous_screen) {
        // Cleanup current stage
        if (manager->current_stage->cleanup) {
            manager->current_stage->cleanup(manager->current_stage);
        }

        // Switch to new stage
        manager->previous_screen = game->current_screen;
        switch (game->current_screen) {
            case SCREEN_TRIBUTE:
            case SCREEN_COVER:
                manager->current_stage = manager->tribute_stage;
                break;
            case SCREEN_GAME:
                manager->current_stage = manager->playing_stage;
                break;
            case SCREEN_GAME_OVER:
                manager->current_stage = manager->game_over_stage;
                break;
        }

        // Initialize new stage
        if (manager->current_stage->init) {
            manager->current_stage->init(manager->current_stage, game);
        }
    }

    // Update current stage
    return manager->current_stage->update(manager->current_stage);
}

void stage_manager_cleanup(stage_manager_ptr manager) {
    // Cleanup current stage
    if (manager->current_stage && manager->current_stage->cleanup) {
        manager->current_stage->cleanup(manager->current_stage);
    }

    // Destroy all stages
    destroy_stage(manager->tribute_stage);
    destroy_stage(manager->playing_stage);
    destroy_stage(manager->game_over_stage);
}
