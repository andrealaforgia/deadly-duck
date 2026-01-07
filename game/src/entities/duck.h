/**
 * @file duck.h
 * @brief Duck entity (player character) with Object-oriented interface
 *
 * Provides both traditional procedural interface and Object-oriented
 * methods for enhanced encapsulation and cleaner code structure.
 */

#ifndef GAME_ENTITIES_DUCK_H_
#define GAME_ENTITIES_DUCK_H_

#include <stdbool.h>
#include "types.h"

/**
 * Duck entity structure
 * 
 * Contains all state for a duck entity, designed to be used with
 * both procedural functions and Object-oriented methods.
 */
typedef struct duck_t {
    // Position and movement
    float x;           // X position
    float y;           // Y position
    float vx;          // Velocity X
    bool facing_right; // True if facing right, false if facing left
    
    // Combat state
    bool shooting;     // True if currently shooting
    timestamp_ms_t shoot_start_time;  // When shooting started
    
    // Life state
    bool dead;         // True if duck is dead
    timestamp_ms_t death_time; // When duck died
    
    // Extended Object-oriented state (optional)
    float bounds_min_x;    // Left movement boundary
    float bounds_max_x;    // Right movement boundary  
    int health;           // Health points (0 = dead)
    float max_speed;      // Maximum movement speed
} duck_t;

// Pointer typedef for duck
typedef duck_t* duck_ptr;

// Duck constants
#define DUCK_WIDTH (14 * 2)   // Sprite width at 2x scale
#define DUCK_HEIGHT (11 * 2)  // Sprite height at 2x scale
#define DUCK_SPEED 4.0f       // Default movement speed
#define DUCK_SHOOT_DURATION 100  // Shooting animation duration in ms
#define DUCK_DEFAULT_HEALTH 3    // Default health points

// =============================================================================
// PROCEDURAL INTERFACE (Backward Compatibility)
// =============================================================================

/**
 * Initialize duck at starting position (procedural interface)
 *
 * @param duck Duck to initialize
 * @param x Starting X position
 * @param y Starting Y position
 */
void duck_init(duck_ptr duck, float x, float y);

/**
 * Update duck state (procedural interface)
 *
 * @param duck Duck to update
 */
void duck_update(duck_ptr duck);

/**
 * Respawn duck after death (procedural interface)
 *
 * @param duck Duck to respawn
 * @param x Respawn X position
 * @param y Respawn Y position
 */
void duck_respawn(duck_ptr duck, float x, float y);

// =============================================================================
// OBJECT-ORIENTED INTERFACE (Enhanced)
// =============================================================================

/**
 * @brief Create and initialize a duck with Object-oriented features
 * @param x Starting X position
 * @param y Starting Y position
 * @param bounds_min_x Left movement boundary
 * @param bounds_max_x Right movement boundary
 * @return Pointer to created duck, NULL on failure
 */
duck_ptr duck_create(float x, float y, float bounds_min_x, float bounds_max_x);

/**
 * @brief Initialize duck with Object-oriented features
 * @param self Duck instance
 * @param x Starting X position
 * @param y Starting Y position
 * @param bounds_min_x Left movement boundary
 * @param bounds_max_x Right movement boundary
 */
void duck_init_bounds(duck_ptr self, float x, float y, float bounds_min_x, float bounds_max_x);

/**
 * @brief Destroy duck instance (frees memory if allocated with duck_create)
 * @param self Duck instance to destroy
 */
void duck_destroy(duck_ptr self);

/**
 * @brief Update duck with enhanced physics and boundary checking
 * @param self Duck instance
 * @param delta_time Time since last update (for smooth movement)
 */
void duck_update_enhanced(duck_ptr self, float delta_time);

/**
 * @brief Set duck velocity with automatic boundary and speed limiting
 * @param self Duck instance
 * @param velocity_x Desired horizontal velocity
 */
void duck_set_velocity(duck_ptr self, float velocity_x);

/**
 * @brief Move duck by delta amount with boundary checking
 * @param self Duck instance
 * @param dx Horizontal movement delta
 */
void duck_move(duck_ptr self, float dx);

/**
 * @brief Start shooting action (Object-oriented)
 * @param self Duck instance
 * @return true if shooting started, false if already shooting or dead
 */
bool duck_start_shooting(duck_ptr self);

/**
 * @brief Stop shooting action immediately
 * @param self Duck instance
 */
void duck_stop_shooting(duck_ptr self);

/**
 * @brief Deal damage to duck
 * @param self Duck instance
 * @param damage Amount of damage
 * @return true if duck died from damage
 */
bool duck_take_damage(duck_ptr self, int damage);

/**
 * @brief Heal duck (restore health)
 * @param self Duck instance
 * @param healing Amount of health to restore
 */
void duck_heal(duck_ptr self, int healing);

/**
 * @brief Set movement boundaries for duck
 * @param self Duck instance
 * @param min_x Left boundary
 * @param max_x Right boundary
 */
void duck_set_bounds(duck_ptr self, float min_x, float max_x);

// =============================================================================
// GETTER METHODS (Object-oriented)
// =============================================================================

/**
 * @brief Get duck position
 * @param self Duck instance
 * @param x Output X position
 * @param y Output Y position
 */
void duck_get_position(const duck_ptr self, float* x, float* y);

/**
 * @brief Get duck velocity
 * @param self Duck instance
 * @return Current horizontal velocity
 */
float duck_get_velocity(const duck_ptr self);

/**
 * @brief Get duck bounding box for collision detection
 * @param self Duck instance
 * @param x Output X position
 * @param y Output Y position
 * @param width Output width
 * @param height Output height
 */
void duck_get_bounds(const duck_ptr self, float* x, float* y, float* width, float* height);

/**
 * @brief Check if duck is facing right
 * @param self Duck instance
 * @return true if facing right, false if facing left
 */
bool duck_is_facing_right(const duck_ptr self);

/**
 * @brief Check if duck is currently shooting
 * @param self Duck instance
 * @return true if shooting, false otherwise
 */
bool duck_is_shooting(const duck_ptr self);

/**
 * @brief Check if duck is alive (health > 0 and not dead)
 * @param self Duck instance
 * @return true if alive, false if dead
 */
bool duck_is_alive(const duck_ptr self);

/**
 * @brief Get duck health
 * @param self Duck instance
 * @return Current health points
 */
int duck_get_health(const duck_ptr self);

/**
 * @brief Check if duck is within its movement boundaries
 * @param self Duck instance
 * @return true if within bounds, false otherwise
 */
bool duck_is_within_bounds(const duck_ptr self);

#endif  // GAME_ENTITIES_DUCK_H_
