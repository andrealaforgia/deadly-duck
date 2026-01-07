/**
 * @file duck.c
 * @brief Duck entity implementation with both procedural and Object-oriented interfaces
 */

#include "duck.h"
#include "clock.h"
#include "constants.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Private helper functions
static void duck_update_shooting(duck_ptr duck);
static void duck_init_extended(duck_ptr duck);

// =============================================================================
// PROCEDURAL INTERFACE (Backward Compatibility)
// =============================================================================

void duck_init(duck_ptr duck, float x, float y) {
    if (!duck) return;
    
    // Initialize basic state
    duck->x = x;
    duck->y = y;
    duck->vx = 0.0f;
    duck->facing_right = true;
    duck->shooting = false;
    duck->shoot_start_time = 0;
    duck->dead = false;
    duck->death_time = 0;
    
    // Initialize extended state with defaults (wide bounds for backward compatibility)
    duck_init_extended(duck);
}

void duck_update(duck_ptr duck) {
    if (!duck) return;
    
    // Apply movement for backward compatibility
    duck->x += duck->vx;
    
    // Simple boundary checking without stopping velocity
    if (duck->x < 0) {
        duck->x = 0;
    } else if (duck->x + DUCK_WIDTH > LOGICAL_WIDTH) {
        duck->x = LOGICAL_WIDTH - DUCK_WIDTH;
    }
    
    duck_update_shooting(duck);
}

void duck_respawn(duck_ptr duck, float x, float y) {
    if (!duck) return;
    
    duck->x = x;
    duck->y = y;
    duck->vx = 0.0f;
    duck->facing_right = true;
    duck->shooting = false;
    duck->dead = false;
    duck->death_time = 0;
    duck->shoot_start_time = 0;
    
    // Reset extended state
    duck->health = DUCK_DEFAULT_HEALTH;
}

// =============================================================================
// OBJECT-ORIENTED INTERFACE (Enhanced)
// =============================================================================

duck_ptr duck_create(float x, float y, float bounds_min_x, float bounds_max_x) {
    duck_ptr self = malloc(sizeof(duck_t));
    if (!self) return NULL;
    
    duck_init_bounds(self, x, y, bounds_min_x, bounds_max_x);
    return self;
}

void duck_init_bounds(duck_ptr self, float x, float y, float bounds_min_x, float bounds_max_x) {
    if (!self) return;
    
    // Initialize basic state using procedural function
    duck_init(self, x, y);
    
    // Set Object-oriented specific state
    self->bounds_min_x = bounds_min_x;
    self->bounds_max_x = bounds_max_x;
    self->health = DUCK_DEFAULT_HEALTH;
    self->max_speed = DUCK_SPEED * 1.5f;  // Allow slightly faster than default
}

void duck_destroy(duck_ptr self) {
    if (self) {
        free(self);
    }
}

void duck_update_enhanced(duck_ptr self, float delta_time) {
    if (!self) return;
    
    // Call base update
    duck_update_shooting(self);
    
    // Apply enhanced movement with delta time
    self->x += self->vx * delta_time;
    
    // Enhanced boundary checking with stopping
    if (self->x < self->bounds_min_x) {
        self->x = self->bounds_min_x;
        self->vx = 0;  // Stop at boundary
    } else if (self->x + DUCK_WIDTH > self->bounds_max_x) {
        self->x = self->bounds_max_x - DUCK_WIDTH;
        self->vx = 0;  // Stop at boundary
    }
}

void duck_set_velocity(duck_ptr self, float velocity_x) {
    if (!self || !duck_is_alive(self)) return;
    
    // Clamp to max speed
    if (velocity_x > self->max_speed) {
        velocity_x = self->max_speed;
    } else if (velocity_x < -self->max_speed) {
        velocity_x = -self->max_speed;
    }
    
    self->vx = velocity_x;
    
    // Update facing direction
    if (velocity_x > 0) {
        self->facing_right = true;
    } else if (velocity_x < 0) {
        self->facing_right = false;
    }
}

void duck_move(duck_ptr self, float dx) {
    if (!self || !duck_is_alive(self)) return;
    
    self->x += dx;
    
    // Apply boundary checking
    if (self->x < self->bounds_min_x) {
        self->x = self->bounds_min_x;
    } else if (self->x + DUCK_WIDTH > self->bounds_max_x) {
        self->x = self->bounds_max_x - DUCK_WIDTH;
    }
}

bool duck_start_shooting(duck_ptr self) {
    if (!self || !duck_is_alive(self) || duck_is_shooting(self)) {
        return false;
    }
    
    self->shooting = true;
    self->shoot_start_time = get_clock_ticks_ms();
    return true;
}

void duck_stop_shooting(duck_ptr self) {
    if (!self) return;
    
    self->shooting = false;
}

bool duck_take_damage(duck_ptr self, int damage) {
    if (!self || !duck_is_alive(self)) return false;
    
    self->health -= damage;
    if (self->health <= 0) {
        self->health = 0;
        self->dead = true;
        self->death_time = get_clock_ticks_ms();
        self->vx = 0;  // Stop movement
        return true;  // Duck died
    }
    
    return false;  // Duck still alive
}

void duck_heal(duck_ptr self, int healing) {
    if (!self) return;
    
    self->health += healing;
    // Note: Could add max health cap later
}

void duck_set_bounds(duck_ptr self, float min_x, float max_x) {
    if (!self) return;
    
    self->bounds_min_x = min_x;
    self->bounds_max_x = max_x;
}

// =============================================================================
// GETTER METHODS (Object-oriented)
// =============================================================================

void duck_get_position(const duck_ptr self, float* x, float* y) {
    if (!self || !x || !y) return;
    
    *x = self->x;
    *y = self->y;
}

float duck_get_velocity(const duck_ptr self) {
    return self ? self->vx : 0.0f;
}

void duck_get_bounds(const duck_ptr self, float* x, float* y, float* width, float* height) {
    if (!self || !x || !y || !width || !height) return;
    
    *x = self->x;
    *y = self->y;
    *width = DUCK_WIDTH;
    *height = DUCK_HEIGHT;
}

bool duck_is_facing_right(const duck_ptr self) {
    return self ? self->facing_right : false;
}

bool duck_is_shooting(const duck_ptr self) {
    return self ? self->shooting : false;
}

bool duck_is_alive(const duck_ptr self) {
    return self ? (!self->dead && self->health > 0) : false;
}

int duck_get_health(const duck_ptr self) {
    return self ? self->health : 0;
}

bool duck_is_within_bounds(const duck_ptr self) {
    if (!self) return false;
    
    return (self->x >= self->bounds_min_x && 
            self->x + DUCK_WIDTH <= self->bounds_max_x);
}

// =============================================================================
// PRIVATE HELPER FUNCTIONS
// =============================================================================

static void duck_update_shooting(duck_ptr duck) {
    if (!duck || !duck->shooting) return;
    
    timestamp_ms_t current_time = get_clock_ticks_ms();
    if (current_time - duck->shoot_start_time >= DUCK_SHOOT_DURATION) {
        duck->shooting = false;
    }
}

static void duck_init_extended(duck_ptr duck) {
    if (!duck) return;
    
    // Set reasonable defaults for Object-oriented features
    duck->bounds_min_x = 0;
    duck->bounds_max_x = LOGICAL_WIDTH;  // Use actual screen width
    duck->health = DUCK_DEFAULT_HEALTH;
    duck->max_speed = DUCK_SPEED * 1.5f;
}
