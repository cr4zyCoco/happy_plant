#include <stdio.h>
#include "plant.h"
#include "display.h"
#include "ultrasonic.h"
#include "light_sensor.h"


static void draw_face_happy(void)
{
    int ex = 30;
    int ey = 15;
    int ew = 20;
    int eh = 20;
    //Left eye
    draw_rect(ex, ey, ew, eh);
    fill_rect(ex+(ew/2),ey,ew/2,10);
    fill_rect(ex,ey+10,ew,eh-10);
    //Right eye
    draw_rect(ex+45, ey, ew, eh);
    fill_rect(ex+45+(ew/2),ey,ew/2,10);
    fill_rect(ex+45,ey+10,ew,eh-10);

    //mouth
    int mx = 55;
    int my = 42;
    int mh = 8;
    draw_line_h(mx+2, my+mh, 13);
    draw_line_h(mx+4, my+mh+1, 9);
    fill_rect(mx, my, 17, mh);


}

static void draw_face_sad(void)
{
    int ex = 30;
    int ey = 20;
    int ew = 20;
    int eh = 15;
    //Left eye
    draw_rect(ex, ey, ew, eh);
    fill_rect(ex+(ew/2),ey,ew/2,10);
    fill_rect(ex,ey+10,ew,eh-10);
    //Right eye
    draw_rect(ex+45, ey, ew, eh);
    fill_rect(ex+45+(ew/2),ey,ew/2,10);
    fill_rect(ex+45,ey+10,ew,eh-10);

    //tear
    draw_line_v(ex+54, ey+eh+2, 4);
    draw_line_h(ex+53, ey+eh+4, 3);
    fill_rect(ex+52, ey+eh+5, 4, 3);
    draw_line_h(ex+53, ey+eh+8, 2);
    

    //mouth
    int mx = 55;
    int my = 38;
    int mh = 8;

    draw_line_h(mx+2, my+mh+4, 13);
    draw_line_h(mx+4, my+mh+3, 9);
    draw_pixel(mx+2, my+mh+5);
    draw_pixel(mx+2, my+mh+6);
    draw_pixel(mx+mh+6, my+mh+5);
    draw_pixel(mx+mh+6, my+mh+6);
    
}

static void draw_face_thirsty(void)
{
    int ex = 30;
    int ey = 15;
    int ew = 20;
    int eh = 20;
    //Left eye
    draw_rect(ex, ey, ew, eh);
    fill_rect(ex+(ew/2),ey,ew/2,10);
    fill_rect(ex,ey+10,ew,eh-10);
    //Right eye
    draw_rect(ex+45, ey, ew, eh);
    fill_rect(ex+45+(ew/2),ey,ew/2,10);
    fill_rect(ex+45,ey+10,ew,eh-10);

    //mouth
    int mx = 55;
    int my = 42;
    int mh = 8;

    draw_line_h(mx+2, my+mh+4, 13);
    draw_line_h(mx+4, my+mh+5, 9);
    
}

static void draw_face_lovely(void)
{
    int ex = 32;
    int ey = 15;
    int ew = 15;
    int eh = 20;
    //Left eye
    draw_line_h(ex, ey, ew);
    draw_pixel(ex-1, ey+1);
    draw_line_v(ex-2, ey+2, 4);
    draw_pixel(ex+ew+1, ey+1);
    draw_line_v(ex+ew+2, ey+2,4);

    //right eye
    draw_line_h(ex+45, ey, ew);
    draw_pixel(ex+45-1, ey+1);
    draw_line_v(ex+45-2, ey+2, 4);
    draw_pixel(ex+45+ew+1, ey+1);
    draw_line_v(ex+45+ew+2, ey+2,4);
    
    //blush
    //left
    for (int i = 0; i<9; i++){
        if (i % 2 == 0 && i != 6 && i != 8){
            draw_pixel(ex-3+i, ey+19);
            draw_pixel(ex-4+i, ey+21);
            draw_pixel(ex-4+i, ey+23);
            draw_pixel(ex-3+i, ey+25);
        }
        if (i%2 == 1){
            draw_pixel(ex-4+i, ey+20);
            draw_pixel(ex-4+i, ey+22);
            draw_pixel(ex-4+i, ey+24);
        }
    }
    //right
    for (int i = 0; i<9; i++){
        if (i % 2 == 0 && i != 6 && i != 8){
            draw_pixel(ex+45+ew-3+i, ey+19);
            draw_pixel(ex+45+ew-4+i, ey+21);
            draw_pixel(ex+45+ew-4+i, ey+23);
            draw_pixel(ex+45+ew-3+i, ey+25);
        }
        if (i%2 == 1){
            draw_pixel(ex+45+ew-4+i, ey+20);
            draw_pixel(ex+45+ew-4+i, ey+22);
            draw_pixel(ex+45+ew-4+i, ey+24);
        }
    }

    //mouth
    int mx = 55;
    int my = 42;
    int mh = 8;
    draw_line_h(mx+2, my+mh, 13);
    draw_line_h(mx+4, my+mh+1, 9);
    fill_rect(mx, my, 17, mh);
    
}


static void draw_face_judge(void)
{
    int ex = 30;
    int ey = 15;
    int ew = 20;
    int eh = 17;
    //Left eye
    draw_rect(ex, ey, ew, eh);
    fill_rect(ex+(ew/2),ey,ew/2,7);
    fill_rect(ex,ey+7,ew,eh-7);
    //Right eye
    draw_rect(ex+45, ey, ew, eh);
    fill_rect(ex+45+(ew/2),ey,ew/2,7);
    fill_rect(ex+45,ey+7,ew,eh-7);

    int mx = 55;
    int my = 42;
    int mh = 8;

    fill_rect(mx, my, 17, mh);
    draw_line_h(mx, my+1, 3);
    draw_line_h(mx+11, my+1, 3);
    
}


static void draw_face_dead(void)
{

    //eyes
    int ex = 30;
    int ey = 15;
    int ew = 20;

    draw_line_diagonal_r(ex, ey, ew);
    draw_line_diagonal_l(ex+ew, ey, ew);

    draw_line_diagonal_r(ex+45, ey, ew);
    draw_line_diagonal_l(ex+45+ew, ey, ew);

    //mouth
    int mx = 58;
    int my = 42;

    draw_rect(mx, my, 8, 8);

   
}


void plant_faces_draw(plant_state_t state)
{
    display_clear();

    switch (state)
    {
        case PLANT_HAPPY:
            draw_face_happy();
            break;

        case PLANT_LOVELY:
            draw_face_lovely();
            break;

        case PLANT_SAD:
            draw_face_sad();
            break;

        case PLANT_THIRSTY:
            draw_face_thirsty();
            break;

        case PLANT_JUDGE:
            draw_face_judge();
            break;

        case PLANT_DEAD:
            draw_face_dead();
            break;
    }

    display_update();
}