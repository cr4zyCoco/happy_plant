#include "display.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

#define I2C_PORT I2C_NUM_0
#define OLED_ADDR 0x3C

// OLED 128x64 = 1024 Bytes Buffer
static uint8_t buffer[128 * 8];

void draw_line_h(int x, int y, int length)
{
    for (int i = 0; i < length; i++)
    {
        draw_pixel(x + i, y);
    }
}

void draw_line_v(int x, int y, int length)
{
    for (int i = 0; i < length; i++)
    {
        draw_pixel(x, y + i);
    }
}

void draw_line_diagonal_r(int x, int y, int length)
{
    for (int i = 0; i < length; i++)
    {
        draw_pixel(x+i, y + i);
    }
}

void draw_line_diagonal_l(int x, int y, int length)
{
    for (int i = 0; i < length; i++)
    {
        draw_pixel(x-i, y + i);
    }
}

void draw_rect(int x, int y, int w, int h)
{
    draw_line_h(x, y, w);
    draw_line_h(x, y + h, w);

    draw_line_v(x, y, h);
    draw_line_v(x + w, y, h);
}

void fill_rect(int x, int y, int w, int h)
{
    for (int iy = 0; iy < h; iy++)
    {
        for (int ix = 0; ix < w; ix++)
        {
            draw_pixel(x + ix, y + iy);
        }
    }
}


// -------------------------
// Low-Level I2C Command
// -------------------------
static void ssd1306_send_cmd(uint8_t cmd)
{
    uint8_t data[2] = {0x00, cmd};
    i2c_master_write_to_device(I2C_PORT, OLED_ADDR,
                               data, 2,
                               1000 / portTICK_PERIOD_MS);
}




// -------------------------
// Init Display
// -------------------------
void ssd1306_init(void)
{
    vTaskDelay(pdMS_TO_TICKS(100));

    ssd1306_send_cmd(0xAE); // display off
    ssd1306_send_cmd(0x20); // memory mode
    ssd1306_send_cmd(0x00); // horizontal mode

    ssd1306_send_cmd(0xB0);

    ssd1306_send_cmd(0xC8); // COM scan dir

    ssd1306_send_cmd(0x00);
    ssd1306_send_cmd(0x10);

    ssd1306_send_cmd(0x40); // start line

    ssd1306_send_cmd(0x81); // contrast
    ssd1306_send_cmd(0xFF);

    ssd1306_send_cmd(0xA1); // segment remap
    ssd1306_send_cmd(0xA6); // normal display

    ssd1306_send_cmd(0xA8);
    ssd1306_send_cmd(0x3F);

    ssd1306_send_cmd(0xD3);
    ssd1306_send_cmd(0x00);

    ssd1306_send_cmd(0xD5);
    ssd1306_send_cmd(0x80);

    ssd1306_send_cmd(0xD9);
    ssd1306_send_cmd(0xF1);

    ssd1306_send_cmd(0xDA);
    ssd1306_send_cmd(0x12);

    ssd1306_send_cmd(0xDB);
    ssd1306_send_cmd(0x40);

    ssd1306_send_cmd(0x8D);
    ssd1306_send_cmd(0x14);

    ssd1306_send_cmd(0xAF); // display on
}


// -------------------------
// Clear buffer
// -------------------------
void display_clear(void)
{
    memset(buffer, 0, sizeof(buffer));
}




// -------------------------
// Set pixel in buffer
// -------------------------
void draw_pixel(int x, int y)
{
    if (x < 0 || x >= 128 || y < 0 || y >= 64) return;

    buffer[x + (y / 8) * 128] |= (1 << (y % 8));
}


// -------------------------
// Send buffer to OLED
// -------------------------
void display_update(void)
{
    for (int page = 0; page < 8; page++) {

        ssd1306_send_cmd(0xB0 + page);
        ssd1306_send_cmd(0x00);
        ssd1306_send_cmd(0x10);

        uint8_t data[129];
        data[0] = 0x40; // data mode

        for (int i = 0; i < 128; i++) {
            data[i + 1] = buffer[i + page * 128];
        }

        i2c_master_write_to_device(I2C_PORT, OLED_ADDR,
                                   data, 129,
                                   1000 / portTICK_PERIOD_MS);
    }
}