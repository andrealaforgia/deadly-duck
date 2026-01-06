#include <stdbool.h>
#include <stdio.h>
#include "constants.h"
#include "game.h"
#include "stage_manager.h"
#include "frame_limiter.h"

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    game_t game = {0};

    if (!game_init(&game)) {
        game_terminate(&game);
        return 1;
    }

    printf("Deadly Duck - Press ESC to quit\n");
    printf("Sprite sheet dimensions: %dx%d (%.2f:1 ratio)\n",
           game.sprite_sheet.width, game.sprite_sheet.height,
           (float)game.sprite_sheet.width / game.sprite_sheet.height);

    // Initialize stage manager
    stage_manager_t stage_manager = {0};
    if (!stage_manager_init(&stage_manager, &game)) {
        game_terminate(&game);
        return 1;
    }

    // Initialize frame limiter
    int target_fps = 1000 / FRAME_DELAY;  // Convert from frame delay to FPS
    frame_limiter_t frame_limiter = create_frame_limiter(target_fps);

    // Game loop
    while (game.running) {
        // Update stages and handle transitions
        game_stage_action_t action = stage_manager_update(&stage_manager, &game);

        if (action == QUIT) {
            game.running = false;
        }

        // Frame rate limiting using engine
        frame_limiter_wait(&frame_limiter);
    }

    // Cleanup
    stage_manager_cleanup(&stage_manager);
    game_terminate(&game);
    return 0;
}
