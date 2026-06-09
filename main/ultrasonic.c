#include "ultrasonic.h"
#include "esp_timer.h"
#include "esp_rom_sys.h"
#include "driver/gpio.h"

#define TRIG_PIN GPIO_NUM_9
#define ECHO_PIN GPIO_NUM_2

static gpio_num_t trig = TRIG_PIN;
static gpio_num_t echo = ECHO_PIN;

void ultrasonic_init(void)
{
    gpio_config_t trig_conf = {
        .pin_bit_mask = 1ULL << trig,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&trig_conf);

    gpio_config_t echo_conf = {
        .pin_bit_mask = 1ULL << echo,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&echo_conf);

    gpio_set_level(trig, 0);
}

float ultrasonic_get_distance_cm(void)
{
    int64_t timeout;
    int64_t start_time;
    int64_t end_time;

    gpio_set_level(trig, 0);
    esp_rom_delay_us(2);

    gpio_set_level(trig, 1);
    esp_rom_delay_us(10);
    gpio_set_level(trig, 0);

    timeout = esp_timer_get_time() + 30000;
    while (gpio_get_level(echo) == 0) {
        if (esp_timer_get_time() > timeout) {
            return -1.0f;
        }
    }

    start_time = esp_timer_get_time();

    timeout = esp_timer_get_time() + 30000;
    while (gpio_get_level(echo) == 1) {
        if (esp_timer_get_time() > timeout) {
            return -1.0f;
        }
    }

    end_time = esp_timer_get_time();

    return (end_time - start_time) / 58.0f;
}