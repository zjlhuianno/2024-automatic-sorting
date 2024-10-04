#include "main.h"
#include "cmsis_os.h"
#include "i2c.h"

#define GW_GRAY_ADDR 0x4C

void Grayscale_task(void const * argument);
extern void grayscale_init(void);
extern void grayscale_GetData(void);
