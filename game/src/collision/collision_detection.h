/**
 * @file collision_detection.h
 * @brief Simple collision detection functions
 *
 * Provides basic AABB collision detection for all game entities.
 * No patterns, no abstractions - just simple geometry checks.
 */

#ifndef COLLISION_DETECTION_H
#define COLLISION_DETECTION_H

#include <stdbool.h>

/**
 * @brief Simple AABB collision detection
 * @param x1 First rectangle x position
 * @param y1 First rectangle y position
 * @param w1 First rectangle width
 * @param h1 First rectangle height
 * @param x2 Second rectangle x position
 * @param y2 Second rectangle y position
 * @param w2 Second rectangle width
 * @param h2 Second rectangle height
 * @return true if rectangles overlap
 */
bool check_aabb_collision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);

#endif // COLLISION_DETECTION_H
