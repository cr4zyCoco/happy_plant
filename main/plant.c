#include <stdio.h>
#include "plant.h"
#include "ultrasonic.h"
#include "light_sensor.h"
#include "plant_faces.h"
#include "esp_timer.h"
#include "plant_clock.h"

/* ---------- Einstellbare Max-/Grenzwerte ---------- */

#define WATER_DISTANCE_FULL_CM      3.0f
#define WATER_DISTANCE_LOW_CM       5.0f
#define WATER_DISTANCE_CRITICAL_CM  6.0f

#define DAY_LIGHT_THRESHOLD         700
#define NIGHT_DARK_THRESHOLD       1500

#define HAPPINESS_LOVELY            80
#define HAPPINESS_HAPPY             60
#define HAPPINESS_SAD               40
#define HAPPINESS_JUDGE             20

void plant_toggle_mode(plant_t *plant)
{
    // Platzhalter, bis Blume/Kaktus-Profile fertig sind
}

/* ---------- Hilfsfunktionen ---------- */

static void plant_limit_values(plant_t *plant)
{
    if (plant->water > 100) plant->water = 100;
    if (plant->water < 0) plant->water = 0;

    if (plant->happiness > 100) plant->happiness = 100;
    if (plant->happiness < 0) plant->happiness = 0;

    if (plant->energy > 100) plant->energy = 100;
    if (plant->energy < 0) plant->energy = 0;
}

static void plant_update_water_from_distance(plant_t *plant, float distance_cm)
{
    if (distance_cm <= WATER_DISTANCE_FULL_CM)
    {
        plant->water = 100;
    }
    else if (distance_cm <= WATER_DISTANCE_LOW_CM)
    {
        plant->water = 40;
    }
    else if (distance_cm <= WATER_DISTANCE_CRITICAL_CM)
    {
        plant->water = 20;
    }
    else
    {
        plant->water = 0;
    }
}


/* ---------- Public Functions ---------- */

void plant_init(plant_t *plant)
{
    plant->water = 100;
    plant->happiness = 70;
    plant->energy = 100;

    plant->sun_minutes_today = 0;

    plant->state = PLANT_HAPPY;
    plant->need = NEED_NONE;
}

void plant_add_happiness(plant_t *plant, int amount)
{
    plant->happiness += amount;
    plant_limit_values(plant);
}

void plant_water(plant_t *plant)
{
    plant->water += 20;
    plant_limit_values(plant);
}

void plant_draw(plant_state_t state)
{
    plant_faces_draw(state);
}



void plant_update(plant_t *plant)
{
    float distance = ultrasonic_get_distance_cm();

    if (distance < 0)
    {
        plant->state = PLANT_SAD;
        return;
    }

    plant_update_water_from_distance(plant, distance);
    plant_limit_values(plant);

    int is_day = plant_clock_is_daytime();
    int light_raw = light_sensor_get_raw();

    bool enough_light = false;
    bool dark_enough = false;

    if (is_day)
    {
        // Tagsüber: kleiner Wert = hell
        enough_light = (light_raw < DAY_LIGHT_THRESHOLD);
    }
    else
    {
        // Nachts: großer Wert = dunkel
        dark_enough = (light_raw > NIGHT_DARK_THRESHOLD);
    }

    /* ---------- FSM ----------
       Priorität:
       1. Wasser
       2. Licht
       3. Zufriedenheit
    */

    // Wasser entscheidet zuerst

    if (plant->water <= 0)
    {
        plant->state = PLANT_DEAD;
        return;
    }

    if (plant->water <= 20)
    {
        plant->state = PLANT_JUDGE;
        return;
    }

    if (plant->water <= 40)
    {
        plant->state = PLANT_THIRSTY;
        return;
    }

    // Licht entscheidet danach

    if (is_day && !enough_light)
    {
        plant->state = PLANT_SAD;
        return;
    }

    if (!is_day && !dark_enough)
    {
        plant->state = PLANT_JUDGE;
        return;
    }

    // Zufriedenheit entscheidet zuletzt

    if (plant->happiness < HAPPINESS_JUDGE)
    {
        plant->state = PLANT_JUDGE;
        return;
    }

    if (plant->happiness < HAPPINESS_SAD)
    {
        plant->state = PLANT_SAD;
        return;
    }

    if (plant->happiness >= HAPPINESS_LOVELY)
    {
        plant->state = PLANT_LOVELY;
        return;
    }

    if (plant->happiness >= HAPPINESS_HAPPY)
    {
        plant->state = PLANT_HAPPY;
        return;
    }

    plant->state = PLANT_HAPPY;
}