/**
 * @file    mcu_isr.h
 * @author  meng_yu
 * @brief   &#& function header file
 * @version 0.0.1
 * @date    2020-09-29
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */

#ifndef _MCU_ISR_H_
#define _MCU_ISR_H_

/*-----------------------------------------------------------------------------------
  Exported types
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported macro
-----------------------------------------------------------------------------------*/
//NVIC_PreemptionPriority:��ռ���ȼ�
//NVIC_SubPriority       :��Ӧ���ȼ�
//NVIC_Group             :�жϷ��� 0~4
//ע�����ȼ����ܳ����趨����ķ�Χ!����������벻���Ĵ���
enum nvic_group
{
    NVIC_GROUP0,//group0:0 bit Preemption Priority,4 bit SubPriority
    NVIC_GROUP1,//group1:1 bit Preemption Priority,3 bit SubPriority
    NVIC_GROUP2,//group2:2 bit Preemption Priority,2 bit SubPriority
    NVIC_GROUP3,//group3:3 bit Preemption Priority,1 bit SubPriority
    NVIC_GROUP4,//group4:4 bit Preemption Priority,0 bit SubPriority
};

/*-----------------------------------------------------------------------------------
  Exported variables
-----------------------------------------------------------------------------------*/
extern volatile qword_t ticks;

/*-----------------------------------------------------------------------------------
  Exported functions
-----------------------------------------------------------------------------------*/
void NVIC_Config(IRQn_Type IRQn, byte_t PreemptionPrio, byte_t SubPrio, byte_t Enable);


#endif /* _ISR_H_ */


