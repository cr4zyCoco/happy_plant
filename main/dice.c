#include "dice.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdlib.h>

static led_strip_handle_t led_strip;

// Bitmaske für 25 LEDs (5x5)
static uint32_t active_mask = 0;

// Zufallsfarbe erzeugen
static void random_color(uint8_t *r, uint8_t *g, uint8_t *b)
{
    // Basisfarben nicht niedrig (damit keine grellen Farben entstehen)
    *r = rand() % 40;
    *g = rand() % 40;
    *b = rand() % 40;

    // Eine Farbe  stärker machen (knalligere Farbe)
    //int boost = rand() % 3;

    //if (boost == 0) *r += 80;
    //if (boost == 1) *g += 80;
    //if (boost == 2) *b += 80;
}

// Übergabe des LED-Handles
void dice_init(led_strip_handle_t strip)
{
    led_strip = strip;
}

static void noise(void)
{
    active_mask = 0;

    for (int i = 0; i < 8; i++) {
        int rnd = rand() % 25; // 0–24
        active_mask |= (1 << rnd);
    }
}

static void show_mask(void)
{
    led_strip_clear(led_strip);

    for (int i = 0; i < 25; i++) {

        if (active_mask & (1 << i)) {

            // Farbe
            uint8_t r, g, b;
            random_color(&r, &g, &b);

            led_strip_set_pixel(led_strip, i, r, g, b);
        }
    }

    led_strip_refresh(led_strip);
}

static void noise_phase(void)
{
    for (int t = 0; t < 50; t++) { // 50 * 100ms = 5s
        noise();
        show_mask();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

static void fade_out(void)
{
    while (active_mask != 0) {

        for (int i = 0; i < 2; i++) {

            int rnd = rand() % 25;

            // Bit löschen
            active_mask &= ~(1 << rnd);
        }

        show_mask();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

static const uint32_t dice_patterns[6] = {
    (1 << 12), // 1 (Mitte)

    (1 << 4) | (1 << 20), // 2

    (1 << 4) | (1 << 12) | (1 << 20), // 3

    (1 << 0) | (1 << 4) | (1 << 20) | (1 << 24), // 4

    (1 << 0) | (1 << 4) | (1 << 12) | (1 << 20) | (1 << 24), // 5

    (1 << 0) | (1 << 4) | (1 << 10) | (1 << 14) | (1 << 20) | (1 << 24) // 6
};

static void show_number(int num)
{
    active_mask = dice_patterns[num - 1];

    led_strip_clear(led_strip);

    // zufällige EINHEITLICHE Farbe für das Ergebnis
    uint8_t r, g, b;
    random_color(&r, &g, &b);

    for (int i = 0; i < 25; i++) {

        if (active_mask & (1 << i)) {

            led_strip_set_pixel(led_strip, i, r, g, b);
        }
    }

    led_strip_refresh(led_strip);
}

void dice_roll(bool cheat_mode)
{
    noise_phase();   // 5 Sekunden Rauschen
    fade_out();      // nach und nach ausblenden

    int result;

    if (cheat_mode) {
        result = 6;
    } else {
        result = (rand() % 6) + 1;
    }

    show_number(result);
}