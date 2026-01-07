/**
 * @file stage_director.c
 * @brief Dynamic stage lifecycle and transition management implementation
 */

#include "stage_director.h"

#include <stdio.h>
#include <string.h>
#include "tribute_stage.h"
#include "playing_stage.h"
#include "game_over_stage.h"

static stage_ptr find_stage_for_screen(stage_director_ptr director, game_screen_t screen_type) {
    for (size_t i = 0; i < director->stage_count; i++) {
        if (director->stages[i].screen_type == screen_type) {
            return director->stages[i].instance;
        }
    }
    return NULL;
}

static bool create_stage_instance(stage_director_ptr director, game_screen_t screen_type) {
    for (size_t i = 0; i < director->stage_count; i++) {
        if (director->stages[i].screen_type == screen_type && !director->stages[i].instance) {
            director->stages[i].instance = director->stages[i].create_stage_fn();
            return director->stages[i].instance != NULL;
        }
    }
    return false;
}

bool stage_director_init(stage_director_ptr director, game_ptr game) {
    // Initialize director state
    memset(director, 0, sizeof(stage_director_t));
    
    // Register all available stages
    if (!stage_director_register_stage(director, SCREEN_TRIBUTE, create_tribute_stage_instance) ||
        !stage_director_register_stage(director, SCREEN_COVER, create_tribute_stage_instance) ||
        !stage_director_register_stage(director, SCREEN_GAME, create_playing_stage_instance) ||
        !stage_director_register_stage(director, SCREEN_GAME_OVER, create_game_over_stage_instance)) {
        printf("Failed to register stages\n");
        return false;
    }

    // Create initial stage instance
    if (!create_stage_instance(director, game->current_screen)) {
        printf("Failed to create initial stage for screen %d\n", game->current_screen);
        return false;
    }

    // Set current stage
    director->current_stage = find_stage_for_screen(director, game->current_screen);
    if (!director->current_stage) {
        printf("Failed to find initial stage\n");
        return false;
    }

    // Initialize current stage
    if (director->current_stage->init) {
        director->current_stage->init(director->current_stage, game);
    }

    // Track previous screen for transitions
    director->previous_screen = game->current_screen;

    return true;
}

bool stage_director_register_stage(stage_director_ptr director, 
                                   game_screen_t screen_type,
                                   stage_ptr (*create_stage_fn)(void)) {
    if (director->stage_count >= MAX_STAGES) {
        printf("Cannot register more stages, maximum reached\n");
        return false;
    }

    director->stages[director->stage_count].screen_type = screen_type;
    director->stages[director->stage_count].create_stage_fn = create_stage_fn;
    director->stages[director->stage_count].instance = NULL;
    director->stage_count++;

    return true;
}

game_stage_action_t stage_director_update(stage_director_ptr director, game_ptr game) {
    // Check for screen transitions
    if (game->current_screen != director->previous_screen) {
        // Cleanup current stage
        if (director->current_stage && director->current_stage->cleanup) {
            director->current_stage->cleanup(director->current_stage);
        }

        // Create new stage instance if needed
        stage_ptr new_stage = find_stage_for_screen(director, game->current_screen);
        if (!new_stage) {
            if (!create_stage_instance(director, game->current_screen)) {
                printf("Failed to create stage for screen %d\n", game->current_screen);
                return QUIT;
            }
            new_stage = find_stage_for_screen(director, game->current_screen);
        }

        if (!new_stage) {
            printf("No stage registered for screen %d\n", game->current_screen);
            return QUIT;
        }

        // Switch to new stage
        director->current_stage = new_stage;
        director->previous_screen = game->current_screen;

        // Initialize new stage
        if (director->current_stage->init) {
            director->current_stage->init(director->current_stage, game);
        }
    }

    // Update current stage
    if (director->current_stage && director->current_stage->update) {
        return director->current_stage->update(director->current_stage);
    }

    return PROGRESS;
}

void stage_director_cleanup(stage_director_ptr director) {
    // Cleanup current stage
    if (director->current_stage && director->current_stage->cleanup) {
        director->current_stage->cleanup(director->current_stage);
    }

    // Destroy all stage instances
    for (size_t i = 0; i < director->stage_count; i++) {
        if (director->stages[i].instance) {
            destroy_stage(director->stages[i].instance);
            director->stages[i].instance = NULL;
        }
    }

    director->stage_count = 0;
    director->current_stage = NULL;
}
