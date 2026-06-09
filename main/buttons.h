#pragma once

#include <stdbool.h>

typedef enum
{
    BUTTON_NONE,
    BUTTON_CLICK,
    BUTTON_LONG_PRESS
} button_event_t;

void buttons_init(void);

bool button_pressed(void);

button_event_t button_event(void);