/**
 * @file sprite_atlas.h
 * @brief Sprite atlas coordinates and metadata
 *
 * Contains all sprite sheet coordinates and dimensions for game entities.
 */

#ifndef SPRITE_ATLAS_H
#define SPRITE_ATLAS_H

/**
 * Sprite rectangle definition
 */
typedef struct {
    int x, y, w, h;
} sprite_rect_t;

// Duck sprites
extern const sprite_rect_t SPRITE_DUCK_NORMAL;
extern const sprite_rect_t SPRITE_DUCK_SHOOTING;
extern const sprite_rect_t SPRITE_DUCK_DEAD;

// Popcorn sprite
extern const sprite_rect_t SPRITE_POPCORN;

// Crab sprites
extern const sprite_rect_t SPRITE_CRAB_NORMAL;
extern const sprite_rect_t SPRITE_CRAB_WITH_BRICK;
extern const sprite_rect_t SPRITE_CRAB_DROPPING;

// Brick sprite
extern const sprite_rect_t SPRITE_BRICK;

// Jellyfish animation frames (4 frames)
extern const sprite_rect_t SPRITE_JELLYFISH_FRAMES[4];

// Heart sprite for lives
extern const sprite_rect_t SPRITE_HEART;

#endif // SPRITE_ATLAS_H
