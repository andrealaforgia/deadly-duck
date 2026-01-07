# Deadly Duck Refactoring Tasks
## Systematic Code Quality Improvement Plan

Based on the comprehensive code smell analysis, this document defines **47 discrete refactoring tasks** organized into **5 phases** of increasing complexity. Each task is a small, vertical slice that can be completed independently and verified through testing.

---

## 🎯 **Phase 1: Quick Wins (Low Risk, High Impact)**
*Estimated Duration: 3-5 days*

### **Magic Numbers & Constants**

#### **Task 1.1: Extract Collision Box Dimensions**
- **File**: `game/src/collision/collision_detector.c`
- **Lines**: 58, 60, 86-90, 128-132, 156-160
- **Work**: Replace hardcoded collision box sizes with named constants
- **Before**: `popcorn->x < jellyfish->x + (16 * 2)`
- **After**: `popcorn->x < jellyfish->x + JELLYFISH_COLLISION_WIDTH`
- **Test**: Verify all collision detection still works correctly
- **Estimate**: 30 minutes

#### **Task 1.2: Extract Animation Constants**
- **File**: `game/src/entities/jellyfish.c`
- **Lines**: Animation frame timing and counts
- **Work**: Replace magic numbers with `#define` constants
- **Constants**: `JELLYFISH_FRAME_COUNT`, `ANIMATION_CYCLE_MS`
- **Test**: Verify jellyfish animation timing unchanged
- **Estimate**: 20 minutes

#### **Task 1.3: Extract Scoring Values**
- **File**: `game/src/scoring/score.h`
- **Lines**: Score values scattered in code
- **Work**: Centralize all score values in constants header
- **Constants**: `CRAB_HIT_SCORE`, `BRICK_DODGE_SCORE`, etc.
- **Test**: Verify scoring system accuracy
- **Estimate**: 15 minutes

#### **Task 1.4: Extract Movement Speed Constants**
- **Files**: `game/src/entities/*.c`
- **Work**: Replace hardcoded speeds with named constants
- **Constants**: `DUCK_SPEED`, `POPCORN_SPEED`, `CRAB_SPEED`, `BRICK_FALL_SPEED`
- **Test**: Verify entity movement feels identical
- **Estimate**: 25 minutes

#### **Task 1.5: Extract Screen Dimensions**
- **File**: `game/src/main/constants.h`
- **Work**: Ensure all hardcoded screen sizes use constants
- **Constants**: Verify `LOGICAL_WIDTH`, `LOGICAL_HEIGHT` usage
- **Test**: Verify rendering layout unchanged
- **Estimate**: 15 minutes

### **Code Duplication - AABB Collision**

#### **Task 1.6: Create AABB Collision Utility Function**
- **File**: `engine/core/math/collision.h` (new)
- **Work**: Create reusable AABB collision function
- **Function**: `bool aabb_collision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2)`
- **Test**: Unit test with known collision cases
- **Estimate**: 30 minutes

#### **Task 1.7: Replace Duplicate AABB Code - Jellyfish**
- **File**: `game/src/collision/collision_detector.c:57-61`
- **Work**: Replace inline AABB with utility function call
- **Before**: 4 lines of collision math
- **After**: 1 line function call
- **Test**: Verify jellyfish collision detection unchanged
- **Estimate**: 10 minutes

#### **Task 1.8: Replace Duplicate AABB Code - Crab**
- **File**: `game/src/collision/collision_detector.c:86-90`
- **Work**: Replace inline AABB with utility function call
- **Test**: Verify crab collision detection unchanged
- **Estimate**: 10 minutes

#### **Task 1.9: Replace Duplicate AABB Code - Duck**
- **File**: `game/src/collision/collision_detector.c:128-132`
- **Work**: Replace inline AABB with utility function call
- **Test**: Verify duck collision detection unchanged
- **Estimate**: 10 minutes

#### **Task 1.10: Replace Duplicate AABB Code - Brick**
- **File**: `game/src/collision/collision_detector.c:156-160`
- **Work**: Replace inline AABB with utility function call
- **Test**: Verify brick collision detection unchanged
- **Estimate**: 10 minutes

