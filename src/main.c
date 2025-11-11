#include <stdio.h>
#include <stdlib.h>

#include "ecs.h"

static ecs_coordinator_s w_c;

struct feline_system {
    entity_t *entities;
    uint32_t num_entities;
    entity_removal_function on_entity_removal;
} feline_system;

struct feline_component {
    signature_t signature;
    char *message;
};

int main() {
    w_c = create_ecs_coordinator();

    entity_t cat = add_entity(&w_c, 0b10000000000000000000000000000000);
    

    component_list_id_t feline_component_list = add_component_list(&w_c);

    struct feline_component cat_component = {
        .signature = 0b10000000000000000000000000000000,
        .message = "Meow!"
    };

    component_id_t cat_feline_component_id =  add_component(&w_c, feline_component_list, (component_s*)&cat_component);

    printf("%s",
            (
                (struct feline_component*)
                (get_component(&w_c, feline_component_list, cat_feline_component_id)
            )
        )->message);

    printf("Hello, World!\n");

    return 0;
}
