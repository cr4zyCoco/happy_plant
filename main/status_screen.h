#pragma once

#include <stdbool.h>
#include "plant.h"

void status_screen_start(void);
void status_screen_update(plant_t *plant);
bool status_screen_is_active(void);