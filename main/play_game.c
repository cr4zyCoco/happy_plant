#include "play_game.h"
#include "display.h"
#include "buttons.h"
#include "esp_random.h"
#include "plant.h"

//Icons wurden mit Hilfe von KI erstellt
//Game Logik-Aufbau mit Hlfe von KI
// Variablen zur leichteren Anpassung


#define HEART_X 32
#define GROUND_Y 46

#define JUMP_STRENGTH -8
#define GRAVITY 1

#define OBSTACLE_START_X 130
#define OBSTACLE_MIN_SPEED 4
#define OBSTACLE_MAX_SPEED 6

#define GAME_OVER_TICKS 30

static bool active = false;
static bool game_over = false;

static int heart_y = GROUND_Y;
static int heart_velocity = 1;

static int obstacle_x = OBSTACLE_START_X;
static int obstacle_y = GROUND_Y;
static int obstacle_speed = 4;
static int obstacle_type = 0;

static int score = 0;
static int game_over_ticks = 0;

//Icons wurden mit Hilfe von KI erstellt
static const int heart[6][7] =
{
    {0,1,1,0,1,1,0},
    {1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1},
    {0,1,1,1,1,1,0},
    {0,0,1,1,1,0,0},
    {0,0,0,1,0,0,0}
};

static const int leaf[8][6] =
{
    {0,0,0,1,1,0},
    {0,0,1,1,0,1},
    {0,1,1,0,1,1},
    {1,1,0,1,1,0},
    {1,0,1,1,0,0},
    {0,1,1,0,0,0},
    {1,1,0,0,0,0},
    {0,1,0,0,0,0}
};

static const int flower[8][8] =
{
    {0,0,1,0,0,1,0,0},
    {0,1,1,1,1,1,1,0},
    {1,1,0,1,1,0,1,1},
    {0,1,1,1,1,1,1,0},
    {0,0,1,1,1,1,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,1,0,0,1,0,0}
};

static void draw_pixel_heart(int cx, int cy, int scale){
    int start_x = cx - (7 * scale) / 2;
    int start_y = cy - (6 * scale) / 2;

    for (int row = 0; row < 6; row++){
        for (int col = 0; col < 7; col++){
            if (heart[row][col]){
                fill_rect(start_x + col * scale,
                          start_y + row * scale,
                          scale,
                          scale);
            }
        }
    }
}

static void draw_leaf(int x, int y){
    for (int row = 0; row < 8; row++){
        for (int col = 0; col < 6; col++){
            if (leaf[row][col]){
                draw_pixel(x + col, y + row);
            }
        }
    }
}

static void draw_flower(int x, int y){
    for (int row = 0; row < 8; row++){
        for (int col = 0; col < 8; col++){
            if (flower[row][col]){
                draw_pixel(x + col, y + row);
            }
        }
    }
}

static void draw_cactus(int x, int y){
    // Stamm
    draw_line_v(x, y, 14);
    draw_line_v(x + 1, y, 14);

    // linker Arm
    draw_line_h(x - 4, y + 6, 4);
    draw_line_v(x - 4, y + 3, 4);

    // rechter Arm
    draw_line_h(x + 2, y + 8, 4);
    draw_line_v(x + 5, y + 5, 4);

    // kleine Stacheln
    draw_pixel(x - 1, y + 2);
    draw_pixel(x + 2, y + 4);
    draw_pixel(x - 1, y + 9);
    draw_pixel(x + 2, y + 11);
}

static void spawn_obstacle(void){
    obstacle_x = OBSTACLE_START_X;

    obstacle_type = esp_random() % 3;

    if (obstacle_type == 0)       // Blume
    obstacle_speed = 6;

    else if (obstacle_type == 1)  // Blatt
    obstacle_speed = 5;

    else                          // Kaktus
    obstacle_speed = 4;

    if (obstacle_type == 0){
        obstacle_y = GROUND_Y + 2;      // Blume unten
    } 
    else if (obstacle_type == 1){
        obstacle_y = GROUND_Y - 8;      // Blatt fliegt höher
    } 
    else{
    obstacle_y = GROUND_Y - 10;
    }
}

static void draw_obstacle(void){
    if (obstacle_type == 0){
        draw_flower(obstacle_x, obstacle_y);
    }
    else if (obstacle_type == 1){
        draw_leaf(obstacle_x, obstacle_y);
    }
    else{
        draw_cactus(obstacle_x, obstacle_y);
    }
}

static bool collision_detected(void){
    int heart_left   = HEART_X - 10;
    int heart_right  = HEART_X + 10;
    int heart_top    = heart_y - 9;
    int heart_bottom = heart_y + 9;

    int obs_left;
    int obs_right;
    int obs_top;
    int obs_bottom;

    if (obstacle_type == 0){
        obs_left = obstacle_x;
        obs_right = obstacle_x + 8;
        obs_top = obstacle_y;
        obs_bottom = obstacle_y + 8;
    }
    else if (obstacle_type == 1){
        obs_left = obstacle_x;
        obs_right = obstacle_x + 6;
        obs_top = obstacle_y;
        obs_bottom = obstacle_y + 8;
    }
    else {
        obs_left = obstacle_x - 4;
        obs_right = obstacle_x + 6;
        obs_top = obstacle_y;
        obs_bottom = obstacle_y + 14;
    }

    return heart_left < obs_right &&
           heart_right > obs_left &&
           heart_top < obs_bottom &&
           heart_bottom > obs_top;
}

static void draw_score_bar(void){
    int w = score;

    if (w > 100){
        w = 100;
    }

    draw_rect(14, 56, 100, 6);

    if (w > 0){
        fill_rect(15, 57, w, 4);
    }
}

void play_game_start(void){
    active = true;
    game_over = false;

    heart_y = GROUND_Y;
    heart_velocity = 0;

    score = 0;
    game_over_ticks = 0;

    spawn_obstacle();
}

bool play_game_is_active(void){
    return active;
}

void play_game_update(plant_t *plant){
    if (game_over){
        game_over_ticks++;

        plant_draw(PLANT_JUDGE);

        if (game_over_ticks >= GAME_OVER_TICKS){
            plant_add_happiness(plant, score / 5);

            active = false;
            game_over = false;
        }

        return;
    }

    if (button_event() == BUTTON_CLICK){
        if (heart_y >= GROUND_Y){
            heart_velocity = JUMP_STRENGTH;
        }
    }

    heart_y += heart_velocity;
    heart_velocity += GRAVITY;

    if (heart_y > GROUND_Y){
        heart_y = GROUND_Y;
        heart_velocity = 0;
    }

    obstacle_x -= obstacle_speed;

    if (obstacle_x < -20){
        score += 10;
        spawn_obstacle();
    }

    if (collision_detected()){
        game_over = true;
        game_over_ticks = 0;
        return;
    }

    display_clear();
    draw_line_h(0, GROUND_Y + 10, 128);
    draw_pixel_heart(HEART_X, heart_y, 3);
    draw_obstacle();
    draw_score_bar();
    display_update();
}