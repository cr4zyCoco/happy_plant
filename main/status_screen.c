#include "status_screen.h"
#include "display.h"
#include "buttons.h"
#include "ultrasonic.h"
#include "light_sensor.h"

static bool active = false;

static void draw_digit_small(int x, int y, int value)
{
    const int seg[10][7] =
    {
        {1,1,1,1,1,1,0},
        {0,1,1,0,0,0,0},
        {1,1,0,1,1,0,1},
        {1,1,1,1,0,0,1},
        {0,1,1,0,0,1,1},
        {1,0,1,1,0,1,1},
        {1,0,1,1,1,1,1},
        {1,1,1,0,0,0,0},
        {1,1,1,1,1,1,1},
        {1,1,1,1,0,1,1}
    };

    int w = 5;
    int h = 9;

    if (seg[value][0]) draw_line_h(x + 1, y, w);
    if (seg[value][1]) draw_line_v(x + w + 1, y + 1, h / 2);
    if (seg[value][2]) draw_line_v(x + w + 1, y + h / 2 + 1, h / 2);
    if (seg[value][3]) draw_line_h(x + 1, y + h, w);
    if (seg[value][4]) draw_line_v(x, y + h / 2 + 1, h / 2);
    if (seg[value][5]) draw_line_v(x, y + 1, h / 2);
    if (seg[value][6]) draw_line_h(x + 1, y + h / 2, w);
}

static void draw_number(int x, int y, int value)
{
    if (value < 0) value = 0;
    if (value > 9999) value = 9999;

    int digits[4];
    digits[0] = value / 1000;
    digits[1] = (value / 100) % 10;
    digits[2] = (value / 10) % 10;
    digits[3] = value % 10;

    int started = 0;

    for (int i = 0; i < 4; i++)
    {
        if (digits[i] != 0 || started || i == 3)
        {
            draw_digit_small(x, y, digits[i]);
            x += 10;
            started = 1;
        }
    }
}

static void draw_label_d(int x, int y)
{
    draw_rect(x, y, 7, 10);
    draw_line_v(x + 7, y + 2, 6);
}

static void draw_label_l(int x, int y)
{
    draw_line_v(x, y, 10);
    draw_line_h(x, y + 10, 7);
}

static void draw_label_h(int x, int y)
{
    draw_line_v(x, y, 10);
    draw_line_v(x + 7, y, 10);
    draw_line_h(x, y + 5, 7);
}

void status_screen_start(void)
{
    active = true;
}

bool status_screen_is_active(void)
{
    return active;
}

void status_screen_update(plant_t *plant)

{   
    button_event_t event = button_event();

    if (event == BUTTON_CLICK || event == BUTTON_LONG_PRESS)
    {
        active = false;
        return;
    }

    float distance = ultrasonic_get_distance_cm();
    int light = light_sensor_get_raw();
    int happiness = plant->happiness;

    display_clear();

    // D = Distance / Pegelabstand
    draw_label_d(5, 6);
    draw_number(25, 6, (int)distance);

    // L = Light
    draw_label_l(5, 26);
    draw_number(25, 26, light);

    // H = Happiness
    draw_label_h(5, 46);
    draw_number(25, 46, happiness);

    display_update();
}