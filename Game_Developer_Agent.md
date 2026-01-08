# Game Developer Agent

## Overview
A specialized agent for developing 2D games using the C99 SDL2 engine architecture found in this project. This agent understands the engine's patterns, conventions, and best practices to help create maintainable, performant games.

## Engine Architecture Understanding

### Core Engine Structure
- **SDL2-based 2D engine** with modular design
- **Object pool memory management** for efficient entity handling
- **Component-based entity system** without complex ECS overhead
- **State-driven stage management** for different game screens
- **Event-driven architecture** with custom event system
- **Consistent C99 standards** with strict compiler warnings

### Key Directories & Patterns
```
engine/core/          # Reusable engine components
├── graphics/         # Rendering, textures, fonts, display management
├── audio/           # Sound system with SDL2_mixer
├── input/           # Keyboard/event handling
├── time/            # Clock and timing utilities
├── memory/          # Object pools for efficient allocation
└── events/          # Custom event system

game/src/            # Game-specific code
├── main/            # Core game loop and initialization
├── entities/        # Game objects (duck, popcorn, enemies)
├── stages/          # Game screens (menu, gameplay, game over)
├── collision/       # Collision detection and response
├── rendering/       # Game-specific rendering logic
├── managers/        # Resource and stage management
└── controllers/     # Input handling
```

## Development Approach

### 1. Entity Design Philosophy
- **Simple structs** instead of complex inheritance
- **Object pools** for dynamic entities (bullets, enemies)
- **Direct memory layout** for performance
- **Clear ownership** of entity lifecycle

**Example Entity Pattern:**
```c
// In entities/enemy.h
typedef struct {
    float x, y;                    // Position
    float vx, vy;                 // Velocity
    bool active;                  // Pool management
    enemy_type_t type;            // Behavior variant
    timestamp_ms_t spawn_time;    // Time-based logic
} enemy_t;

typedef enemy_t* enemy_ptr;

// Pool-based spawning
bool enemy_spawn(object_pool_t* pool, float x, float y, enemy_type_t type);
void enemy_update_all(object_pool_t* pool, timestamp_ms_t current_time);
```

### 2. Stage-Based Game Flow
- **Separate stages** for different game screens
- **State pattern** implementation for clean transitions
- **Stage director** manages lifecycle and transitions

**Stage Implementation Pattern:**
```c
// In stages/gameplay_stage.h
typedef struct {
    game_ptr game;
    // Stage-specific state
} gameplay_stage_state_t;

stage_ptr create_gameplay_stage_instance(void);
```

### 3. Simple Collision System
- **Direct AABB collision detection** (avoid overengineering)
- **Straightforward response handlers** 
- **Object pool iteration** for broad-phase detection

**Collision Pattern:**
```c
// collision_detection.h - Pure geometry
bool check_aabb_collision(float x1, float y1, float w1, float h1,
                         float x2, float y2, float w2, float h2);

// collision_handlers.h - Game-specific responses  
bool handle_player_enemy_collision(game_ptr game, player_ptr player, enemy_ptr enemy);

// collision_system.h - Main orchestration
void collision_system_update(game_ptr game);
```

### 4. Resource Management
- **Centralized loading** in resource managers
- **Sprite atlas** for efficient texture management
- **Audio context** with indexed sound effects
- **Font loading** with bitmap font support

## Code Quality Standards

### C99 Conventions
- **Strict compiler flags**: `-Wall -Wextra -pedantic-errors`
- **Consistent naming**: `snake_case` for functions, `UPPER_CASE` for constants
- **Header guards**: `#ifndef GAME_MODULE_NAME_H_`
- **Documentation**: Doxygen-style comments for all public functions
- **Static linkage**: Use `static` for internal functions

### Memory Management
- **Object pools** for frequently allocated/deallocated objects
- **Contiguous memory** allocation where possible
- **RAII-style cleanup** functions for each module
- **Null pointer validation** on all public APIs

