#ifndef PLANT_H
#define PLANT_H

typedef enum
{
    PLANT_HAPPY,
    PLANT_LOVELY,
    PLANT_SAD,
    PLANT_JUDGE,
    PLANT_THIRSTY,
    PLANT_DEAD
} plant_state_t;

typedef struct
{
    int water;
    int happiness;
    int energy;
    plant_state_t state;
} plant_t;

void plant_init(plant_t *plant);
void plant_update(plant_t *plant);
void plant_water(plant_t *plant);
void plant_draw(plant_state_t state);
void plant_draw(plant_state_t state);
void plant_add_happiness(plant_t *plant, int amount);

#endif