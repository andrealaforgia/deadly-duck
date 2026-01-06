/**
 * @file duck.h
 * @brief Duck entity (player character)
 */

#ifndef GAME_ENTITIES_DUCK_H_
#define GAME_ENTITIES_DUCK_H_

#include <stdbool.h>
#include <SDL.h>

/**
 * Duck entity structure
 */
typedef struct {
    float x;           // X position
    float y;           // Y position
    float vx;          // Velocity X
    bool facing_right; // True if facing right, false if facing left
    bool shooting;     // True if currently shooting
    Uint32 shoot_start_time;  // When shooting started
    bool dead;         // True if duck is dead
    Uint32 death_time; // When duck died
} duck_t;

// Duck sprite dimensions (2x scale)
#define DUCK_WIDTH (14 * 2)   // (35 - 22 + 1) * 2x scale
#define DUCK_HEIGHT (11 * 2)  // (22 - 12 + 1) * 2x scale
#define DUCK_SPEED 4.0f       // Movement speed

/**
 * Initialize duck at starting position
 *
 * @param duck Duck to initialize
 * @param x Starting X position
 * @param y Starting Y position
 */
void duck_init(duck_t* duck, float x, float y);

/**
 * Update duck state (shooting animation timing)
 *
 * @param duck Duck to update
 */
void duck_update(duck_t* duck);

/**
 * Respawn duck after death
 *
 * @param duck Duck to respawn
 * @param x Respawn X position
 * @param y Respawn Y position
 */
void duck_respawn(duck_t* duck, float x, float y);

#endif  // GAME_ENTITIES_DUCK_H_
