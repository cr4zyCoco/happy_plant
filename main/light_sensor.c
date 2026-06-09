#include "light_sensor.h"
#include "driver/adc.h"

#define LIGHT_ADC_CHANNEL ADC1_CHANNEL_3   // GPIO2
#define DARK_THRESHOLD   3000
#

void light_sensor_init(void)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(LIGHT_ADC_CHANNEL, ADC_ATTEN_DB_11);
}

int light_sensor_get_raw(void)
{
    return adc1_get_raw(LIGHT_ADC_CHANNEL);
}

int light_sensor_is_dark(void)
{
    int value = light_sensor_get_raw();

    if (value > DARK_THRESHOLD)
    {
        return 1;   // dunkel
    }

    return 0;       // hell
}