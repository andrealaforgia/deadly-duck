/**
 * @file game_renderer.c
 * @brief Game rendering implementation
 */

#include "game_renderer.h"
#include <stdio.h>
#include "constants.h"
#include "bitmap_font.h"
#include "sprite_atlas.h"
#include "duck.h"
#include "crab.h"
#include "jellyfish.h"
#include "projectile.h"
#include "brick.h"

void render_lake(game_ptr game) {
    const int lake_height = LAKE_HEIGHT;
    const int lake_start_y = LAKE_START_Y;

    // Draw horizontal lines with gradient from cyan to dark blue
    for (int y = 0; y < lake_height; y++) {
        // Gradient from cyan (0, 255, 255) to dark blue (0, 0, 100)
        float t = (float)y / lake_height;
        int green_value = (int)(255 * (1.0f - t));  // From 255 to 0
        int blue_value = (int)(255 - t * 155);      // From 255 to 100
        SDL_SetRenderDrawColor(game->graphics_context.renderer, 0, green_value, blue_value, 255);
        SDL_RenderDrawLine(game->graphics_context.renderer, 0, lake_start_y + y,
                         LOGICAL_WIDTH - 1, lake_start_y + y);
    }
}

void render_duck(game_ptr game) {
    if (!game->sprite_sheet.texture) return;

    const int duck_scale = 2;  // 2x scale
    const sprite_rect_t* sprite;
    SDL_Rect src_rect;
    SDL_Rect dst_rect;

    if (game->duck.dead) {
        sprite = &SPRITE_DUCK_DEAD;
        src_rect = sprite_to_sdl_rect(sprite);

        dst_rect.x = (int)game->duck.x;
        dst_rect.y = (int)game->duck.y;
        dst_rect.w = sprite->w * duck_scale;
        dst_rect.h = sprite->h * duck_scale;

        SDL_RenderCopy(game->graphics_context.renderer, game->sprite_sheet.texture, &src_rect, &dst_rect);
    } else if (game->duck.shooting) {
        sprite = &SPRITE_DUCK_SHOOTING;
        const int normal_sprite_height = SPRITE_DUCK_NORMAL.h;
        src_rect = sprite_to_sdl_rect(sprite);

        // Adjust y position to align base with normal sprite
        int y_offset = (sprite->h - normal_sprite_height) * duck_scale;

        dst_rect.x = (int)game->duck.x;
        dst_rect.y = (int)game->duck.y - y_offset;  // Shift up to align base
        dst_rect.w = sprite->w * duck_scale;
        dst_rect.h = sprite->h * duck_scale;

        // Flip to left when facing left (sprite shows shooting right)
        SDL_RendererFlip flip = game->duck.facing_right ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
        SDL_RenderCopyEx(game->graphics_context.renderer, game->sprite_sheet.texture, &src_rect, &dst_rect,
                       0.0, NULL, flip);
    } else {
        sprite = &SPRITE_DUCK_NORMAL;
        src_rect = sprite_to_sdl_rect(sprite);

        dst_rect.x = (int)game->duck.x;
        dst_rect.y = (int)game->duck.y;
        dst_rect.w = sprite->w * duck_scale;
        dst_rect.h = sprite->h * duck_scale;

        // Flip to right when facing right (sprite points left)
        SDL_RendererFlip flip = game->duck.facing_right ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        SDL_RenderCopyEx(game->graphics_context.renderer, game->sprite_sheet.texture, &src_rect, &dst_rect,
                       0.0, NULL, flip);
    }
}

void render_popcorn(game_ptr game) {
    if (!game->sprite_sheet.texture) return;

    const int popcorn_scale = 1;   // 1x scale
    SDL_Rect src_rect = sprite_to_sdl_rect(&SPRITE_POPCORN);

    for (int i = 0; i < MAX_POPCORN; i++) {
        if (game->popcorn[i].active) {
            SDL_Rect dst_rect = {
                (int)game->popcorn[i].x,
                (int)game->popcorn[i].y,
                SPRITE_POPCORN.w * popcorn_scale,
                SPRITE_POPCORN.h * popcorn_scale
            };

            SDL_RenderCopy(game->graphics_context.renderer, game->sprite_sheet.texture, &src_rect, &dst_rect);
        }
    }
}

