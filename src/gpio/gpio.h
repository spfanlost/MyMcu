

/**
 * @file    gpio.h
 * @author  meng_yu
 * @brief   &#& function header file
 * @version 0.0.1
 * @date    2020-09-07
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#ifndef _GPIO_H_
#define _GPIO_H_

/*-----------------------------------------------------------------------------------
  Exported types
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported macro
-----------------------------------------------------------------------------------*/
#define FTIR (1)     //�½��ش���
#define RTIR (2)     //�����ش���

//GPIO����ר�ú궨��
#define GPIO_MODE_IN (0)     //��ͨ����ģʽ
#define GPIO_MODE_OUT (1)     //��ͨ���ģʽ
#define GPIO_MODE_AF (2)     //AF����ģʽ
#define GPIO_MODE_AIN (3)     //ģ������ģʽ
#define GPIO_SPEED_2M (0)     //GPIO�ٶ�2Mhz
#define GPIO_SPEED_25M (1)     //GPIO�ٶ�25Mhz
#define GPIO_SPEED_50M (2)     //GPIO�ٶ�50Mhz
#define GPIO_SPEED_100M (3)     //GPIO�ٶ�100Mhz
#define GPIO_PUPD_NONE (0)     //����������
#define GPIO_PUPD_PU (1)     //����
#define GPIO_PUPD_PD (2)     //����
#define GPIO_PUPD_RES (3)     //����
#define GPIO_OTYPE_PP (0)     //�������
#define GPIO_OTYPE_OD (1)     //��©���

//GPIO���ű�Ŷ���
#define PIN0 (1<<0)
#define PIN1 (1<<1)
#define PIN2 (1<<2)
#define PIN3 (1<<3)
#define PIN4 (1<<4)
#define PIN5 (1<<5)
#define PIN6 (1<<6)
#define PIN7 (1<<7)
#define PIN8 (1<<8)
#define PIN9 (1<<9)
#define PIN10 (1<<10)
#define PIN11 (1<<11)
#define PIN12 (1<<12)
#define PIN13 (1<<13)
#define PIN14 (1<<14)
#define PIN15 (1<<15)

/*-----------------------------------------------------------------------------------
  Exported variables
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported functions
-----------------------------------------------------------------------------------*/
void GPIO_Set(GPIO_TypeDef*GPIOx,
    uint32_t PINx,
    uint32_t MODE,
    uint32_t OTYPE,
    uint32_t OSPEED,
    uint32_t PUPD);
void GPIO_AF_Set(GPIO_TypeDef*GPIOx, uint8_t PINx, uint8_t AFx);

#endif /* _GPIO_H_ */

