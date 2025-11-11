#pragma once

#include <stdint.h>

typedef uint32_t entity_t;
typedef uint32_t signature_t;
typedef uint8_t component_id_t;

typedef uint8_t component_list_id_t;
typedef uint8_t system_id_t;


typedef struct _component {
    signature_t signature;
} component_s;


typedef struct _component_list {
    component_s **components;
    uint32_t num_components;
    uint8_t component_array_size_multiplier;
} component_list_s;

typedef struct _system {
    entity_t *entities;
    uint32_t num_entities;
    void (*system_remove_entity_function)(entity_t);
} system_s;

typedef void (*entity_removal_function)(entity_t entity);

typedef struct _ecs_coordinator {
    signature_t *signatures;
    uint32_t num_entities;
    uint8_t entity_array_size_multiplier;

    system_s *systems;
    uint8_t num_systems;
    
    component_list_s *component_lists;
    uint8_t num_component_lists;
} ecs_coordinator_s;

ecs_coordinator_s create_ecs_coordinator();

entity_t add_entity(ecs_coordinator_s *coordinator, signature_t signature);
signature_t get_entity(ecs_coordinator_s *coordinator, entity_t entity);
void remove_entity(ecs_coordinator_s *coordinator, entity_t entity);

component_list_id_t add_component_list(ecs_coordinator_s *coordinator);
component_list_s* get_component_list(ecs_coordinator_s *coordinator, component_list_id_t id);
// void remove_component_list(ecs_coordinator_s *coordinator, component_list_id_t component_list_id);

component_id_t add_component(ecs_coordinator_s *c, component_list_id_t component_list_id, component_s *component);
component_s* get_component(ecs_coordinator_s *coordinator, component_list_id_t component_list_id, component_id_t id);
void remove_component(ecs_coordinator_s *coordinator, component_list_id_t component_list_id, component_id_t id);

system_id_t add_system(ecs_coordinator_s *coordinator);
// void remove_system(ecs_coordinator_s *coordinator, system_id_t id);