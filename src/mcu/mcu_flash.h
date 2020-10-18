/**
 * @file    mcu_flash.h
 * @author  meng_yu
 * @brief   mcu inside flash function header file
 * @version 0.0.1
 * @date    2020-10-18
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */

#ifndef _MCU_FLASH_H_
#define _MCU_FLASH_H_

/*-----------------------------------------------------------------------------------
  Exported types
-----------------------------------------------------------------------------------*/

enum flash_status
{
    FLASH_STS_NOAMAL = 0,
    FLASH_STS_BUSY,
    FLASH_STS_ABNOAMAL,
    FLASH_STS_TIMEOUT = 0xFF,
};

/*-----------------------------------------------------------------------------------
  Exported macro
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported variables
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported functions
-----------------------------------------------------------------------------------*/


//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000     //STM32 FLASH����ʼ��ַ

#define FLASH_SIZE      (512*1024)      /* Flash ���� */

#define FLASH_SAVE_ADDR  0x08060000     //����FLASH �����ַ(����Ϊ4�ı���������������,Ҫ���ڱ�������ռ�õ�������.
                                        //����,д������ʱ��,���ܻᵼ�²�����������,�Ӷ����𲿷ֳ���ʧ.��������.

#ifndef FLASH_KEY1
#define FLASH_KEY1                 0x45670123U
#endif
#ifndef FLASH_KEY2
#define FLASH_KEY2                 0xCDEF89ABU
#endif
#ifndef FLASH_OPT_KEY1
#define FLASH_OPT_KEY1             0x08192A3BU
#endif
#ifndef FLASH_OPT_KEY2
#define FLASH_OPT_KEY2             0x4C5D6E7FU
#endif

//FLASH SECTOR START ADDRESS
#define ADDR_FLASH_SECTOR_0     ((dword_t)0x08000000)       //����0��ʼ��ַ, 16 Kbytes
#define ADDR_FLASH_SECTOR_1     ((dword_t)0x08004000)       //����1��ʼ��ַ, 16 Kbytes
#define ADDR_FLASH_SECTOR_2     ((dword_t)0x08008000)       //����2��ʼ��ַ, 16 Kbytes
#define ADDR_FLASH_SECTOR_3     ((dword_t)0x0800C000)       //����3��ʼ��ַ, 16 Kbytes
#define ADDR_FLASH_SECTOR_4     ((dword_t)0x08010000)       //����4��ʼ��ַ, 64 Kbytes
#define ADDR_FLASH_SECTOR_5     ((dword_t)0x08020000)       //����5��ʼ��ַ, 128 Kbytes
#define ADDR_FLASH_SECTOR_6     ((dword_t)0x08040000)       //����6��ʼ��ַ, 128 Kbytes
#define ADDR_FLASH_SECTOR_7     ((dword_t)0x08060000)       //����7��ʼ��ַ, 128 Kbytes
#define ADDR_FLASH_SECTOR_8     ((dword_t)0x08080000)       //����8��ʼ��ַ, 128 Kbytes
#define ADDR_FLASH_SECTOR_9     ((dword_t)0x080A0000)       //����9��ʼ��ַ, 128 Kbytes
#define ADDR_FLASH_SECTOR_10    ((dword_t)0x080C0000)       //����10��ʼ��ַ,128 Kbytes
#define ADDR_FLASH_SECTOR_11    ((dword_t)0x080E0000)       //����11��ʼ��ַ,128 Kbytes

#define ADDR_FLASH_SECTOR_12    ((dword_t)0x08100000)       //����12��ʼ��ַ, 16 Kbytes
#define ADDR_FLASH_SECTOR_13    ((dword_t)0x08104000)       //����13��ʼ��ַ, 16 Kbytes
#define ADDR_FLASH_SECTOR_14    ((dword_t)0x08108000)       //����14��ʼ��ַ, 16 Kbytes
#define ADDR_FLASH_SECTOR_15    ((dword_t)0x0810C000)       //����15��ʼ��ַ, 16 Kbytes
#define ADDR_FLASH_SECTOR_16    ((dword_t)0x08110000)       //����16��ʼ��ַ, 64 Kbytes
#define ADDR_FLASH_SECTOR_17    ((dword_t)0x08120000)       //����17��ʼ��ַ, 128 Kbytes
#define ADDR_FLASH_SECTOR_18    ((dword_t)0x08140000)       //����18��ʼ��ַ, 128 Kbytes
#define ADDR_FLASH_SECTOR_19    ((dword_t)0x08160000)       //����19��ʼ��ַ, 128 Kbytes
#define ADDR_FLASH_SECTOR_20    ((dword_t)0x08180000)       //����20��ʼ��ַ, 128 Kbytes
#define ADDR_FLASH_SECTOR_21    ((dword_t)0x081A0000)       //����21��ʼ��ַ, 128 Kbytes
#define ADDR_FLASH_SECTOR_22    ((dword_t)0x081C0000)       //����22��ʼ��ַ, 128 Kbytes
#define ADDR_FLASH_SECTOR_23    ((dword_t)0x081E0000)       //����23��ʼ��ַ, 128 Kbytes


void mcu_flash_unlock(void);
void mcu_flash_lock(void);
byte_t mcu_flash_get_status(void);
byte_t mcu_flash_wait_done(dword_t time);
byte_t mcu_flash_erase_sector(dword_t sector_addr);
byte_t mcu_flash_write_dword(dword_t faddr, dword_t dat);
dword_t mcu_flash_read_dword(dword_t faddr);
void mcu_flash_write(dword_t WriteAddr,dword_t *pBuffer,dword_t NumToWrite);
void mcu_flash_read(dword_t ReadAddr,dword_t *pBuffer,dword_t NumToRead);
void mcu_flash_test_write(dword_t wr_addr, dword_t wr_dat);


#endif /* _MCU_FLASH_H_ */