void render_crabs(game_ptr game) {
    if (!game->sprite_sheet.texture) return;

    const int crab_scale = 2;    // 2x scale

    for (int i = 0; i < NUM_CRABS; i++) {
        if (!game->crabs[i].alive) continue;

        const sprite_rect_t* sprite;
        SDL_Rect src_rect;
        SDL_Rect dst_rect;

        if (game->crabs[i].dropping) {
            sprite = &SPRITE_CRAB_DROPPING;
        } else if (game->crabs[i].has_brick) {
            sprite = &SPRITE_CRAB_WITH_BRICK;
        } else {
            sprite = &SPRITE_CRAB_NORMAL;
        }

        src_rect = sprite_to_sdl_rect(sprite);
        dst_rect.x = (int)game->crabs[i].x;
        dst_rect.y = (int)game->crabs[i].y;
        dst_rect.w = sprite->w * crab_scale;
        dst_rect.h = sprite->h * crab_scale;

        SDL_RenderCopy(game->graphics_context.renderer, game->sprite_sheet.texture, &src_rect, &dst_rect);
    }
}

void render_jellyfish(game_ptr game) {
    if (!game->sprite_sheet.texture) return;

    const int jellyfish_scale = 2;  // 2x scale

    for (int i = 0; i < NUM_JELLYFISH; i++) {
        const sprite_rect_t* sprite = &SPRITE_JELLYFISH_FRAMES[game->jellyfish[i].anim_frame];
        SDL_Rect src_rect = sprite_to_sdl_rect(sprite);
        SDL_Rect dst_rect;

        dst_rect.x = (int)game->jellyfish[i].x;
        dst_rect.y = (int)game->jellyfish[i].y;
        dst_rect.w = sprite->w * jellyfish_scale;
        dst_rect.h = sprite->h * jellyfish_scale;

        SDL_RenderCopy(game->graphics_context.renderer, game->sprite_sheet.texture, &src_rect, &dst_rect);
    }
}

void render_bricks(game_ptr game) {
    if (!game->sprite_sheet.texture) return;

    const int brick_scale = 1;    // 1x scale
    SDL_Rect src_rect = sprite_to_sdl_rect(&SPRITE_BRICK);

    for (int i = 0; i < MAX_BRICKS; i++) {
        if (game->bricks[i].active) {
            SDL_Rect dst_rect = {
                (int)game->bricks[i].x,
                (int)game->bricks[i].y,
                SPRITE_BRICK.w * brick_scale,
                SPRITE_BRICK.h * brick_scale
            };

            SDL_RenderCopy(game->graphics_context.renderer, game->sprite_sheet.texture, &src_rect, &dst_rect);
        }
    }
}

void render_ui(game_ptr game) {
    // Draw lives indicator (small ducks at bottom-left)
    if (game->sprite_sheet.texture) {
        const int life_duck_scale = 1;  // 1x scale
        const int spacing = 5;
        const int bottom_margin = 5;
        SDL_Rect src_rect = sprite_to_sdl_rect(&SPRITE_DUCK_NORMAL);

        int y_pos = LOGICAL_HEIGHT - SPRITE_DUCK_NORMAL.h * life_duck_scale - bottom_margin;

        for (int i = 0; i < game->lives; i++) {
            SDL_Rect dst_rect = {
                spacing + i * (SPRITE_DUCK_NORMAL.w * life_duck_scale + spacing),
                y_pos,
                SPRITE_DUCK_NORMAL.w * life_duck_scale,
                SPRITE_DUCK_NORMAL.h * life_duck_scale
            };

            // Render facing right
            SDL_RenderCopyEx(game->graphics_context.renderer, game->sprite_sheet.texture, &src_rect, &dst_rect,
                           0.0, NULL, SDL_FLIP_HORIZONTAL);
        }
    }

    // Draw score (right-aligned at bottom-right)
    if (game->font.texture.texture) {
        char score_text[32];
        snprintf(score_text, sizeof(score_text), "%d", game->score);

        const int bottom_margin = 5;
        const int right_margin = 5;

        int text_width = get_bitmap_text_width(&game->font, score_text);
        int x_pos = LOGICAL_WIDTH - text_width - right_margin;
        int y_pos = LOGICAL_HEIGHT - 7 - bottom_margin;

        render_bitmap_text(&game->font, &game->graphics_context, score_text,
                           x_pos, y_pos, FONT_COLOR_WHITE);
    }
}

void render_entities(game_ptr game) {
    render_duck(game);
    render_popcorn(game);
    render_crabs(game);
    render_jellyfish(game);
    render_bricks(game);
}

void render_game(game_ptr game) {
    // Clear screen with black
    SDL_SetRenderDrawColor(game->graphics_context.renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->graphics_context.renderer);

    // Render game elements
    render_lake(game);
    render_entities(game);
    render_ui(game);

    // Present the rendered frame
    SDL_RenderPresent(game->graphics_context.renderer);
}
