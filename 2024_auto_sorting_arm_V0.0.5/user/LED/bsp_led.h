#ifndef BSP_LED_H
#define BSP_LED_H

#include "main.h"

/**
  * @brief          aRGB show
  * @param[in]      aRGB: 0xaaRRGGBB, 'aa' is alpha, 'RR' is red, 'GG' is green, 'BB' is blue
  * @retval         none
  */
/**
  * @brief          ��ʾRGB
  * @param[in]      aRGB:0xaaRRGGBB,'aa' ��͸����,'RR'�Ǻ�ɫ,'GG'����ɫ,'BB'����ɫ
  * @retval         none
  */
	
extern void led_init(void);
extern void aRGB_led_show(uint32_t aRGB);


#endif
