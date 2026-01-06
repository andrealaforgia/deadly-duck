/**
 * @file player_controller.c
 * @brief Player input controller implementation
 */

#include "player_controller.h"
#include "game.h"
#include "audio.h"
#include "projectile.h"
#include "duck.h"
#include "keyboard.h"
#include "events.h"
#include "clock.h"

bool player_process_input(game_ptr game) {
    // Check for quit event using engine event system
    event_t engine_event = poll_event();
    if (engine_event == QUIT_EVENT) {
        return false;
    }
    
    // Check keyboard state using engine
    keyboard_state_t* keyboard_state = &game->keyboard_state;
    
    if (is_esc_key_pressed(keyboard_state)) {
        return false;
    }

    // Handle duck movement and shooting controls (only if duck is alive)
    if (!game->duck.dead) {
        // Handle left movement
        if (is_left_key_pressed(keyboard_state)) {
            game->duck.vx = -DUCK_SPEED;
            game->duck.facing_right = false;
        }
        
        // Handle right movement  
        if (is_right_key_pressed(keyboard_state)) {
            game->duck.vx = DUCK_SPEED;
            game->duck.facing_right = true;
        }
        
        // Handle shooting
        if (is_space_key_pressed(keyboard_state)) {
            // Trigger shooting
            game->duck.shooting = true;
            game->duck.shoot_start_time = get_clock_ticks_ms();

            // Play quack sound
            play_sound(&game->audio_context, SOUND_QUACK);

            // Spawn a popcorn projectile
            const int duck_sprite_width = DUCK_WIDTH;
            float offset = game->duck.facing_right ? duck_sprite_width * 0.7f : duck_sprite_width * 0.3f;

            popcorn_spawn(game->popcorn, MAX_POPCORN,
                         game->duck.x + offset,
                         game->duck.y);
        }
        
        // Stop horizontal movement when neither left nor right is pressed
        if (!is_left_key_pressed(keyboard_state) && !is_right_key_pressed(keyboard_state)) {
            game->duck.vx = 0;
        }
    }
    
    return true;
}
