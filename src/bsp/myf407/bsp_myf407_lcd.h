/**
 * @file    bsp_myf407_lcd.h
 * @author  meng_yu
 * @brief   myf407 board lcd function header file
 * @version 0.0.1
 * @date    2020-10-17
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */

#ifndef _BSP_MYF407_LCD_H_
#define _BSP_MYF407_LCD_H_

#include "drv_lcd.h"

//ʹ��NOR/SRAM�� Bank1.sector4,��ַλHADDR[27,26]=11 A6��Ϊ��������������
//ע������ʱSTM32�ڲ�������һλ����! 111 1110=0X7E
//#define LCD_BASE        ((u32)(0x6C000000 | 0x0000007E))
#define MYF407_LCD_BASE ((uint32_t)(0x6C000000 | 0x0000007E))

extern struct bsp_lcd bsp_myf407_lcd;

#endif /* _BSP_MYF407_LCD_H_ */

