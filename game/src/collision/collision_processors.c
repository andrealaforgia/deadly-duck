/**
 * @file collision_processors.c
 * @brief Concrete collision processors implementation
 */

#include "collision_processors.h"
#include "collision_observer.h"
#include <stdlib.h>
#include <string.h>

// Forward declarations of processor-specific methods
static collision_result_t strategy_detect_collision(collision_processor_t* self, entity_ptr entity_a, entity_ptr entity_b);
static void strategy_respond_collision(collision_processor_t* self, game_ptr game, entity_ptr entity_a, entity_ptr entity_b, collision_result_t result);

static collision_result_t pool_detect_collision(collision_processor_t* self, entity_ptr entity_a, entity_ptr entity_b);
static void pool_respond_collision(collision_processor_t* self, game_ptr game, entity_ptr entity_a, entity_ptr entity_b, collision_result_t result);
static void pool_post_process(collision_processor_t* self, game_ptr game, entity_ptr entity_a, entity_ptr entity_b, collision_result_t result);

static bool filtered_pre_process(collision_processor_t* self, entity_ptr entity_a, entity_ptr entity_b);

// Strategy-based collision processor implementation
strategy_collision_processor_t* create_strategy_processor(collision_strategy_t* strategy, const char* name) {
    strategy_collision_processor_t* processor = malloc(sizeof(strategy_collision_processor_t));
    if (!processor) return NULL;
    
    collision_processor_init(&processor->base, strategy, name);
    processor->base.detect_collision = strategy_detect_collision;
    processor->base.respond_collision = strategy_respond_collision;
    
    return processor;
}

static collision_result_t strategy_detect_collision(collision_processor_t* self, entity_ptr entity_a, entity_ptr entity_b) {
    if (!self->strategy || !self->strategy->check) {
        return no_collision_result();
    }
    
    return self->strategy->check(entity_a, entity_b);
}

static void strategy_respond_collision(collision_processor_t* self, game_ptr game, entity_ptr entity_a, entity_ptr entity_b, collision_result_t result) {
    if (self->strategy && self->strategy->respond) {
        self->strategy->respond(game, entity_a, entity_b, result);
    }
}

// Pool-managed collision processor implementation
pool_collision_processor_t* create_pool_processor(collision_strategy_t* strategy, 
                                                 object_pool_t* pool_a, 
                                                 object_pool_t* pool_b,
                                                 const char* name) {
    pool_collision_processor_t* processor = malloc(sizeof(pool_collision_processor_t));
    if (!processor) return NULL;
    
    collision_processor_init(&processor->base, strategy, name);
    processor->base.detect_collision = pool_detect_collision;
    processor->base.respond_collision = pool_respond_collision;
    processor->base.post_process = pool_post_process;
    
    processor->pool_a = pool_a;
    processor->pool_b = pool_b;
    processor->index_a = 0;
    processor->index_b = 0;
    
    return processor;
}

static collision_result_t pool_detect_collision(collision_processor_t* self, entity_ptr entity_a, entity_ptr entity_b) {
    if (!self->strategy || !self->strategy->check) {
        return no_collision_result();
    }
    
    return self->strategy->check(entity_a, entity_b);
}

static void pool_respond_collision(collision_processor_t* self, game_ptr game, entity_ptr entity_a, entity_ptr entity_b, collision_result_t result) {
    if (self->strategy && self->strategy->respond) {
        self->strategy->respond(game, entity_a, entity_b, result);
    }
}

static void pool_post_process(collision_processor_t* self, game_ptr game, entity_ptr entity_a, entity_ptr entity_b, collision_result_t result) {
    pool_collision_processor_t* pool_proc = (pool_collision_processor_t*)self;
    collision_subject_t* subject = get_collision_subject();
    
    // Handle pool cleanup for destroyed entities
    if (result.entity_a_destroyed && pool_proc->pool_a) {
        pool_release(pool_proc->pool_a, pool_proc->index_a);
        
        // Notify observers of pool release
        if (subject) {
            collision_event_data_t event = collision_event_create(
                COLLISION_EVENT_POOL_RELEASED,
                entity_a, entity_b, result, game, self->name
            );
            collision_subject_notify(subject, &event);
        }
    }
    
    if (result.entity_b_destroyed && pool_proc->pool_b) {
        pool_release(pool_proc->pool_b, pool_proc->index_b);
        
        // Notify observers of pool release
        if (subject) {
            collision_event_data_t event = collision_event_create(
                COLLISION_EVENT_POOL_RELEASED,
                entity_a, entity_b, result, game, self->name
            );
            collision_subject_notify(subject, &event);
        }
    }
}

// Filtered collision processor implementation
filtered_collision_processor_t* create_filtered_processor(collision_strategy_t* strategy,
                                                         bool (*filter)(entity_ptr),
                                                         const char* name) {
    filtered_collision_processor_t* processor = malloc(sizeof(filtered_collision_processor_t));
    if (!processor) return NULL;
    
    collision_processor_init(&processor->base, strategy, name);
    processor->base.detect_collision = strategy_detect_collision;
    processor->base.respond_collision = strategy_respond_collision;
    processor->base.pre_process = filtered_pre_process;
    
    processor->entity_filter = filter;
    
    return processor;
}

static bool filtered_pre_process(collision_processor_t* self, entity_ptr entity_a, entity_ptr entity_b) {
    filtered_collision_processor_t* filtered_proc = (filtered_collision_processor_t*)self;
    
    // First run default validation
    if (!collision_processor_default_pre_process(self, entity_a, entity_b)) {
        return false;
    }
    
    // Then apply custom filter if provided
    if (filtered_proc->entity_filter) {
        return filtered_proc->entity_filter(entity_a) && filtered_proc->entity_filter(entity_b);
    }
    
    return true;
}

// Generic cleanup function
void destroy_collision_processor(collision_processor_t* processor) {
    if (processor) {
        free(processor);
    }
}

// Entity filters implementation
bool active_popcorn_filter(entity_ptr entity) {
    popcorn_ptr popcorn = (popcorn_ptr)entity;
    return popcorn && popcorn->active;
}

bool living_duck_filter(entity_ptr entity) {
    duck_ptr duck = (duck_ptr)entity;
    return duck && !duck->dead;
}

bool alive_crab_filter(entity_ptr entity) {
    crab_ptr crab = (crab_ptr)entity;
    return crab && crab->alive;
}

bool falling_brick_filter(entity_ptr entity) {
    brick_ptr brick = (brick_ptr)entity;
    return brick && brick->active && !brick->landed;
}

bool landed_brick_filter(entity_ptr entity) {
    brick_ptr brick = (brick_ptr)entity;
    return brick && brick->landed;
}
