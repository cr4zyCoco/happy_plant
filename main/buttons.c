#include "buttons.h"
#include "driver/gpio.h"

// Buttonpins
#define BUTTON_LEFT  9
#define BUTTON_RIGHT 2

void buttons_init(void)
{
    
    gpio_set_direction(BUTTON_LEFT, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_LEFT, GPIO_PULLUP_ONLY);

    gpio_set_direction(BUTTON_RIGHT, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_RIGHT, GPIO_PULLUP_ONLY);
}

//  Pullup --> gedrückt = 0
bool button_left_pressed(void)
{
    return gpio_get_level(BUTTON_LEFT) == 0;
}

bool button_right_pressed(void)
{
    return gpio_get_level(BUTTON_RIGHT) == 0;
}