/**
 * @file tribute_stage.c
 * @brief Tribute stage implementation
 */

#include "tribute_stage.h"

#include <stdlib.h>
#include <string.h>
#include <SDL.h>

#include "constants.h"
#include "bitmap_font.h"

// Tribute text lines
static const char* TRIBUTE_LINES[] = {
    "THIS IS A TRIBUTE",
    "TO ED HODAPP",
    "WHO ORIGINALLY WROTE",
    "DEADLY DUCK IN 1982",
    "FOR SIRIUS SOFTWARE"
};
static const int NUM_TRIBUTE_LINES = 5;

// Forward declarations for stage callbacks
static void tribute_init(stage_ptr stage, game_ptr game);
static game_stage_action_t tribute_update(stage_ptr stage);
static void tribute_cleanup(stage_ptr stage);

// Helper functions
static void handle_input(tribute_stage_state_ptr state);
static void update_scroll(tribute_stage_state_ptr state);
static void render_tribute(tribute_stage_state_ptr state);

stage_ptr create_tribute_stage_instance(void) {
    stage_ptr stage = (stage_ptr)malloc(sizeof(stage_t));
    if (!stage) return NULL;

    stage->state = NULL;
    stage->init = tribute_init;
    stage->update = tribute_update;
    stage->cleanup = tribute_cleanup;
    stage->name = "Tribute";

    return stage;
}

static void tribute_init(stage_ptr stage, game_ptr game) {
    tribute_stage_state_ptr state = (tribute_stage_state_ptr)malloc(sizeof(tribute_stage_state_t));
    if (!state) return;

    state->game = game;
    state->scroll_y = LOGICAL_HEIGHT;  // Start from bottom of screen
    state->start_time = SDL_GetTicks();
    state->waiting_for_space = true;  // Wait for space key before scrolling

    stage->state = state;
}

static game_stage_action_t tribute_update(stage_ptr stage) {
    tribute_stage_state_ptr state = (tribute_stage_state_ptr)stage->state;

    handle_input(state);
    update_scroll(state);
    render_tribute(state);

    return PROGRESS;
}

static void tribute_cleanup(stage_ptr stage) {
    if (stage->state) {
        free(stage->state);
        stage->state = NULL;
    }
}

static void handle_input(tribute_stage_state_ptr state) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                state->game->running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    state->game->running = false;
                } else if (event.key.keysym.sym == SDLK_SPACE) {
                    if (state->waiting_for_space) {
                        // Start scrolling
                        state->waiting_for_space = false;
                        state->start_time = SDL_GetTicks();
                    } else {
                        // Skip to game
                        state->game->current_screen = SCREEN_GAME;
                    }
                }
                break;
        }
    }
}

static void update_scroll(tribute_stage_state_ptr state) {
    // Only scroll if not waiting for space key
    if (!state->waiting_for_space) {
        // Scroll upward (100 pixels per second)
        state->scroll_y -= 100.0f / FPS;
    }

    // After cover image scrolls completely off screen, move to game screen
    // Calculate text height dynamically based on scale
    int estimated_scale = 3;  // Rough estimate
    int text_height = NUM_TRIBUTE_LINES * (7 * estimated_scale + 5);
    // Cover starts 50 pixels after text
    float cover_start_y = state->scroll_y + text_height + 50;
    // Cover is scaled to 90% of screen width, calculate height maintaining aspect ratio
    int target_cover_width = (int)(LOGICAL_WIDTH * 0.9f);
    float aspect_ratio = (float)state->game->cover_height / (float)state->game->cover_width;
    float cover_scaled_height = target_cover_width * aspect_ratio;

    // If bottom of cover has scrolled off top of screen, transition to game
    if (cover_start_y + cover_scaled_height < 0) {
        state->game->current_screen = SCREEN_GAME;
    }
}