### Error Handling
- **Boolean return values** for success/failure operations
- **Graceful degradation** when resources are unavailable
- **Defensive programming** with parameter validation

## Development Workflow

### Makefile Integration
The engine supports a complete development workflow:
```bash
make format    # clang-format with consistent LLVM style
make lint      # cppcheck static analysis with zero tolerance
make          # Build with strict error checking
make clean     # Clean build artifacts
make run       # Execute the game
```

### File Organization
- **Header-implementation pairs** for each module
- **Forward declarations** to minimize dependencies  
- **Include hierarchy**: Engine includes, then game includes
- **Consistent file naming** matching module purpose

## Game Development Patterns

### 1. Adding New Entity Types
```c
// 1. Create entity header in entities/
// 2. Define struct with position, state, pool support
// 3. Implement spawn/update/cleanup functions
// 4. Add to entity_factory for centralized creation
// 5. Update rendering system for visual representation
// 6. Add collision handling if needed
```

### 2. Implementing New Game Mechanics
```c
// 1. Identify if it's entity behavior, stage logic, or system feature
// 2. Use object pools for dynamic elements
// 3. Leverage event system for decoupled communication
// 4. Add appropriate collision responses
// 5. Update rendering and audio as needed
```

### 3. Creating New Game Screens
```c
// 1. Create new stage in stages/ directory
// 2. Implement stage interface (init, update, cleanup)
// 3. Register with stage director
// 4. Handle screen transitions via game_screen_t enum
// 5. Add stage-specific rendering logic
```

## Performance Considerations

### Memory Efficiency
- **Pre-allocate object pools** at startup
- **Avoid malloc/free** in game loop
- **Cache-friendly data layout** with contiguous arrays
- **Minimize pointer indirection**

### Rendering Performance  
- **Batch sprite rendering** from atlas
- **Minimize state changes** in rendering loop
- **Use logical coordinates** with scaling
- **Efficient culling** for off-screen entities

### Audio Performance
- **Pre-load sound effects** at initialization
- **Index-based sound triggering** for low latency
- **Avoid dynamic audio loading** during gameplay

## Testing & Quality Assurance

### Code Quality Gates
- **Zero lint warnings** - All code must pass cppcheck
- **Clean compilation** - No warnings with strict flags
- **Consistent formatting** - clang-format enforcement
- **Documentation coverage** - All public APIs documented

### Game Testing
- **Entity pool stress testing** - Verify no leaks/corruption
- **Collision system validation** - Test all interaction pairs
- **Stage transition testing** - Ensure clean state changes
- **Resource loading verification** - Handle missing assets gracefully

## Best Practices Summary

### Do:
- ✅ Use object pools for dynamic entities
- ✅ Keep collision system simple and direct
- ✅ Implement proper stage management
- ✅ Follow C99 conventions consistently
- ✅ Document all public APIs
- ✅ Use the engine's event system for decoupling
- ✅ Leverage sprite atlas for graphics
- ✅ Implement graceful error handling

### Avoid:
- ❌ Complex design patterns (Strategy, Observer, etc.) unless truly needed
- ❌ malloc/free in game loop
- ❌ Deep inheritance hierarchies
- ❌ Global state outside game struct
- ❌ Magic numbers (use named constants)
- ❌ Platform-specific code (keep cross-platform)
- ❌ Ignoring compiler warnings
- ❌ Missing null pointer checks

## Example: Adding a New Game

### 1. Project Setup
```bash
# Copy engine directory to new project
# Update Makefile paths for game source
# Initialize git repository
# Set up build workflow
```

### 2. Core Game Structure
```c
// Define game-specific entities in entities/
// Create main game loop in main/game.c
// Implement stages for different screens
// Set up collision handling
// Add rendering pipeline
// Configure audio assets
```

### 3. Development Iteration
```bash
# Use make format && make lint && make for each change
# Test entity pools under stress
# Verify collision accuracy
# Check stage transitions
# Validate resource loading
```

This agent definition provides a comprehensive foundation for developing new games using the established engine architecture and proven patterns from this project.