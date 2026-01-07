/**
 * @file crab.c
 * @brief Crab enemy implementation
 */

#include "crab.h"
#include "clock.h"
#include <stdlib.h>

void crabs_update_all(object_pool_t* crab_pool, object_pool_t* brick_pool,
                      int logical_width, timestamp_ms_t current_time,
                      void (*play_sound_callback)(void*, int), void* sound_context) {
    // Manual iteration since we need to access all crabs
    for (size_t i = 0; i < crab_pool->capacity; i++) {
        if (!pool_is_active(crab_pool, i)) continue;
        
        crab_ptr crab = (crab_ptr)pool_get_at(crab_pool, i);
        if (!crab || !crab->alive) continue;

        // Update dropping animation
        if (crab->dropping) {
            if (current_time - crab->drop_start_time > DROP_ANIM_DURATION) {
                crab->dropping = false;
                crab->has_brick = false;
                // Set next drop time (3-8 seconds from now)
                crab->next_drop_time = current_time + 3000 + (rand() % 5000);
            }
        }

        // Check if it's time to drop brick AND crab is in central 80% of screen
        float drop_zone_start = logical_width * 0.1f;  // 10% from left
        float drop_zone_end = logical_width * 0.9f;    // 10% from right
        bool in_drop_zone = crab->x >= drop_zone_start &&
                           crab->x + CRAB_WIDTH <= drop_zone_end;

        if (crab->has_brick && !crab->dropping &&
            current_time >= crab->next_drop_time && in_drop_zone) {
            // Start dropping animation
            crab->dropping = true;
            crab->drop_start_time = current_time;

            // Play brick drop sound
            if (play_sound_callback && sound_context) {
                play_sound_callback(sound_context, 2);  // SOUND_BRICK_DROP = 2
            }

            // Spawn a falling brick
            brick_spawn(brick_pool,
                       crab->x + (CRAB_WIDTH / 2) - 6,  // Center under crab
                       crab->y + CRAB_HEIGHT);          // Below crab
        }

        // Move crab
        crab->x += crab->vx;

        // Check if crab is fully off screen (for brick pickup)
        if (crab->x < -CRAB_WIDTH || crab->x > logical_width) {
            if (!crab->off_screen) {
                crab->off_screen = true;
                // Crab gets brick when it goes off-screen (max 6 crabs with bricks)
                if (!crab->has_brick) {
                    // Count how many crabs currently have bricks
                    int crabs_with_bricks = 0;
                    for (size_t j = 0; j < crab_pool->capacity; j++) {
                        if (!pool_is_active(crab_pool, j)) continue;
                        crab_ptr other_crab = (crab_ptr)pool_get_at(crab_pool, j);
                        if (other_crab && other_crab->has_brick) {
                            crabs_with_bricks++;
                        }
                    }
                    // Only give brick if less than max
                    if (crabs_with_bricks < MAX_CRABS_WITH_BRICKS) {
                        crab->has_brick = true;
                    }
                }
            }
        } else if (crab->off_screen) {
            // Coming back on screen
            crab->off_screen = false;
        }

        // Wrap around screen edges (seamless wrapping)
        if (crab->x < -CRAB_WIDTH) {
            crab->x = logical_width;
        } else if (crab->x > logical_width) {
            crab->x = -CRAB_WIDTH;
        }
    }
}
