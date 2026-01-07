/**
 * @file jellyfish.c
 * @brief Jellyfish enemy implementation
 */

#include "jellyfish.h"
#include "clock.h"

void jellyfish_update_all(object_pool_t* pool, int logical_width, timestamp_ms_t current_time) {
    // Check if any jellyfish hit the edge (all bounce together)
    bool should_bounce = false;
    bool new_direction = false;

    for (size_t i = 0; i < pool->capacity; i++) {
        if (!pool_is_active(pool, i)) continue;
        
        jellyfish_ptr jellyfish = (jellyfish_ptr)pool_get_at(pool, i);
        if (!jellyfish) continue;
        
        float new_x = jellyfish->x + jellyfish->vx;

        if (new_x < 0 || new_x + JELLYFISH_WIDTH > logical_width) {
            should_bounce = true;
            new_direction = new_x < 0;  // true = moving right, false = moving left
            break;
        }
    }

    // Update all jellyfish together
    for (size_t i = 0; i < pool->capacity; i++) {
        if (!pool_is_active(pool, i)) continue;
        
        jellyfish_ptr jellyfish = (jellyfish_ptr)pool_get_at(pool, i);
        if (!jellyfish) continue;
        
        if (should_bounce) {
            // All jellyfish reverse direction together
            jellyfish->vx = -jellyfish->vx;
            jellyfish->moving_right = new_direction;
        }

        // Move jellyfish
        jellyfish->x += jellyfish->vx;

        // Clamp to screen bounds
        if (jellyfish->x < 0) {
            jellyfish->x = 0;
        } else if (jellyfish->x + JELLYFISH_WIDTH > logical_width) {
            jellyfish->x = logical_width - JELLYFISH_WIDTH;
        }

        // Animate jellyfish
        if (current_time - jellyfish->last_anim_time >= ANIMATION_CYCLE_MS) {
            jellyfish->anim_frame = (jellyfish->anim_frame + 1) % JELLYFISH_FRAME_COUNT;
            jellyfish->last_anim_time = current_time;
        }
    }
}
