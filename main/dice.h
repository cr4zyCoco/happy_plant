#ifndef DICE_H
#define DICE_H

#include "led_strip.h"

void dice_init(led_strip_handle_t strip);
void dice_roll(bool cheat_mode);

#endif