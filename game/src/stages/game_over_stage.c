/**
 * @file game_over_stage.c
 * @brief Game over stage implementation
 */

#include "game_over_stage.h"

#include <stdlib.h>

#include "constants.h"
#include "bitmap_font.h"
#include "frame.h"
#include "clock.h"
#include "keyboard.h"
#include "events.h"

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
    state->start_time = get_clock_ticks_ms();

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
    // Check for quit events using engine event system
    event_t engine_event = poll_event();
    if (engine_event == QUIT_EVENT) {
        state->game->running = false;
        return;
    }
    
    // Check keyboard state using engine
    keyboard_state_t* keyboard_state = &state->game->keyboard_state;
    
    if (is_esc_key_pressed(keyboard_state)) {
        state->game->running = false;
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

    // Clear screen using engine
    clear_frame(&game->graphics_context);

    // Render GAME OVER text scrolling from bottom to center
    const char* game_over_text = "GAME OVER";

    // Calculate text width to center horizontally
    int text_width = get_bitmap_text_width(&game->font, game_over_text);
    int text_x = (LOGICAL_WIDTH - text_width) / 2;

    // Render in red letters
    render_bitmap_text(&game->font, &game->graphics_context, game_over_text,
                       text_x, (int)state->game_over_y, FONT_COLOR_RED);

    // Present the rendered frame using engine
    render_frame(&game->graphics_context);
}

game_stage_action_t handle_game_over_stage(game_over_stage_state_ptr state) {
    handle_input(state);
    update_scroll(state);
    render_game_over(state);
    return PROGRESS;
}
