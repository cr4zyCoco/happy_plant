#include "plant_clock.h"
#include "buttons.h"
#include "display.h"
#include "esp_timer.h"

//Zahlencode für die 7-Segmentanzeige mit Hilfe von KI erstellt 

static bool clock_set = false;

static int hour = 0;
static int minute = 0;

static int edit_step = 0;

static int digit_h10 = 0;
static int digit_h1  = 0;
static int digit_m10 = 0;
static int digit_m1  = 0;

static int64_t last_update_us = 0;

static void draw_segment_digit(int x, int y, int value){
    const int segments[10][7] =
    {
        {1,1,1,1,1,1,0}, // 0
        {0,1,1,0,0,0,0}, // 1
        {1,1,0,1,1,0,1}, // 2
        {1,1,1,1,0,0,1}, // 3
        {0,1,1,0,0,1,1}, // 4
        {1,0,1,1,0,1,1}, // 5
        {1,0,1,1,1,1,1}, // 6
        {1,1,1,0,0,0,0}, // 7
        {1,1,1,1,1,1,1}, // 8
        {1,1,1,1,0,1,1}  // 9
    };

    int t = 2;
    int w = 10;
    int h = 18;

    if (segments[value][0]) fill_rect(x + t,     y,         w, t); // oben
    if (segments[value][1]) fill_rect(x + w + t, y + t,     t, h/2); // rechts oben
    if (segments[value][2]) fill_rect(x + w + t, y + h/2+t, t, h/2); // rechts unten
    if (segments[value][3]) fill_rect(x + t,     y + h,     w, t); // unten
    if (segments[value][4]) fill_rect(x,         y + h/2+t, t, h/2); // links unten
    if (segments[value][5]) fill_rect(x,         y + t,     t, h/2); // links oben
    if (segments[value][6]) fill_rect(x + t,     y + h/2,   w, t); // mitte
}

void plant_clock_init(void){
    clock_set = false;
    edit_step = 0;

    digit_h10 = 0;
    digit_h1  = 0;
    digit_m10 = 0;
    digit_m1  = 0;

    last_update_us = esp_timer_get_time();
}

bool plant_clock_is_set(void){
    return clock_set;
}

void plant_clock_draw_setup(void){
    display_clear();

    draw_segment_digit(15, 20, digit_h10);
    draw_segment_digit(35, 20, digit_h1);

    draw_pixel(60, 27);
    draw_pixel(60, 28);
    draw_pixel(60, 36);
    draw_pixel(60, 37);

    draw_segment_digit(72, 20, digit_m10);
    draw_segment_digit(92, 20, digit_m1);

    int frame_x = 15;

    switch(edit_step)
    {
        case 0: frame_x = 15; break;
        case 1: frame_x = 35; break;
        case 2: frame_x = 70; break;
        case 3: frame_x = 90; break;
    }

    draw_rect(frame_x - 2, 18, 16, 20);

    display_update();
}

void plant_clock_handle_input(void){
    switch(button_event())
    {
        case BUTTON_CLICK:

            switch(edit_step)
            {
                case 0:
                    digit_h10 = (digit_h10 + 1) % 3;
                    break;

                case 1:
                    digit_h1 = (digit_h1 + 1) % 10;
                    break;

                case 2:
                    digit_m10 = (digit_m10 + 1) % 6;
                    break;

                case 3:
                    digit_m1 = (digit_m1 + 1) % 10;
                    break;
            }

            break;

        case BUTTON_LONG_PRESS:

            edit_step++;

            if(edit_step > 3){
                hour = digit_h10 * 10 + digit_h1;
                minute = digit_m10 * 10 + digit_m1;

                if(hour > 23)
                    hour = 23;

                clock_set = true;

                last_update_us = esp_timer_get_time();
                }

            break;

        default:
            break;
    }
}

void plant_clock_update(void){
    if(!clock_set)
        return;

    int64_t now = esp_timer_get_time();

    if(now - last_update_us >= 60000000){
        minute++;

            if(minute >= 60){
                minute = 0;
                hour++;
                }

            if(hour >= 24){
                hour = 0;
                }

            last_update_us += 60000000;
        }
}

int plant_clock_get_hour(void){
    return hour;
}

int plant_clock_get_minute(void){
    return minute;
}

bool plant_clock_is_daytime(void){
    return (hour >= 8 && hour < 20);
}