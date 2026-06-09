#include "pet_game.h"
#include "display.h"
#include "buttons.h"
#include "plant.h"

#define REQUIRED_HITS 4

#define HEART_SCALE 3
#define HEART_SPEED 10

#define TARGET_X 64
#define TARGET_MIN_X 54
#define TARGET_MAX_X 74

static bool active = false;
static bool success = false;

static int heart_x = 0;
static int hits = 0;

static int success_ticks = 0;


/*
 * Pixelherz
 */
static const int heart[6][7] =
{
    {0,1,1,0,1,1,0},
    {1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1},
    {0,1,1,1,1,1,0},
    {0,0,1,1,1,0,0},
    {0,0,0,1,0,0,0}
};


static void draw_pixel_heart(int cx, int cy, int scale)
{
    int w = 7 * scale;
    int h = 6 * scale;

    int start_x = cx - w / 2;
    int start_y = cy - h / 2;

    for (int row = 0; row < 6; row++)
    {
        for (int col = 0; col < 7; col++)
        {
            if (heart[row][col])
            {
                fill_rect(
                    start_x + col * scale,
                    start_y + row * scale,
                    scale,
                    scale
                );
            }
        }
    }
}


static const int heart_outline[6][7] =
{
    {0,1,1,0,1,1,0},
    {1,0,0,0,0,0,1},
    {1,0,0,0,0,0,1},
    {0,1,0,0,0,1,0},
    {0,0,1,0,1,0,0},
    {0,0,0,1,0,0,0}
};

static void draw_pixel_heart_outline(int cx, int cy, int scale)
{
    int w = 7 * scale;
    int h = 6 * scale;

    int start_x = cx - w / 2;
    int start_y = cy - h / 2;

    for (int row = 0; row < 6; row++)
    {
        for (int col = 0; col < 7; col++)
        {
            if (heart_outline[row][col])
            {
                fill_rect(
                    start_x + col * scale,
                    start_y + row * scale,
                    scale,
                    scale
                );
            }
        }
    }
}

static void draw_heart_frame(int cx, int cy, int scale)
{
    draw_pixel_heart_outline(cx, cy, scale + 1);
}


static void draw_progress_bar(void)
{
    int x = 20;
    int y = 56;
    int w = 88;
    int h = 6;

    draw_rect(x, y, w, h);

    int fill_w = (w * hits) / REQUIRED_HITS;

    if (fill_w > 0)
    {
        fill_rect(
            x + 1,
            y + 1,
            fill_w - 1,
            h - 2
        );
    }
}


/*
 * Erfolgsanimation
 */
static void draw_success_animation(void)
{
    display_clear();

    int radius = success_ticks * 3;

    draw_rect(
        64 - radius,
        32 - radius,
        radius * 2,
        radius * 2
    );

    draw_rect(
        64 - radius / 2,
        32 - radius / 2,
        radius,
        radius
    );

    display_update();
}


void pet_game_start(void)
{
    active = true;
    success = false;

    heart_x = -20;
    hits = 0;

    success_ticks = 0;
}


bool pet_game_is_active(void)
{
    return active;
}


void pet_game_update(plant_t *plant)
{
    /*
     * Erfolgsphase
     */
    if (success)
    {
        success_ticks++;

        if (success_ticks < 20)
        {
            draw_success_animation();
        }
        else
        {
            plant_draw(PLANT_LOVELY);
        }

        /*
         * 50 Ticks bei 100 ms
         * = 5 Sekunden
         */
        if (success_ticks >= 70)
        {
            active = false;
            success = false;
        }

        return;
    }


    /*
     * Spiel zeichnen
     */
    display_clear();

    draw_heart_frame(TARGET_X, 28, HEART_SCALE);

    draw_pixel_heart(
        heart_x,
        28,
        HEART_SCALE
    );

    draw_progress_bar();

    display_update();


    /*
     * Klick erkannt
     */
    if (button_event() == BUTTON_CLICK)
    {
        if (
            heart_x >= TARGET_MIN_X &&
            heart_x <= TARGET_MAX_X
        )
        {
            hits++;
        }

        if (hits >= REQUIRED_HITS)
        {
            plant_add_happiness(plant, 20);

            if (plant->happiness > 100)
            {
                plant->happiness = 100;
            }

            success = true;
            success_ticks = 0;
            

            return;
        }
    }


    /*
     * Herz bewegen
     */
    heart_x += HEART_SPEED;

    if (heart_x > 150)
    {
        heart_x = -20;
    }
}