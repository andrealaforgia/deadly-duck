#ifndef CONSTANTS_H
#define CONSTANTS_H

// Screen dimensions with 1.48:1 aspect ratio (width:height)
// Using a base height of 480 pixels
#define LOGICAL_WIDTH 710   // 480 * 1.48 ≈ 710
#define LOGICAL_HEIGHT 480

// Window will scale to fit screen while maintaining aspect ratio
#define WINDOW_SCALE_FACTOR 0.9  // Use 90% of screen height

// Game timing
#define FPS 60
#define FRAME_DELAY (1000 / FPS)

// Side rectangle dimensions
#define SIDE_RECT_WIDTH ((int)(LOGICAL_WIDTH * 0.055))  // 0.055 * 710 = 39 pixels

// Lake calculations
#define LAKE_HEIGHT (LOGICAL_HEIGHT / 10)
#define LAKE_START_Y (LOGICAL_HEIGHT - LAKE_HEIGHT)

#endif // CONSTANTS_H
