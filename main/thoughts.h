#pragma once

#include <stdbool.h>
#include "plant.h"

void thoughts_start(plant_t *plant);
void thoughts_update(plant_t *plant);
bool thoughts_is_active(void);