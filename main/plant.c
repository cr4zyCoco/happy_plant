#include <stdio.h>
#include "plant.h"
#include "ultrasonic.h"
#include "light_sensor.h"
#include "plant_faces.h"

void plant_init(plant_t *plant)
{
    plant->water = 25;
    plant->happiness = 50;
    plant->energy = 70;
    plant->state = PLANT_HAPPY;
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
    //float distance = ultrasonic_get_distance_cm();
    int is_dark = light_sensor_is_dark();
    float distance = 5;


    if (distance < 0)
    {
        plant->state = PLANT_SAD;
        return;
    }

    if (is_dark)
    {
        plant->energy -= 1;
    }
    else
    {
        plant->energy += 1;
    }

    plant_limit_values(plant);

    if (plant->water <= 0)
    {
        plant->state = PLANT_DEAD;
    }
    else if (plant->water < 30 && plant->happiness < 60)
    {
        plant->state = PLANT_JUDGE;
    }
    else if (plant->water < 60)
    {
        plant->state = PLANT_THIRSTY;
    }
    else if (plant->energy < 30 && plant->happiness < 70)
    {
        plant->state = PLANT_SAD;
    }
    else
    {
        plant->state = PLANT_HAPPY;
    }
}