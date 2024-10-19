#include "main.h"
#include "cmsis_os.h"
#include "i2c.h"
#include "configs.h"

void Grayscale_task(void const * argument);
void grayscale_GetData(uint8_t num);
