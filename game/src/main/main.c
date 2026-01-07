#include <stdbool.h>
#include <stdio.h>
#include "constants.h"
#include "game.h"
#include "stage_director.h"
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

    // Initialize stage director
    stage_director_t stage_director = {0};
    if (!stage_director_init(&stage_director, &game)) {
        game_terminate(&game);
        return 1;
    }

    // Initialize frame limiter
    int target_fps = 1000 / FRAME_DELAY;  // Convert from frame delay to FPS
    frame_limiter_t frame_limiter = create_frame_limiter(target_fps);

    // Game loop
    while (game.running) {
        // Update stages and handle transitions
        game_stage_action_t action = stage_director_update(&stage_director, &game);

        if (action == QUIT) {
            game.running = false;
        }

        // Frame rate limiting using engine
        frame_limiter_wait(&frame_limiter);
    }

    // Cleanup
    stage_director_cleanup(&stage_director);
    game_terminate(&game);
    return 0;
}
