#include <stdio.h>
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/i2c.h"
#include "esp_log.h"

#include "buttons.h"
#include "dice.h"
#include "i2c.h"


void app_main(void)
{
    i2c_master_init();

}

