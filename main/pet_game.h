#pragma once

#include <stdbool.h>
#include "plant.h"

void pet_game_start(void);
void pet_game_update(plant_t *plant);
bool pet_game_is_active(void);