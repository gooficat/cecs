#include "ecs.h"
#include <math.h>
#include <stdlib.h>


ecs_coordinator_s create_ecs_coordinator() {
    return (ecs_coordinator_s) {
        .signatures = malloc(sizeof(signature_t)),
        .num_entities = 0,
        .entity_array_size_multiplier = 1,
        
        .systems = malloc(sizeof(system_s)),
        .num_systems = 0,

        .component_lists = malloc(sizeof(component_list_s)),
        .num_component_lists = 0,
    };
}

entity_t add_entity(ecs_coordinator_s *c, signature_t signature) {
    ++c->num_entities;
    if (pow(2, c->entity_array_size_multiplier) < c->num_entities) {
        c->entity_array_size_multiplier *= 2;
        c->signatures = realloc(c->signatures, sizeof(signature_t) * c->entity_array_size_multiplier);
    }
    c->signatures[c->num_entities-1] = signature;
    return c->num_entities-1;
}

signature_t get_entity(ecs_coordinator_s *c, entity_t entity) {
    return c->signatures[entity];
}

void system_on_entity_removal(system_s *system, entity_t entity) {
    for (uint32_t i = 0; i < system->num_entities; i+=1) {
        if (system->entities[i] == entity) {
            system->entities[i] = system->entities[system->num_entities-1];
            system->entities = realloc(system->entities, sizeof(entity_t) * --system->num_entities);
            break;
        }
    }
}

void remove_entity(ecs_coordinator_s *c, entity_t entity) {
    for (int i = 0; i < c->num_systems; i++) {
        system_on_entity_removal(&c->systems[i], entity);
    }
    // signature_t sig = get_entity(c, entity);
    // for (int i = 0; i < c->num_component_lists; i++) {
    //     if (c->component_lists[i].signature & sig) {

    //     }
    // }

    c->signatures[entity] = 0;
}

component_list_id_t add_component_list(ecs_coordinator_s *c) {
    component_list_id_t new_component_list_id = c->num_component_lists;
    ++c->num_component_lists;
    c->component_lists = realloc(c->component_lists, c->num_component_lists * sizeof(component_list_s));
    c->component_lists[new_component_list_id] = (component_list_s) {
        .components = malloc(sizeof(component_list_s*)),
        .num_components = 0,
        .component_array_size_multiplier = 1
    };
    return new_component_list_id;
}

component_list_s* get_component_list(ecs_coordinator_s *c, component_list_id_t id) {
    return &c->component_lists[id];
}

component_id_t add_component(ecs_coordinator_s *c, component_list_id_t component_list_id, component_s *component) {
    component_list_s *cl = &c->component_lists[component_list_id];
    component_id_t cid = cl->num_components;
    ++cl->num_components;
    if (pow(2, cl->component_array_size_multiplier) < cl->num_components) {
        cl->component_array_size_multiplier *= 2;
        cl->components = realloc(cl->components, sizeof(component_s) * cl->component_array_size_multiplier);
    }
    cl->components[cid] = component;
    return cid;
}

component_s* get_component(ecs_coordinator_s *c, component_list_id_t component_list_id, component_id_t id) {
    return c->component_lists[component_list_id].components[id];
}

void remove_component(ecs_coordinator_s *c, component_list_id_t component_list_id, component_id_t id) {
    free(c->component_lists[component_list_id].components[id]);
    c->component_lists[component_list_id].components[id] = 0x0;
}

system_id_t add_system(ecs_coordinator_s *c) {
    c->systems = realloc(c->systems, sizeof(system_s) * ++c->num_systems);
    return c->num_systems - 1;
}


// void remove_system(ecs_coordinator_s *c, system_id_t id) {

// }