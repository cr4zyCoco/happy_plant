#include <stdbool.h>
#ifndef PLANT_H
#define PLANT_H

typedef enum{
    PLANT_MODE_FLOWER,
    PLANT_MODE_CACTUS
} plant_mode_t;

typedef enum{
    PLANT_HAPPY,
    PLANT_LOVELY,
    PLANT_SAD,
    PLANT_JUDGE,
    PLANT_THIRSTY,
    PLANT_DEAD
} plant_state_t;

typedef enum{
    NEED_NONE,
    NEED_WATER,
    NEED_LIGHT,
    NEED_HAPPINESS,
    NEED_CRITICAL
} plant_need_t;

typedef struct{
    int water;
    int happiness;
    int energy;

    int age_days;
    bool death_recorded;
    int death_day;
    int sun_minutes_today;

    plant_mode_t mode;
    plant_state_t state;
    plant_need_t need;
} plant_t;

void plant_init(plant_t *plant);
void plant_update(plant_t *plant);
void plant_draw(plant_state_t state);
void plant_draw(plant_state_t state);
void plant_add_happiness(plant_t *plant, int amount);

#endif