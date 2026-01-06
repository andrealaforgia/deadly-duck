/**
 * @file crab.c
 * @brief Crab enemy implementation
 */

#include "crab.h"
#include <stdlib.h>

void crabs_init_all(crab_t* crabs, int count, int logical_width, float y_position) {
    // Initialize crabs spaced evenly across the screen
    int spacing = logical_width / (count + 1);

    for (int i = 0; i < count; i++) {
        crabs[i].x = spacing * (i + 1) - CRAB_WIDTH / 2;
        crabs[i].y = y_position;
        crabs[i].vx = (i % 2 == 0) ? 1.5f : -1.5f;  // Alternate directions
        crabs[i].moving_right = (i % 2 == 0);
        crabs[i].alive = true;
        crabs[i].has_brick = false;
        crabs[i].off_screen = false;
        crabs[i].dropping = false;
        crabs[i].drop_start_time = 0;
        crabs[i].next_drop_time = SDL_GetTicks() + 3000 + (rand() % 5000);
    }
}

void crabs_update_all(crab_t* crabs, int count, brick_t* bricks, int brick_count,
                      int logical_width, Uint32 current_time,
                      void (*play_sound_callback)(void*, int), void* sound_context) {
    for (int i = 0; i < count; i++) {
        if (!crabs[i].alive) continue;  // Skip dead crabs

        // Update dropping animation
        if (crabs[i].dropping) {
            if (current_time - crabs[i].drop_start_time > DROP_ANIM_DURATION) {
                crabs[i].dropping = false;
                crabs[i].has_brick = false;
                // Set next drop time (3-8 seconds from now)
                crabs[i].next_drop_time = current_time + 3000 + (rand() % 5000);
            }
        }

        // Check if it's time to drop brick AND crab is in central 80% of screen
        float drop_zone_start = logical_width * 0.1f;  // 10% from left
        float drop_zone_end = logical_width * 0.9f;    // 10% from right
        bool in_drop_zone = crabs[i].x >= drop_zone_start &&
                           crabs[i].x + CRAB_WIDTH <= drop_zone_end;

        if (crabs[i].has_brick && !crabs[i].dropping &&
            current_time >= crabs[i].next_drop_time && in_drop_zone) {
            // Start dropping animation
            crabs[i].dropping = true;
            crabs[i].drop_start_time = current_time;

            // Play brick drop sound
            if (play_sound_callback && sound_context) {
                play_sound_callback(sound_context, 2);  // SOUND_BRICK_DROP = 2
            }

            // Spawn a falling brick
            brick_spawn(bricks, brick_count,
                       crabs[i].x + (CRAB_WIDTH / 2) - 6,  // Center under crab
                       crabs[i].y + CRAB_HEIGHT);          // Below crab
        }

        // Move crab
        crabs[i].x += crabs[i].vx;

        // Check if crab is fully off screen (for brick pickup)
        if (crabs[i].x < -CRAB_WIDTH || crabs[i].x > logical_width) {
            if (!crabs[i].off_screen) {
                crabs[i].off_screen = true;
                // Crab gets brick when it goes off-screen (max 6 crabs with bricks)
                if (!crabs[i].has_brick) {
                    // Count how many crabs currently have bricks
                    int crabs_with_bricks = 0;
                    for (int j = 0; j < count; j++) {
                        if (crabs[j].has_brick) {
                            crabs_with_bricks++;
                        }
                    }
                    // Only give brick if less than max
                    if (crabs_with_bricks < MAX_CRABS_WITH_BRICKS) {
                        crabs[i].has_brick = true;
                    }
                }
            }
        } else if (crabs[i].off_screen) {
            // Coming back on screen
            crabs[i].off_screen = false;
        }

        // Wrap around screen edges (seamless wrapping)
        if (crabs[i].x < -CRAB_WIDTH) {
            crabs[i].x = logical_width;
        } else if (crabs[i].x > logical_width) {
            crabs[i].x = -CRAB_WIDTH;
        }
    }
}
