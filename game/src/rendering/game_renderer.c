/**
 * @file game_renderer.c
 * @brief Game rendering implementation
 */

#include "game_renderer.h"
#include <stdio.h>
#include "constants.h"
#include "bitmap_font.h"
#include "sprite_atlas.h"
#include "frame.h"
#include "duck.h"
#include "crab.h"
#include "jellyfish.h"
#include "projectile.h"
#include "brick.h"

void render_lake(game_ptr game) {
    const int lake_height = LAKE_HEIGHT;
    const int lake_start_y = LAKE_START_Y;

    // Draw horizontal lines with gradient from cyan to dark blue using engine
    for (int y = 0; y < lake_height; y++) {
        // Gradient from cyan (0, 255, 255) to dark blue (0, 0, 100)
        float t = (float)y / lake_height;
        int green_value = (int)(255 * (1.0f - t));  // From 255 to 0
        int blue_value = (int)(255 - t * 155);      // From 255 to 100
        color_t line_color = COLOR(0, green_value, blue_value);
        draw_line(&game->graphics_context, 0, lake_start_y + y,
                  LOGICAL_WIDTH - 1, lake_start_y + y, line_color);
    }
}

void render_duck(game_ptr game) {
    if (!game->sprite_sheet.texture) return;

    const int duck_scale = 2;  // 2x scale
    const sprite_rect_t* sprite;

    if (game->duck.dead) {
        sprite = &SPRITE_DUCK_DEAD;
        rect_t src_rect = make_rect(sprite->x, sprite->y, sprite->w, sprite->h);
        render_sprite_scaled(&game->graphics_context, &game->sprite_sheet, &src_rect,
                           (int)game->duck.x, (int)game->duck.y, duck_scale);
    } else if (game->duck.shooting) {
        sprite = &SPRITE_DUCK_SHOOTING;
        const int normal_sprite_height = SPRITE_DUCK_NORMAL.h;
        rect_t src_rect = make_rect(sprite->x, sprite->y, sprite->w, sprite->h);

        // Adjust y position to align base with normal sprite
        int y_offset = (sprite->h - normal_sprite_height) * duck_scale;
        rect_t dst_rect = make_rect((int)game->duck.x, (int)game->duck.y - y_offset, 
                                   sprite->w * duck_scale, sprite->h * duck_scale);

        // Flip to left when facing left (sprite shows shooting right)
        flip_t flip = game->duck.facing_right ? FLIP_NONE : FLIP_HORIZONTAL;
        render_sprite_flipped(&game->graphics_context, &game->sprite_sheet, &src_rect, &dst_rect, flip);
    } else {
        sprite = &SPRITE_DUCK_NORMAL;
        rect_t src_rect = make_rect(sprite->x, sprite->y, sprite->w, sprite->h);
        rect_t dst_rect = make_rect((int)game->duck.x, (int)game->duck.y, 
                                   sprite->w * duck_scale, sprite->h * duck_scale);

        // Flip to right when facing right (sprite points left)
        flip_t flip = game->duck.facing_right ? FLIP_HORIZONTAL : FLIP_NONE;
        render_sprite_flipped(&game->graphics_context, &game->sprite_sheet, &src_rect, &dst_rect, flip);
    }
}

void render_popcorn(game_ptr game) {
    if (!game->sprite_sheet.texture) return;

    const int popcorn_scale = 1;   // 1x scale
    rect_t src_rect = make_rect(SPRITE_POPCORN.x, SPRITE_POPCORN.y, SPRITE_POPCORN.w, SPRITE_POPCORN.h);

    for (size_t i = 0; i < game->popcorn_pool.capacity; i++) {
        if (!pool_is_active(&game->popcorn_pool, i)) continue;
        
        popcorn_ptr popcorn = (popcorn_ptr)pool_get_at(&game->popcorn_pool, i);
        if (popcorn && popcorn->active) {
            render_sprite_scaled(&game->graphics_context, &game->sprite_sheet, &src_rect,
                               (int)popcorn->x, (int)popcorn->y, popcorn_scale);
        }
    }
}

