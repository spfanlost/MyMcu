

/**
 * @file    led.h
 * @author  meng_yu
 * @brief   Low level LED definitions function header file
 * @version 0.0.1
 * @date    2020-09-10
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#ifndef _LED_H_
#define _LED_H_
#include "gpio.h"

/*-----------------------------------------------------------------------------------
  Exported types
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported macro
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported variables
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported functions
-----------------------------------------------------------------------------------*/
#define LED1_PIN PIN9 //(PF9)
#define LED2_PIN PIN10 //(PF10)

extern void led_init(void);
extern void led_on(dword_t pin);
extern void led_off(dword_t pin);

#endif /* _LED_H_ */

