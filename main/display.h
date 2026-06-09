#pragma once

#include <stdint.h>
#include <stdbool.h>

void ssd1306_init(void);

void display_clear(void);
void display_update(void);

void draw_line_h(int x, int y, int length);
void draw_line_v(int x, int y, int length);
void draw_line_diagonal_r(int x, int y, int length);
void draw_line_diagonal_l(int x, int y, int length);
void draw_rect(int x, int y, int w, int h);


void fill_rect(int x, int y, int w, int h);

void draw_pixel(int x, int y);