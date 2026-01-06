/**
 * @file jellyfish.c
 * @brief Jellyfish enemy implementation
 */

#include "jellyfish.h"
#include "clock.h"

void jellyfish_init_all(jellyfish_t* jellyfish, int count, int logical_width, float y_position) {
    // Initialize jellyfish spaced evenly across the screen
    int spacing = logical_width / (count + 1);

    for (int i = 0; i < count; i++) {
        jellyfish[i].x = spacing * (i + 1);
        jellyfish[i].y = y_position;
        jellyfish[i].vx = (i % 2 == 0) ? 2.0f : -2.0f;  // Alternate directions
        jellyfish[i].moving_right = (i % 2 == 0);
        jellyfish[i].anim_frame = i % 4;  // Stagger animation frames (0, 1, 2, 3)
        jellyfish[i].last_anim_time = get_clock_ticks_ms();
    }
}

void jellyfish_update_all(jellyfish_t* jellyfish, int count, int logical_width, timestamp_ms_t current_time) {
    // Check if any jellyfish hit the edge (all bounce together)
    bool should_bounce = false;
    bool new_direction = false;

    for (int i = 0; i < count; i++) {
        float new_x = jellyfish[i].x + jellyfish[i].vx;

        if (new_x < 0 || new_x + JELLYFISH_WIDTH > logical_width) {
            should_bounce = true;
            new_direction = new_x < 0;  // true = moving right, false = moving left
            break;
        }
    }

    // Update all jellyfish together
    for (int i = 0; i < count; i++) {
        if (should_bounce) {
            // All jellyfish reverse direction together
            jellyfish[i].vx = -jellyfish[i].vx;
            jellyfish[i].moving_right = new_direction;
        }

        // Move jellyfish
        jellyfish[i].x += jellyfish[i].vx;

        // Clamp to screen bounds
        if (jellyfish[i].x < 0) {
            jellyfish[i].x = 0;
        } else if (jellyfish[i].x + JELLYFISH_WIDTH > logical_width) {
            jellyfish[i].x = logical_width - JELLYFISH_WIDTH;
        }

        // Animate jellyfish (change frame every 250ms)
        if (current_time - jellyfish[i].last_anim_time >= 250) {
            jellyfish[i].anim_frame = (jellyfish[i].anim_frame + 1) % 4;
            jellyfish[i].last_anim_time = current_time;
        }
    }
}
