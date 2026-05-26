#include "i2c.h"

#include "driver/i2c.h"
#include "esp_log.h"

#define I2C_MASTER_SCL_IO       6
#define I2C_MASTER_SDA_IO       5
#define I2C_MASTER_NUM          I2C_NUM_0
#define I2C_MASTER_FREQ_HZ      400000

static const char *TAG = "I2C";

void i2c_master_init(void)
{
    i2c_config_t conf =
    {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ
    };

    ESP_ERROR_CHECK(
        i2c_param_config(I2C_MASTER_NUM, &conf)
    );

    ESP_ERROR_CHECK(
        i2c_driver_install(
            I2C_MASTER_NUM,
            conf.mode,
            0,
            0,
            0
        )
    );

    ESP_LOGI(TAG, "I2C initialized");
}