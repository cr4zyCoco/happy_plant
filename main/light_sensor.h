#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

void light_sensor_init(void);
int light_sensor_get_raw(void);
int light_sensor_is_dark(void);

#endif