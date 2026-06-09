#pragma once

#include <stdbool.h>
#include "plant.h"

void play_game_start(void);
void play_game_update(plant_t *plant);
bool play_game_is_active(void);