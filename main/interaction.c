#include "interaction.h"
#include "buttons.h"
#include "display.h"
#include "pet_game.h"
#include "play_game.h"
#include "thoughts.h"
#include "status_screen.h"

//für die Idee der Logik wurde KI zur Hilfe genommen und für die Icons. 

typedef enum
{
    ACTION_PET,
    ACTION_PLAY,
    ACTION_THOUGHT,
    ACTION_STATUS,
    ACTION_EXIT,
    ACTION_COUNT
} action_t;

static bool menu_open = false;
static action_t selected_action = ACTION_PET;

static bool last_button = false;
static int press_ticks = 0;
static int result_ticks = 0;

static void draw_icon_heart(int x, int y)
{
    //mit Hilfe von KI erstellt:
    fill_rect(x, y + 3, 4, 4);
    fill_rect(x + 6, y + 3, 4, 4);
    fill_rect(x + 2, y + 6, 8, 4);
    fill_rect(x + 3, y + 10, 6, 3);
    fill_rect(x + 5, y + 13, 2, 2);
    
}

static void draw_icon_play(int x, int y)
{
    draw_line_v(x, y, 16);
    draw_line_diagonal_r(x, y, 8);
    draw_line_diagonal_l(x + 8, y + 8, 8);
}

static void draw_icon_bubble(int x, int y)
{
    //mit Hilfe von KI erstellt:
    draw_rect(x, y, 16, 10);
    draw_pixel(x + 4, y + 11);
    draw_pixel(x + 3, y + 12);

    draw_pixel(x + 4, y + 5);
    draw_pixel(x + 8, y + 5);
    draw_pixel(x + 12, y + 5);
}

static void draw_icon_status(int x, int y)
{
    //mit Hilfe von KI erstellt:
    // kleiner Topf
    draw_rect(x + 4, y + 10, 10, 8);

    // Blume
    draw_line_v(x + 9, y + 4, 8);
    draw_pixel(x + 8, y + 3);
    draw_pixel(x + 10, y + 3);
    draw_pixel(x + 7, y + 4);
    draw_pixel(x + 11, y + 4);
}

static void draw_x(int x, int y)
{
    draw_line_diagonal_r(x, y, 14);
    draw_line_diagonal_l(x + 14, y, 14);
}


static void draw_menu(void)
{
    display_clear();

    // Reihe 1
    draw_icon_heart(20, 10);
    draw_icon_play(58, 10);
    draw_icon_bubble(96, 10);

    // Reihe 2
    draw_icon_status(38, 42);
    draw_x(78, 42);

    int frame_x = 14;
    int frame_y = 6;

    if (selected_action == ACTION_PLAY)
    {
        frame_x = 54;
        frame_y = 6;
    }
    else if (selected_action == ACTION_THOUGHT)
    {
        frame_x = 92;
        frame_y = 6;
    }
    else if (selected_action == ACTION_STATUS)
    {
        frame_x = 32;
        frame_y = 38;
    }
    else if (selected_action == ACTION_EXIT)
    {
        frame_x = 72;
        frame_y = 38;
    }

    draw_rect(frame_x, frame_y, 24, 24);
    display_update();
}

static void execute_action(plant_t *plant)
{
    switch (selected_action)
    {
        case ACTION_PET:
            pet_game_start();
            menu_open = false;
            result_ticks = 0;
            break;

        case ACTION_PLAY:
            play_game_start();
            menu_open = false;
            result_ticks = 0;
            break;

        case ACTION_THOUGHT:
            thoughts_start(plant);
            menu_open = false;
            result_ticks = 0;
            break;

        case ACTION_STATUS:
            status_screen_start();
            menu_open = false;
            result_ticks = 0;
            break;

        case ACTION_EXIT:
            menu_open = false;
            result_ticks = 0;
            break;

        default:
            break;
    }
}
void interaction_init(void)
{
    menu_open = false;
    selected_action = ACTION_PET;
}

bool interaction_is_active(void)
{
    return menu_open || result_ticks > 0;
}

void interaction_update(plant_t *plant)
{
    switch(button_event())
    {
        case BUTTON_CLICK:

            if (!menu_open)
            {
                menu_open = true;
            }
            else
            {
                selected_action++;

                if(selected_action >= ACTION_COUNT)
                {
                    selected_action = 0;
                }
            }

            break;

        case BUTTON_LONG_PRESS:

            if (menu_open)
            {
                execute_action(plant);
            }

            break;

        default:
            break;
    }

    if(menu_open)
    {
        draw_menu();
    }

    if(result_ticks > 0)
    {
        result_ticks--;
    }
}