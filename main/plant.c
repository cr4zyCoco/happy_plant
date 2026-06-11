#include <stdio.h>
#include "plant.h"
#include "ultrasonic.h"
#include "light_sensor.h"
#include "plant_faces.h"

void plant_toggle_mode(plant_t *plant)
{
    if (plant->mode == PLANT_MODE_FLOWER)
    {
        plant->mode = PLANT_MODE_CACTUS;
    }
    else
    {
        plant->mode = PLANT_MODE_FLOWER;
    }
}

void plant_init(plant_t *plant)
{
    plant->water = 100;
    plant->happiness = 100;
    plant->energy = 100;

    plant->state = PLANT_HAPPY;
    plant->need = NEED_NONE;
    plant->age_days = 0;
    plant->death_recorded = false;
    plant->death_day = 0;
}

static void plant_limit_values(plant_t *plant)
{
    if (plant->water > 100) plant->water = 100;
    if (plant->water < 0) plant->water = 0;

    if (plant->happiness > 100) plant->happiness = 100;
    if (plant->happiness < 0) plant->happiness = 0;

    if (plant->energy > 100) plant->energy = 100;
    if (plant->energy < 0) plant->energy = 0;
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
    float distance = 12;//ultrasonic_get_distance_cm();
    int is_dark = 0;//light_sensor_is_dark();

    if (distance < 0)
    {
        plant->state = PLANT_SAD;
        plant->need = NEED_CRITICAL;
        return;
    }

    if (distance >= 12)
        plant->water = 0;
    else if (distance > 10)
        plant->water = 20;
    else if (distance > 6)
        plant->water = 50;
    else
        plant->water = 100;

    
    if (plant->water <= 0)
{
    plant->state = PLANT_DEAD;
    plant->need = NEED_WATER;

    if (!plant->death_recorded)
    {
        plant->death_recorded = true;
        plant->death_day = plant->age_days;
    }

    return;
}

    if (is_dark)
        plant->energy -= 1;
    else
        plant->energy += 1;

    if (plant->energy > 100) plant->energy = 100;
    if (plant->energy < 0) plant->energy = 0;

    if (plant->water <= 0)
    {
        plant->state = PLANT_DEAD;
        plant->need = NEED_WATER;
    }
    else if (plant->water < 40)
    {
        plant->state = PLANT_THIRSTY;
        plant->need = NEED_WATER;
    }
    else if (plant->energy < 40)
    {
        plant->state = PLANT_SAD;
        plant->need = NEED_LIGHT;
    }
    else if (plant->happiness < 40)
    {
        plant->state = PLANT_JUDGE;
        plant->need = NEED_HAPPINESS;
    }
    else
    {
        plant->state = PLANT_HAPPY;
        plant->need = NEED_NONE;
    }
}