void render_crabs(game_ptr game) {
    if (!game->sprite_sheet.texture) return;

    const int crab_scale = 2;    // 2x scale

    for (size_t i = 0; i < game->crab_pool.capacity; i++) {
        if (!pool_is_active(&game->crab_pool, i)) continue;
        
        crab_ptr crab = (crab_ptr)pool_get_at(&game->crab_pool, i);
        if (!crab || !crab->alive) continue;

        const sprite_rect_t* sprite;

        if (crab->dropping) {
            sprite = &SPRITE_CRAB_DROPPING;
        } else if (crab->has_brick) {
            sprite = &SPRITE_CRAB_WITH_BRICK;
        } else {
            sprite = &SPRITE_CRAB_NORMAL;
        }

        rect_t src_rect = make_rect(sprite->x, sprite->y, sprite->w, sprite->h);
        render_sprite_scaled(&game->graphics_context, &game->sprite_sheet, &src_rect,
                           (int)crab->x, (int)crab->y, crab_scale);
    }
}

void render_jellyfish(game_ptr game) {
    if (!game->sprite_sheet.texture) return;

    const int jellyfish_scale = 2;  // 2x scale

    for (size_t i = 0; i < game->jellyfish_pool.capacity; i++) {
        if (!pool_is_active(&game->jellyfish_pool, i)) continue;
        
        jellyfish_ptr jellyfish = (jellyfish_ptr)pool_get_at(&game->jellyfish_pool, i);
        if (!jellyfish) continue;
        
        const sprite_rect_t* sprite = &SPRITE_JELLYFISH_FRAMES[jellyfish->anim_frame];
        rect_t src_rect = make_rect(sprite->x, sprite->y, sprite->w, sprite->h);
        render_sprite_scaled(&game->graphics_context, &game->sprite_sheet, &src_rect,
                           (int)jellyfish->x, (int)jellyfish->y, jellyfish_scale);
    }
}

void render_bricks(game_ptr game) {
    if (!game->sprite_sheet.texture) return;

    const int brick_scale = 1;    // 1x scale
    rect_t src_rect = make_rect(SPRITE_BRICK.x, SPRITE_BRICK.y, SPRITE_BRICK.w, SPRITE_BRICK.h);

    for (size_t i = 0; i < game->brick_pool.capacity; i++) {
        if (!pool_is_active(&game->brick_pool, i)) continue;
        
        brick_ptr brick = (brick_ptr)pool_get_at(&game->brick_pool, i);
        if (brick && brick->active) {
            render_sprite_scaled(&game->graphics_context, &game->sprite_sheet, &src_rect,
                               (int)brick->x, (int)brick->y, brick_scale);
        }
    }
}

void render_ui(game_ptr game) {
    // Draw lives indicator (small ducks at bottom-left)
    if (game->sprite_sheet.texture) {
        const int life_duck_scale = 1;  // 1x scale
        const int spacing = 5;
        const int bottom_margin = 5;
        rect_t src_rect = make_rect(SPRITE_DUCK_NORMAL.x, SPRITE_DUCK_NORMAL.y, 
                                   SPRITE_DUCK_NORMAL.w, SPRITE_DUCK_NORMAL.h);

        int y_pos = LOGICAL_HEIGHT - SPRITE_DUCK_NORMAL.h * life_duck_scale - bottom_margin;

        for (int i = 0; i < game->lives; i++) {
            int x_pos = spacing + i * (SPRITE_DUCK_NORMAL.w * life_duck_scale + spacing);
            rect_t dst_rect = make_rect(x_pos, y_pos, 
                                       SPRITE_DUCK_NORMAL.w * life_duck_scale,
                                       SPRITE_DUCK_NORMAL.h * life_duck_scale);

            // Render facing right
            render_sprite_flipped(&game->graphics_context, &game->sprite_sheet, &src_rect, &dst_rect, FLIP_HORIZONTAL);
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
    // Clear screen using engine
    clear_frame(&game->graphics_context);

    // Render game elements
    render_lake(game);
    render_entities(game);
    render_ui(game);

    // Present the rendered frame using engine
    render_frame(&game->graphics_context);
}
