/**
 * @file    mcu_io.h
 * @author  meng_yu
 * @brief   &#& function header file
 * @version 0.0.1
 * @date    2020-10-11
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */

#ifndef _MCU_IO_H_
#define _MCU_IO_H_

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
#define PIN0 (0x1U<<0)
#define PIN1 (0x1U<<1)
#define PIN2 (0x1U<<2)
#define PIN3 (0x1U<<3)
#define PIN4 (0x1U<<4)
#define PIN5 (0x1U<<5)
#define PIN6 (0x1U<<6)
#define PIN7 (0x1U<<7)
#define PIN8 (0x1U<<8)
#define PIN9 (0x1U<<9)
#define PIN10 (0x1U<<10)
#define PIN11 (0x1U<<11)
#define PIN12 (0x1U<<12)
#define PIN13 (0x1U<<13)
#define PIN14 (0x1U<<14)
#define PIN15 (0x1U<<15)

#define GPIOA_CLK (0x1U<<0)
#define GPIOB_CLK (0x1U<<1)
#define GPIOC_CLK (0x1U<<2)
#define GPIOD_CLK (0x1U<<3)
#define GPIOE_CLK (0x1U<<4)
#define GPIOF_CLK (0x1U<<5)
#define GPIOG_CLK (0x1U<<6)
#define GPIOH_CLK (0x1U<<7)
#define GPIOI_CLK (0x1U<<8)

/*-----------------------------------------------------------------------------------
  Exported variables
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported functions
-----------------------------------------------------------------------------------*/
void mcu_io_config(GPIO_TypeDef*GPIOx,
    dword_t PINx,
    dword_t MODE,
    dword_t OTYPE,
    dword_t OSPEED,
    dword_t PUPD);
void mcu_io_af_config(GPIO_TypeDef*GPIOx, dword_t pin, dword_t AFx);
void mcu_io_clk_enable(dword_t gpio_clk);
void mcu_io_set(GPIO_TypeDef*GPIOx, dword_t pin);
void mcu_io_reset(GPIO_TypeDef*GPIOx, dword_t pin);
void mcu_io_toggle(GPIO_TypeDef*GPIOx, dword_t pin);
byte_t mcu_io_pin_val(GPIO_TypeDef*GPIOx, dword_t pin);


#endif /* _MCU_IO_H_ */

