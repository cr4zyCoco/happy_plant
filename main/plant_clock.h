#pragma once

#include <stdbool.h>

void plant_clock_init(void);
void plant_clock_update(void);

bool plant_clock_is_set(void);

int plant_clock_get_hour(void);
int plant_clock_get_minute(void);

bool plant_clock_is_daytime(void);

void plant_clock_draw_setup(void);
void plant_clock_handle_input(void);