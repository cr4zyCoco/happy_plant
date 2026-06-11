#include "buttons.h"

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BUTTON GPIO_NUM_1

static bool last_state = false;
static TickType_t press_start = 0;

void buttons_init(void)
{
    gpio_set_direction(BUTTON, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON, GPIO_PULLUP_ONLY);
}

bool button_pressed(void)
{
    return gpio_get_level(BUTTON) == 0;
}

button_event_t button_event(void)
{
    bool current = button_pressed();

    // Taste wurde gedrückt
    if (current && !last_state)
    {
        press_start = xTaskGetTickCount();
    }

    // Taste wurde losgelassen
    if (!current && last_state)
    {
        TickType_t duration =
            xTaskGetTickCount() - press_start;

        last_state = current;

        if (duration >= pdMS_TO_TICKS(1000))
        {
            return BUTTON_LONG_PRESS;
        }
        else
        {
            return BUTTON_CLICK;
        }
    }

    last_state = current;

    return BUTTON_NONE;
}