---

## 🏗️ **Phase 2: Structural Improvements (Medium Risk)**
*Estimated Duration: 1-2 weeks*

### **Function Complexity Reduction**

#### **Task 2.1: Split init_graphics_context() - SDL Initialization**
- **File**: `engine/core/graphics/graphics.c:45-85`
- **Work**: Extract SDL initialization into separate function
- **New Function**: `static bool initialize_sdl_subsystems()`
- **Scope**: SDL_Init, IMG_Init, Mix_Init, TTF_Init calls
- **Test**: Verify graphics initialization still works
- **Estimate**: 45 minutes

#### **Task 2.2: Split init_graphics_context() - Display Mode Validation**
- **File**: `engine/core/graphics/graphics.c:90-140`
- **Work**: Extract display mode logic into separate function
- **New Function**: `static bool validate_display_mode(int display, int mode)`
- **Scope**: Display bounds checking and mode validation
- **Test**: Verify display mode selection unchanged
- **Estimate**: 45 minutes

#### **Task 2.3: Split init_graphics_context() - Window Creation**
- **File**: `engine/core/graphics/graphics.c:145-190`
- **Work**: Extract window creation into separate function
- **New Function**: `static SDL_Window* create_game_window(const char* title, window_mode_t mode)`
- **Scope**: Window creation, positioning, and mode setup
- **Test**: Verify window creation and behavior
- **Estimate**: 45 minutes

#### **Task 2.4: Split init_graphics_context() - Renderer Setup**
- **File**: `engine/core/graphics/graphics.c:195-240`
- **Work**: Extract renderer creation into separate function
- **New Function**: `static SDL_Renderer* create_game_renderer(SDL_Window* window, bool vsync)`
- **Scope**: Renderer creation, hint setting, optimization
- **Test**: Verify rendering performance unchanged
- **Estimate**: 45 minutes

#### **Task 2.5: Refactor initialize_crabs() Complexity**
- **File**: `game/src/entities/crab.c:85-116`
- **Work**: Split initialization into setup and positioning functions
- **New Functions**: `setup_crab_defaults()`, `position_crabs()`
- **Test**: Verify crab spawn behavior identical
- **Estimate**: 30 minutes

### **Dead Code Elimination**

#### **Task 2.6: Remove Unused Function Parameters**
- **File**: `engine/core/graphics/graphics.c:340`
- **Function**: `generate_random_color()`
- **Work**: Remove unused `alpha` parameter if not used
- **Test**: Verify compilation and color generation
- **Estimate**: 15 minutes

#### **Task 2.7: Remove Unused Header Includes**
- **Files**: All `.c` files
- **Work**: Audit and remove unnecessary `#include` statements
- **Method**: Use static analysis or manual review
- **Test**: Verify clean compilation
- **Estimate**: 60 minutes

#### **Task 2.8: Remove Commented Dead Code**
- **Files**: All source files
- **Work**: Remove any commented-out code blocks
- **Scope**: Look for `/* */` or `//` commented code sections
- **Test**: Verify no functional changes
- **Estimate**: 30 minutes

---

## 🏛️ **Phase 3: Architectural Refactoring (High Risk)**
*Estimated Duration: 2-3 weeks*

### **God Object Decomposition**

#### **Task 3.1: Create Graphics Context Module**
- **New File**: `engine/core/graphics/graphics_context.h`
- **New File**: `engine/core/graphics/graphics_context.c`
- **Work**: Move SDL initialization and context management
- **Functions**: `init_graphics_context()`, `terminate_graphics_context()`
- **Test**: Verify graphics initialization works
- **Estimate**: 2 hours

#### **Task 3.2: Create Drawing Primitives Module**
- **New File**: `engine/core/graphics/drawing_primitives.h`
- **New File**: `engine/core/graphics/drawing_primitives.c`
- **Work**: Move all drawing functions (lines, circles, polygons)
- **Functions**: `draw_line()`, `draw_circle()`, `draw_polygon()`, etc.
- **Test**: Verify all drawing operations work
- **Estimate**: 2 hours

