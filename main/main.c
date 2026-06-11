#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_random.h"

#include "i2c.h"
#include "display.h"
#include "buttons.h"
#include "plant.h"
#include "ultrasonic.h"
#include "light_sensor.h"
#include "interaction.h"
#include "pet_game.h"
#include "play_game.h"
#include "thoughts.h"

plant_t plant;

void app_main(void)
{
    i2c_init();
    buttons_init();
    ssd1306_init();
    ultrasonic_init();
    interaction_init();
    light_sensor_init();
    plant_init(&plant);
    
    while (1)
{
    if (pet_game_is_active())
    {
        pet_game_update(&plant);
    }
    else if (play_game_is_active())
    {
        play_game_update(&plant);
    }
    else if (thoughts_is_active())
    {
        thoughts_update(&plant);
    }
    else
    {
        interaction_update(&plant);

        if (!interaction_is_active())
        {
            plant_update(&plant);
            plant_draw(plant.state);
        }
    }

    vTaskDelay(pdMS_TO_TICKS(50));
}
}