static void render_tribute(tribute_stage_state_ptr state) {
    game_ptr game = state->game;

    // Clear screen with black
    SDL_SetRenderDrawColor(game->graphics_context.renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->graphics_context.renderer);

    // Find the longest line to calculate scale
    const char* longest_line = TRIBUTE_LINES[0];
    int max_len = strlen(TRIBUTE_LINES[0]);
    for (int i = 1; i < NUM_TRIBUTE_LINES; i++) {
        int len = strlen(TRIBUTE_LINES[i]);
        if (len > max_len) {
            max_len = len;
            longest_line = TRIBUTE_LINES[i];
        }
    }

    // Calculate scale so longest line takes 90% of screen width
    int target_width = (int)(LOGICAL_WIDTH * 0.9f);
    int unscaled_width = get_bitmap_text_width(&game->font, longest_line);
    int scale = target_width / unscaled_width;
    if (scale < 1) scale = 1;  // Minimum scale of 1

    int line_height = 7 * scale + 5;  // Character height * scale + spacing

    // Render each line centered
    int current_y = (int)state->scroll_y;
    for (int i = 0; i < NUM_TRIBUTE_LINES; i++) {
        if (i == 1) {
            // Line 2: "TO ED HODAPP" - render "TO " in yellow, "ED HODAPP" in pink
            const char* to_text = "TO ";
            const char* name_text = "ED HODAPP";

            // Calculate widths
            int to_width = get_bitmap_text_width_scaled(&game->font, to_text, scale);
            int name_width = get_bitmap_text_width_scaled(&game->font, name_text, scale);
            int total_width = to_width + name_width;

            // Center the whole line
            int line_x = (LOGICAL_WIDTH - total_width) / 2;

            // Render "TO " in yellow
            render_bitmap_text_scaled(&game->font, &game->graphics_context, to_text,
                                     line_x, current_y, FONT_COLOR_YELLOW, scale);

            // Render "ED HODAPP" in pink
            render_bitmap_text_scaled(&game->font, &game->graphics_context, name_text,
                                     line_x + to_width, current_y, FONT_COLOR_PINK, scale);
        } else {
            // All other lines in yellow
            int text_width = get_bitmap_text_width_scaled(&game->font, TRIBUTE_LINES[i], scale);
            int text_x = (LOGICAL_WIDTH - text_width) / 2;

            render_bitmap_text_scaled(&game->font, &game->graphics_context, TRIBUTE_LINES[i],
                                     text_x, current_y, FONT_COLOR_YELLOW, scale);
        }

        current_y += line_height;
    }

    // Render "PRESS SPACE TO START" if waiting
    if (state->waiting_for_space) {
        // Blink every 500ms
        Uint32 elapsed = SDL_GetTicks() - state->start_time;
        if ((elapsed / 500) % 2 == 0) {
            const char* start_text = "PRESS SPACE TO START";
            int start_scale = 2;
            int start_width = get_bitmap_text_width_scaled(&game->font, start_text, start_scale);
            int start_x = (LOGICAL_WIDTH - start_width) / 2;
            int start_y = LOGICAL_HEIGHT / 2;

            render_bitmap_text_scaled(&game->font, &game->graphics_context, start_text,
                                     start_x, start_y, FONT_COLOR_WHITE, start_scale);
        }
    }

    // Render cover image 50 pixels after the text
    int text_height = NUM_TRIBUTE_LINES * line_height;
    int cover_y = (int)(state->scroll_y + text_height + 50);

    // Scale cover to 90% of screen width, maintaining aspect ratio
    int target_cover_width = (int)(LOGICAL_WIDTH * 0.9f);
    float aspect_ratio = (float)game->cover_height / (float)game->cover_width;
    int cover_scaled_height = (int)(target_cover_width * aspect_ratio);
    int cover_x = (LOGICAL_WIDTH - target_cover_width) / 2;

    SDL_Rect dest_rect = {cover_x, cover_y, target_cover_width, cover_scaled_height};
    SDL_RenderCopy(game->graphics_context.renderer, game->cover_image.texture, NULL, &dest_rect);

    // Present the rendered frame
    SDL_RenderPresent(game->graphics_context.renderer);
}

game_stage_action_t handle_tribute_stage(tribute_stage_state_ptr state) {
    handle_input(state);
    update_scroll(state);
    render_tribute(state);
    return PROGRESS;
}