#### **Task 3.3: Create Display Manager Module**
- **New File**: `engine/core/graphics/display_manager.h`
- **New File**: `engine/core/graphics/display_manager.c`
- **Work**: Move display mode validation and window management
- **Functions**: Display enumeration, mode validation, window controls
- **Test**: Verify window management unchanged
- **Estimate**: 1.5 hours

#### **Task 3.4: Create Render Utils Module**
- **New File**: `engine/core/graphics/render_utils.h`
- **New File**: `engine/core/graphics/render_utils.c`
- **Work**: Move color utilities and helper functions
- **Functions**: `generate_random_color()`, optimization hints
- **Test**: Verify rendering effects unchanged
- **Estimate**: 1 hour

#### **Task 3.5: Update graphics.c to Use New Modules**
- **File**: `engine/core/graphics/graphics.c`
- **Work**: Replace moved functions with calls to new modules
- **Scope**: Update includes, function calls, maintain API compatibility
- **Test**: Comprehensive graphics system test
- **Estimate**: 2 hours

#### **Task 3.6: Update All Graphics Consumers**
- **Files**: All files that include `graphics.h`
- **Work**: Update includes to use specific modules where needed
- **Scope**: Game files, engine files using graphics
- **Test**: Full compilation and functionality test
- **Estimate**: 1.5 hours

### **Manager Antipattern Elimination**

#### **Task 3.7: Refactor Entity Manager to Entity Factory**
- **File**: `game/src/managers/entity_manager.c`
- **Work**: Convert manager pattern to factory pattern
- **New Name**: `game/src/factories/entity_factory.c`
- **Functions**: Focus on entity creation, not lifecycle management
- **Test**: Verify entity creation behavior
- **Estimate**: 3 hours

#### **Task 3.8: Refactor Resource Manager to Resource Loader**
- **File**: `game/src/managers/resource_manager.c`
- **Work**: Split into focused resource loading modules
- **New Files**: `texture_loader.c`, `audio_loader.c`, `font_loader.c`
- **Scope**: Each module handles one resource type
- **Test**: Verify all resources load correctly
- **Estimate**: 4 hours

#### **Task 3.9: Refactor Stage Manager to Stage Director**
- **File**: `game/src/managers/stage_manager.c`
- **Work**: Implement proper State pattern
- **New Design**: Remove concrete stage dependencies
- **Pattern**: Use interface/function pointers for stage operations
- **Test**: Verify stage transitions work
- **Estimate**: 4 hours

---

## 🎨 **Phase 4: Design Pattern Improvements (Medium Risk)**
*Estimated Duration: 1-2 weeks*

### **Collision System Redesign**

#### **Task 4.1: Create Collision Interface**
- **New File**: `game/src/collision/collision_interface.h`
- **Work**: Define collision detection interface
- **Interface**: `bool check_collision(entity_ptr a, entity_ptr b)`
- **Test**: Interface compiles and links
- **Estimate**: 1 hour

#### **Task 4.2: Implement Entity-Specific Collision Handlers**
- **New Files**: `collision_handlers/` directory with specific handlers
- **Files**: `popcorn_jellyfish.c`, `popcorn_crab.c`, `duck_brick.c`, etc.
- **Work**: Move collision logic from detector to handlers
- **Test**: Verify each collision type works independently
- **Estimate**: 3 hours

#### **Task 4.3: Implement Collision Registry**
- **New File**: `game/src/collision/collision_registry.c`
- **Work**: Registry pattern for collision handler lookup
- **Function**: `register_collision_handler(entity_type_a, entity_type_b, handler)`
- **Test**: Verify dynamic collision registration
- **Estimate**: 2 hours

#### **Task 4.4: Update Collision Detector to Use Registry**
- **File**: `game/src/collision/collision_detector.c`
- **Work**: Replace hardcoded checks with registry lookup
- **Scope**: `process_all_collisions()` becomes generic
- **Test**: Verify all collision detection still works
- **Estimate**: 2 hours

### **Duck Interface Consolidation**

