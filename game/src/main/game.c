/**
 * @file game.c
 * @brief Core game state and lifecycle implementation
 */

#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "constants.h"
#include "resource_manager.h"
#include "entity_initializer.h"
#include "score.h"
#include "clock.h"
#include "keyboard.h"

bool game_init(game_t* game) {
    // Seed random number generator
    srand((unsigned int)time(NULL));

    // Load all game resources (graphics, audio, fonts)
    if (!load_game_resources(game)) {
        return false;
    }

    // Initialize event system
    game->event_system = create_event_system();

    // Initialize keyboard state
    game->keyboard_state = init_keyboard_state();

    // Initialize game state
    game->running = true;
    game->current_screen = SCREEN_TRIBUTE;
    game->current_stage = NULL;  // Will be set when stage system is implemented
    game->scroll_y = LOGICAL_HEIGHT;  // Start from bottom of screen
    game->tribute_start_time = get_clock_ticks_ms();
    game->tribute_waiting = true;  // Wait for space key before scrolling

    // Initialize all game entities
    initialize_all_entities(game);

    // Subscribe to game events for scoring
    subscribe_score_events(game);

    // Initialize game statistics
    game->lives = 3;
    game->score = 0;

    return true;
}

void game_terminate(game_t* game) {
    // Clean up entity pools
    cleanup_all_entities(game);
    
    // Free all game resources
    free_game_resources(game);
}
