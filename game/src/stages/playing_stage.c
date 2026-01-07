/**
 * @file playing_stage.c
 * @brief Main gameplay stage implementation
 */

#include "playing_stage.h"

#include <stdlib.h>
#include <stdio.h>

#include "constants.h"
#include "player_controller.h"
#include "collision_detector.h"
#include "game_renderer.h"
#include "duck.h"
#include "object_pool.h"
#include "projectile.h"
#include "jellyfish.h"
#include "crab.h"
#include "brick.h"

// Forward declarations for stage callbacks
static void playing_init(stage_ptr stage, game_ptr game);
static game_stage_action_t playing_update(stage_ptr stage);
static void playing_cleanup(stage_ptr stage);

// Helper function
static void update_gameplay(playing_stage_state_ptr state);

stage_ptr create_playing_stage_instance(void) {
    stage_ptr stage = (stage_ptr)malloc(sizeof(stage_t));
    if (!stage) return NULL;

    stage->state = NULL;
    stage->init = playing_init;
    stage->update = playing_update;
    stage->cleanup = playing_cleanup;
    stage->name = "Playing";

    return stage;
}

static void playing_init(stage_ptr stage, game_ptr game) {
    playing_stage_state_ptr state = (playing_stage_state_ptr)malloc(sizeof(playing_stage_state_t));
    if (!state) return;

    state->game = game;
    stage->state = state;
}

static game_stage_action_t playing_update(stage_ptr stage) {
    playing_stage_state_ptr state = (playing_stage_state_ptr)stage->state;

    // Handle player input
    if (!player_process_input(state->game)) {
        state->game->running = false;
        return PROGRESS;
    }

    // Update game logic
    update_gameplay(state);

    // Process collisions
    process_all_collisions(state->game);

    // Render the game
    render_game(state->game);

    return PROGRESS;
}

static void playing_cleanup(stage_ptr stage) {
    if (stage->state) {
        free(stage->state);
        stage->state = NULL;
    }
}


static void update_gameplay(playing_stage_state_ptr state) {
    game_ptr game = state->game;
    timestamp_ms_t current_time = get_clock_ticks_ms();

    // Check for game over
    if (game->lives <= 0) {
        game->current_screen = SCREEN_GAME_OVER;
        return;
    }

    // Handle duck respawn after death (2 seconds delay)
    if (game->duck.dead) {
        if (current_time - game->duck.death_time >= 2000) {
            duck_respawn(&game->duck, LOGICAL_WIDTH / 2.0f, LAKE_START_Y - DUCK_HEIGHT);
        }
    }

    // Update duck state (only if alive)
    if (!game->duck.dead) {
        // Let duck_update handle movement and basic boundary checking
        duck_update(&game->duck);
        
        // Additional collision check with landed bricks after movement
        if (check_duck_landed_brick_collision(game, game->duck.x)) {
            // If collision detected, undo the movement
            game->duck.x -= game->duck.vx;
            game->duck.vx = 0;  // Stop duck movement
        }
    }

    // Update projectiles
    popcorn_update_all(&game->popcorn_pool, LOGICAL_HEIGHT);

    // Update jellyfish
    jellyfish_update_all(&game->jellyfish_pool, LOGICAL_WIDTH, current_time);

    // Update crabs
    crabs_update_all(&game->crab_pool, &game->brick_pool,
                     LOGICAL_WIDTH, current_time, (void (*)(void*, int))play_sound, &game->audio_context);

    // Update bricks
    bricks_update_all(&game->brick_pool, LAKE_START_Y, current_time);
}

