/**
 * @file sprite_atlas.c
 * @brief Sprite atlas coordinates and metadata implementation
 */

#include "sprite_atlas.h"

// Duck sprites
const sprite_rect_t SPRITE_DUCK_NORMAL = {22, 12, 14, 11};          // Duck pointing left
const sprite_rect_t SPRITE_DUCK_SHOOTING = {42, 9, 11, 14};         // Duck shooting right
const sprite_rect_t SPRITE_DUCK_DEAD = {54, 78, 17, 10};            // Dead duck

// Popcorn sprite
const sprite_rect_t SPRITE_POPCORN = {62, 14, 4, 4};

// Crab sprites
const sprite_rect_t SPRITE_CRAB_NORMAL = {14, 30, 17, 15};          // Normal crab
const sprite_rect_t SPRITE_CRAB_WITH_BRICK = {56, 30, 16, 15};      // Crab carrying brick
const sprite_rect_t SPRITE_CRAB_DROPPING = {36, 30, 13, 14};        // Crab dropping brick

// Brick sprite
const sprite_rect_t SPRITE_BRICK = {58, 30, 11, 4};

// Jellyfish animation frames
const sprite_rect_t SPRITE_JELLYFISH_FRAMES[4] = {
    {80, 34, 12, 10},     // Frame 0
    {80, 54, 12, 11},     // Frame 1
    {106, 34, 12, 10},    // Frame 2
    {106, 54, 12, 11}     // Frame 3
};

// Heart sprite for lives
const sprite_rect_t SPRITE_HEART = {78, 0, 8, 7};

