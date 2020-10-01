

/**
 * @file    gpio.c
 * @author  meng_yu
 * @brief   &#& function realize
 * @version 0.0.1
 * @date    2020-09-07
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#include "common.h"
#include "stm32_config.h"
#include "gpio.h"

/*-----------------------------------------------------------------------------------
  Private declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Extern variables declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Global variables definition
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Local functions declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Local functions definition
-----------------------------------------------------------------------------------*/
/**
 * @brief  This function GPIO_Set
 * @param  GPIOx: GPIOA~GPIOI.
 * @param  PINx: PINx:0X0000~0XFFFF, each bit means a pin
 * @param  MODE: 0~3;ģʽѡ��,0,����(ϵͳ��λĬ��״̬);1,��ͨ���;2,���ù���;3,ģ������.
 * @param  OTYPE: 0/1;�������ѡ��,0,�������;1,��©���.
 * @param  OSPEED: 0~3;����ٶ�����,0,2Mhz;1,25Mhz;2,50Mhz;3,100Mh.
 * @param  PUPD: ����������,0,����������;1,����;2,����;
 * @note:  ������ģʽ(��ͨ����/ģ������)��,OTYPE��OSPEED������Ч!!
 */
void GPIO_Set(GPIO_TypeDef*GPIOx,
    dword_t PINx,
    dword_t MODE,
    dword_t OTYPE,
    dword_t OSPEED,
    dword_t PUPD)
{
    dword_t pinpos = 0, pos = 0, curpin = 0;

    for(pinpos = 0; pinpos<16; pinpos++)
    {
        pos = 1 << pinpos;
        curpin = PINx&pos;
        if(curpin==pos)
        {
            GPIOx->MODER &=~(3 << (pinpos*2)); //�����ԭ��������
            GPIOx->MODER |= MODE << (pinpos*2); //�����µ�ģʽ
            if((MODE==0X01)||(MODE==0X02)) //��������ģʽ/���ù���ģʽ
            {
                GPIOx->OSPEEDR &=~(3 << (pinpos*2)); //���ԭ��������
                GPIOx->OSPEEDR |= (OSPEED << (pinpos*2)); //�����µ��ٶ�ֵ
                GPIOx->OTYPER &=~(1 << pinpos); //���ԭ��������
                GPIOx->OTYPER |= OTYPE << pinpos; //�����µ����ģʽ
            }
            GPIOx->PUPDR &=~(3 << (pinpos*2)); //�����ԭ��������
            GPIOx->PUPDR |= PUPD << (pinpos*2); //�����µ�������
        }
    }
}


/**
 * @brief  This function GPIO_AF_Set
 * @param  GPIOx: GPIOA~GPIOI.
 * @param  PINx: 0~15,����IO���ű��.
 * @param  AFx: AF0~AF15.AF0~15�������(��������г����õ�,��ϸ�����STM32F40x-DataSheet,Table 9):
 *      AF0:MCO/SWD/SWCLK/RTC      AF1:TIM1/TIM2;                AF2:TIM3~5;                    AF3:TIM8~11
 *      AF4:I2C1~I2C4;             AF5:SPI1~SPI6;                AF6:SPI3/SAI1;                 AF7:SPI2/3/USART1~3/
    UART5/SPDIFRX;
 *      AF8:USART4~8/SPDIFRX/SAI2; AF9;CAN1~2/TIM12~14/LCD/QSPI; AF10:USB_OTG/USB_HS/SAI2/QSPI; AF11:ETH
 *      AF12:FMC/SDIO/OTG/HS       AF13:DCIM                     AF14:LCD;                      AF15:EVENTOUT
 * @note:  call
 */
void GPIO_AF_Set(GPIO_TypeDef*GPIOx, uint8_t PINx, uint8_t AFx)
{
    GPIOx->AFR[PINx >> 3] &=~(0X0F << ((PINx&0X07)*4));
    GPIOx->AFR[PINx >> 3] |= (dword_t)
    AFx << ((PINx&0X07)*4);
}


