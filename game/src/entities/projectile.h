/**
 * @file projectile.h
 * @brief Popcorn projectile entity
 */

#ifndef GAME_ENTITIES_PROJECTILE_H_
#define GAME_ENTITIES_PROJECTILE_H_

#include <stdbool.h>
#include <SDL.h>

/**
 * Popcorn projectile structure
 */
typedef struct {
    float x;           // X position
    float y;           // Y position
    float vy;          // Velocity Y (vertical movement, can be negative if reflected)
    bool active;       // True if this popcorn is in flight
    bool reflected;    // True if reflected by jellyfish
} popcorn_t;

// Popcorn sprite dimensions
#define POPCORN_WIDTH 7   // Sprite width
#define POPCORN_HEIGHT 6  // Sprite height
#define POPCORN_SPEED 6.0f  // Upward speed

// Pool size
#define MAX_POPCORN 10  // Maximum number of popcorn projectiles on screen

/**
 * Initialize popcorn pool (set all to inactive)
 *
 * @param popcorn Array of popcorn projectiles
 * @param count Number of projectiles in array
 */
void popcorn_pool_init(popcorn_t* popcorn, int count);

/**
 * Spawn a popcorn projectile
 *
 * @param popcorn Array of popcorn projectiles
 * @param count Number of projectiles in array
 * @param x Starting X position
 * @param y Starting Y position
 * @return true if spawned successfully, false if pool is full
 */
bool popcorn_spawn(popcorn_t* popcorn, int count, float x, float y);

/**
 * Update all active popcorn projectiles
 *
 * @param popcorn Array of popcorn projectiles
 * @param count Number of projectiles in array
 * @param logical_height Screen height for bounds checking
 */
void popcorn_update_all(popcorn_t* popcorn, int count, int logical_height);

/**
 * Reflect a popcorn projectile downward
 *
 * @param projectile Projectile to reflect
 */
void popcorn_reflect(popcorn_t* projectile);

#endif  // GAME_ENTITIES_PROJECTILE_H_
