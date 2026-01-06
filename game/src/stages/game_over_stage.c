/**
 * @file game_over_stage.c
 * @brief Game over stage implementation
 */

#include "game_over_stage.h"

#include <stdlib.h>
#include <SDL.h>

#include "constants.h"
#include "bitmap_font.h"

// Forward declarations for stage callbacks
static void game_over_init(stage_ptr stage, game_ptr game);
static game_stage_action_t game_over_update(stage_ptr stage);
static void game_over_cleanup(stage_ptr stage);

// Helper functions
static void handle_input(game_over_stage_state_ptr state);
static void update_scroll(game_over_stage_state_ptr state);
static void render_game_over(game_over_stage_state_ptr state);

stage_ptr create_game_over_stage_instance(void) {
    stage_ptr stage = (stage_ptr)malloc(sizeof(stage_t));
    if (!stage) return NULL;

    stage->state = NULL;
    stage->init = game_over_init;
    stage->update = game_over_update;
    stage->cleanup = game_over_cleanup;
    stage->name = "GameOver";

    return stage;
}

static void game_over_init(stage_ptr stage, game_ptr game) {
    game_over_stage_state_ptr state = (game_over_stage_state_ptr)malloc(sizeof(game_over_stage_state_t));
    if (!state) return;

    state->game = game;
    state->game_over_y = LOGICAL_HEIGHT;  // Start from bottom of screen
    state->start_time = SDL_GetTicks();

    stage->state = state;
}

static game_stage_action_t game_over_update(stage_ptr stage) {
    game_over_stage_state_ptr state = (game_over_stage_state_ptr)stage->state;

    handle_input(state);
    update_scroll(state);
    render_game_over(state);

    return PROGRESS;
}

static void game_over_cleanup(stage_ptr stage) {
    if (stage->state) {
        free(stage->state);
        stage->state = NULL;
    }
}

static void handle_input(game_over_stage_state_ptr state) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                state->game->running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    state->game->running = false;
                }
                break;
        }
    }
}

static void update_scroll(game_over_stage_state_ptr state) {
    // Scroll GAME OVER text from bottom to center
    const int text_scale = 5;  // Large text
    const int text_height = 7 * text_scale;
    const int target_y = (LOGICAL_HEIGHT - text_height) / 2;  // Center vertically

    if (state->game_over_y > target_y) {
        state->game_over_y -= 200.0f / FPS;  // Scroll up 200 pixels per second
        if (state->game_over_y < target_y) {
            state->game_over_y = target_y;  // Snap to center
        }
    }
}

static void render_game_over(game_over_stage_state_ptr state) {
    game_ptr game = state->game;

    // Draw black background
    SDL_SetRenderDrawColor(game->graphics_context.renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->graphics_context.renderer);

    // Render GAME OVER text scrolling from bottom to center
    const char* game_over_text = "GAME OVER";
    const int text_scale = 5;  // Large text

    // Calculate text width to center horizontally
    int text_width = get_bitmap_text_width_scaled(&game->font, game_over_text, text_scale);
    int text_x = (LOGICAL_WIDTH - text_width) / 2;

    // Render in big red letters
    render_bitmap_text_scaled(&game->font, &game->graphics_context, game_over_text,
                             text_x, (int)state->game_over_y, FONT_COLOR_RED, text_scale);

    // Present the rendered frame
    SDL_RenderPresent(game->graphics_context.renderer);
}

game_stage_action_t handle_game_over_stage(game_over_stage_state_ptr state) {
    handle_input(state);
    update_scroll(state);
    render_game_over(state);
    return PROGRESS;
}
