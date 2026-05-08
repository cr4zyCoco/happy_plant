#include <stdio.h>
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "buttons.h"
#include "dice.h"


void app_main(void)
{

    buttons_init();
    dice_init(led_strip);

    button_left_pressed(),
    button_right_pressed();

    bool cheat_mode = false;

    while (1) {

        if (button_right_pressed() ) {
            cheat_mode = true;

            dice_roll(cheat_mode);
        }

        if (button_left_pressed()) {

            dice_roll(cheat_mode);

            cheat_mode = false;
        }

        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

