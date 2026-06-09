#ifndef INTERACTION_H
#define INTERACTION_H

#include <stdbool.h>
#include "plant.h"

void interaction_init(void);
void interaction_update(plant_t *plant);
bool interaction_is_active(void);

#endif