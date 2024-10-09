#ifndef GRAYSCALE_SENSOR_H
#define GRAYSCALE_SENSOR_H

#include "main.h"
#include "i2c.h"

#define GW_GRAY_ADDR 0x4C
extern void grayscale_init(void);
extern void grayscale_GetData(void);

#endif