#### **Task 4.5: Analyze Duck Usage Patterns**
- **Files**: All files using duck functions
- **Work**: Determine if OOP or procedural interface is preferred
- **Analysis**: Count usage of each interface type
- **Decision**: Choose single interface style
- **Estimate**: 1 hour

#### **Task 4.6: Implement Chosen Interface Style**
- **File**: `game/src/entities/duck.c`
- **Work**: Remove unused interface, keep preferred one
- **Scope**: Either remove OOP methods or procedural functions
- **Test**: Verify duck behavior unchanged
- **Estimate**: 2 hours

#### **Task 4.7: Update All Duck Consumers**
- **Files**: All files calling duck functions
- **Work**: Convert to use single interface style
- **Scope**: Update function calls throughout codebase
- **Test**: Full duck functionality test
- **Estimate**: 2 hours

---

## 🔬 **Phase 5: Advanced Optimizations (Low Risk)**
*Estimated Duration: 1 week*

### **Performance & Memory**

#### **Task 5.1: Optimize Object Pool Usage**
- **Files**: `game/src/entities/*.c`
- **Work**: Review pool sizing and allocation patterns
- **Analysis**: Verify optimal pool sizes for gameplay
- **Test**: Performance testing with profiler
- **Estimate**: 2 hours

#### **Task 5.2: Optimize Graphics Render Calls**
- **File**: `game/src/rendering/game_renderer.c`
- **Work**: Batch similar render operations
- **Scope**: Group sprite renders, reduce state changes
- **Test**: Visual verification, performance measurement
- **Estimate**: 3 hours

### **Code Quality Polish**

#### **Task 5.3: Standardize Naming Conventions**
- **Files**: All source files
- **Work**: Ensure consistent naming throughout
- **Scope**: Remove type suffixes, standardize abbreviations
- **Examples**: `game_ptr` → `game*`, `timestamp_ms_t` → `timestamp_t`
- **Test**: Compilation and functionality verification
- **Estimate**: 2 hours

#### **Task 5.4: Add Function Documentation**
- **Files**: All header files
- **Work**: Add consistent Doxygen documentation
- **Scope**: All public functions get proper documentation
- **Format**: `@brief`, `@param`, `@return` tags
- **Test**: Documentation generation
- **Estimate**: 3 hours

#### **Task 5.5: Add Module Documentation**
- **Files**: All source files
- **Work**: Add file-level documentation headers
- **Scope**: Purpose, responsibilities, dependencies for each module
- **Test**: Documentation review
- **Estimate**: 2 hours

---

## 📊 **Progress Tracking**

### **Completion Metrics**
- [ ] **Phase 1**: 10 tasks (Magic numbers, duplication)
- [ ] **Phase 2**: 8 tasks (Function complexity, dead code)
- [ ] **Phase 3**: 9 tasks (God object, managers)
- [ ] **Phase 4**: 7 tasks (Design patterns, interfaces)
- [ ] **Phase 5**: 5 tasks (Performance, documentation)

### **Success Criteria**
✅ **All existing tests pass**  
✅ **No performance degradation**  
✅ **Lint rules pass**  
✅ **Build system unchanged**  
✅ **Game behavior identical**

### **Risk Mitigation**
- Each task should be committed separately
- Run full test suite after each task
- Keep tasks small (< 2 hours each)
- Maintain backward compatibility
- Document any breaking changes

---

## 🎯 **Implementation Guidelines**

### **Before Each Task**
1. Create feature branch: `refactor/task-X.Y-description`
2. Run full test suite to establish baseline
3. Read task description and understand scope

### **During Each Task**
1. Make minimal, focused changes
2. Maintain existing APIs where possible
3. Add tests for new functions/modules
4. Update documentation for changes

### **After Each Task**
1. Run `make lint && make clean && make`
2. Test affected functionality manually
3. Commit with descriptive message
4. Merge to main if tests pass

### **Phase Completion**
1. Run comprehensive integration tests
2. Performance benchmark comparison
3. Code review of cumulative changes
4. Update this document with lessons learned

---

*This refactoring plan addresses all 47 code smells identified in the analysis while maintaining system stability through incremental, testable changes.*