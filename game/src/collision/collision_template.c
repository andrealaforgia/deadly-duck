/**
 * @file collision_template.c
 * @brief Template Method pattern implementation for collision processing
 */

#include "collision_template.h"
#include "collision_observer.h"

void collision_processor_init(collision_processor_t* processor, collision_strategy_t* strategy, const char* name) {
    processor->process_collision = collision_processor_template_method;
    processor->pre_process = collision_processor_default_pre_process;
    processor->post_process = collision_processor_default_post_process;
    processor->strategy = strategy;
    processor->name = name;
    
    // Derived classes must set detect_collision and respond_collision
    processor->detect_collision = NULL;
    processor->respond_collision = NULL;
}

bool collision_processor_template_method(collision_processor_t* self, game_ptr game, entity_ptr entity_a, entity_ptr entity_b) {
    collision_subject_t* subject = get_collision_subject();
    
    // Template Method Algorithm:
    // 1. Pre-processing (validation, filtering)
    if (!self->pre_process(self, entity_a, entity_b)) {
        return false;
    }
    
    // 2. Collision detection (must be implemented by concrete classes)
    if (!self->detect_collision) {
        return false;
    }
    collision_result_t result = self->detect_collision(self, entity_a, entity_b);
    
    // 3. Early exit if no collision
    if (!result.collision_detected) {
        return false;
    }
    
    // 4. Notify observers of collision detection
    if (subject) {
        collision_event_data_t event = collision_event_create(
            COLLISION_EVENT_DETECTED,
            entity_a, entity_b, result, game, self->name
        );
        collision_subject_notify(subject, &event);
    }
    
    // 5. Collision response (must be implemented by concrete classes)
    if (self->respond_collision) {
        self->respond_collision(self, game, entity_a, entity_b, result);
    }
    
    // 6. Notify observers of collision resolution
    if (subject) {
        collision_event_data_t event = collision_event_create(
            COLLISION_EVENT_RESOLVED,
            entity_a, entity_b, result, game, self->name
        );
        collision_subject_notify(subject, &event);
    }
    
    // 7. Notify observers of entity destruction
    if (subject && (result.entity_a_destroyed || result.entity_b_destroyed)) {
        collision_event_data_t event = collision_event_create(
            COLLISION_EVENT_ENTITY_DESTROYED,
            entity_a, entity_b, result, game, self->name
        );
        collision_subject_notify(subject, &event);
    }
    
    // 8. Post-processing (cleanup, logging, etc.)
    self->post_process(self, game, entity_a, entity_b, result);
    
    return true;
}

bool collision_processor_default_pre_process(collision_processor_t* self, entity_ptr entity_a, entity_ptr entity_b) {
    (void)self; // Unused parameter
    
    // Default validation: ensure entities are valid
    return entity_a != NULL && entity_b != NULL;
}

void collision_processor_default_post_process(collision_processor_t* self, game_ptr game, entity_ptr entity_a, entity_ptr entity_b, collision_result_t result) {
    // Default implementation: no-op
    (void)self;
    (void)game;
    (void)entity_a;
    (void)entity_b;
    (void)result;
}
