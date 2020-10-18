/**
 * @file            dev_lcdbus.c
 * @brief           对各种LCD接口封装
 * @author          wujique
 * @date            2018年4月18日 星期三
 * @version         初稿
 * @par             版权所有 (C), 2013-2023
 * @par History:
 * 1.日    期:        2018年4月18日 星期三
 *   作    者:         wujique
 *   修改内容:   创建文件
	1 源码归屋脊雀工作室所有。
	2 可以用于的其他商业用途（配套开发板销售除外），不须授权。
	3 屋脊雀工作室不对代码功能做任何保证，请使用者自行测试，后果自负。
	4 可随意修改源码并分发，但不可直接销售本代码获利，并且请保留WUJIQUE版权说明。
	5 如发现BUG或有优化，欢迎发布更新。请联系：code@wujique.com
	6 使用本源码则相当于认同本版权说明。
	7 如侵犯你的权利，请联系：code@wujique.com
	8 一切解释权归屋脊雀工作室所有。
*/
#include <stdarg.h>
#include <stdio.h>

#include "mcu.h"
#include "mcu_io.h"
#include "log.h"
#include "p_list.h"
#include "alloc.h"

#include "bus_i2c.h"
#include "bus_spi.h"
#include "bus_lcd.h"

#include "mcu_fsmc.h"

/*
	一个LCD接口
	除了通信的接口
	还有其他不属于通信接口的信号
*/

/*LCD 总线设备节点根节点*/
struct list_head DevBusLcdRoot = {&DevBusLcdRoot, &DevBusLcdRoot};	
/**
 *@brief:      bus_lcd_IO_init
 *@details:    初始化lcd接口IO
 *@param[in]   const DevLcdBus *dev  
 *@param[out]  无
 *@retval:     
 */
static void bus_lcd_IO_init(const DevLcdBus *dev) 
{

	if (dev->pnode.type == BUS_LCD_I2C) return;

	/* 初始化管脚 */
	if (dev->A0port != NULL) {
		mcu_io_config_out(dev->A0port,dev->A0pin);
		mcu_io_output_setbit(dev->A0port,dev->A0pin);
	}
	
	if (dev->rstport != NULL) {
		mcu_io_config_out(dev->rstport,dev->rstpin);
		mcu_io_output_setbit(dev->rstport,dev->rstpin);
	}
	
	if (dev->blport != NULL) {
		mcu_io_config_out(dev->blport,dev->blpin);
		mcu_io_output_setbit(dev->blport,dev->blpin);
	}
}
/**
 *@brief:     
 *@details:   
 *@param[in]    
 *@param[out]  
 *@retval:     
 */
s32 bus_lcd_bl(DevLcdBusNode *node, u8 sta)
{
	if (sta ==MCU_IO_STA_1) {
		mcu_io_output_setbit(node->dev.blport, node->dev.blpin);
	} else {
		mcu_io_output_resetbit(node->dev.blport, node->dev.blpin);	
	}
	return 0;
}
/**
 *@brief:     
 *@details:   
 *@param[in]    
 *@param[out]  
 *@retval:     
 */
s32 bus_lcd_rst(DevLcdBusNode *node, u8 sta)
{
	if (sta ==MCU_IO_STA_1) {
		mcu_io_output_setbit(node->dev.rstport, node->dev.rstpin);
	} else {
		mcu_io_output_resetbit(node->dev.rstport, node->dev.rstpin);	
	}
	return 0;
}
/**
 *@brief:     
 *@details:   
 *@param[in]    
 *@param[out]  
 *@retval:     
 */
DevLcdBusNode *bus_lcd_open(char *name)
{
	/*找设备*/
	DevLcdBusNode *node;
	struct list_head *listp;

	//wjq_log(LOG_INFO, "lcd bus name:%s!\r\n", name);
	
	listp = DevBusLcdRoot.next;
	node = NULL;
	
	while(1) {
		if (listp == &DevBusLcdRoot) break;

		node = list_entry(listp, DevLcdBusNode, list);
		//wjq_log(LOG_INFO, "lcd bus name:%s!\r\n", node->dev.name);
		
		if(strcmp (name, node->dev.pnode.name) == 0) {
			break;
		} else {
			node = NULL;
		}
		
		listp = listp->next;
	}

	if (node != NULL) {
		if (node->gd == 0) {
			wjq_log(LOG_INFO, "lcd bus open err:using!\r\n");
			node = NULL;
		} else {
			
			if (node->dev.pnode.type == BUS_LCD_SPI) {
				node->basenode = (void *)bus_spich_open(node->dev.basebus, SPI_MODE_3, 10000);

			} else if (node->dev.pnode.type == BUS_LCD_I2C) {
				node->basenode = bus_i2c_open(node->dev.basebus);
			} else if (node->dev.pnode.type == BUS_LCD_8080) {
				/*8080特殊处理*/
				node->basenode = (void *)1;
			}
			
			if (node->basenode == NULL) {
				wjq_log(LOG_INFO, "lcd bus open base bus err!\r\n");	
				node =  NULL;
			} else {
				node->gd = 0;

			}
		}
	}else {
		wjq_log(LOG_INFO, "lcd bus open err:%s!\r\n", name);
	}

	return node;
}
/**
 *@brief:     
 *@details:   
 *@param[in]    
 *@param[out]  
 *@retval:     
 */

s32 bus_lcd_close(DevLcdBusNode *node)
{
	if(node->gd != 0)return -1;
	
	if(node->dev.pnode.type == BUS_LCD_SPI){
		bus_spich_close((DevSpiChNode *)node->basenode);
		
	}else if(node->dev.pnode.type == BUS_LCD_I2C){
		bus_i2c_close((DevI2cNode *)node->basenode);	
	}else if(node->dev.pnode.type == BUS_LCD_8080){
		/*8080特殊处理*/
		node->basenode = NULL;
	}
	
	node->gd = -1;
	
	return 0;
}


/**
 *@brief:     
 *@details:   
 *@param[in]    
 *@param[out]  
 *@retval:     
 */
s32 bus_lcd_write_data(DevLcdBusNode *node, u8 *data, u32 len)
{
	/*直接定义256字节，可能有BUG，要根据len动态申请*/
	u8 tmp[256];
	u32 i;

	switch(node->dev.pnode.type)
	{
		case BUS_LCD_SPI:
		{
			bus_spich_transfer((DevSpiChNode *)node->basenode,  data, NULL, len);
		}
		break;
		case BUS_LCD_I2C:
		{
			tmp[0] = 0x40;
			memcpy(&tmp[1], data, len);
			bus_i2c_transfer((DevI2cNode *)node->basenode, 0x3C, MCU_I2C_MODE_W, tmp, len+1);
		}
		break;
		case BUS_LCD_8080:			
		{
			u16 *p;
			p = (u16 *)data;
			for(i=0; i<len; i++)
			{
				*LcdData = *(p+i);	
			}
		}
		break;
		default:
			break;
	}
	
	return 0;
}


s32 bus_lcd_w_data(DevLcdBusNode *node, u16 color, u32 len)
{

	u32 i;

	switch(node->dev.pnode.type)
	{
		case BUS_LCD_SPI:
		{

		}
		break;
		case BUS_LCD_I2C:
		{

		}
		break;
		case BUS_LCD_8080:			
		{
			for(i=len; i>0; i--)
			{
				*LcdData = color;	
			}
		}
		break;
		default:
			break;
	}
	
	return 0;
}

#if 0//FSMC尝试DMA
#define FSMC_LCD_ADDRESS      0x6C010000

void bus_lcd_fsmc_dma_init(u32 MemoryAddr,	u32 len)
{
	DMA_InitTypeDef  DMA_InitStructure;
	
	/* DMA2 Stream1 Configuration */
	DMA_DeInit(DMA2_Stream0);
	while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE)
  	{
  		//wjq_log(LOG_DEBUG, "W-");
  	}
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;	
	DMA_InitStructure.DMA_PeripheralBaseAddr = MemoryAddr;   
	DMA_InitStructure.DMA_Memory0BaseAddr = FSMC_LCD_ADDRESS;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;
	DMA_InitStructure.DMA_BufferSize = len*2;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	/* DMA2 IRQ channel Configuration */
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	
	DMA_Cmd(DMA2_Stream0, ENABLE); 

	while ((DMA_GetCmdStatus(DMA2_Stream0) != ENABLE))
  	{
  		//wjq_log(LOG_DEBUG, "E-");
  	}
}


/**
 *@brief:     非阻塞方式发送数据
 *@details:   
 *@param[in]    
 *@param[out]  
 *@retval:     
 			测试8080非阻塞发送，对刷图性能提升不大，暂不启用。
 */
s32 bus_lcd_flush_data(DevLcdBusNode *node, u8 *data, u32 len)
{
	/*直接定义256字节，可能有BUG，要根据len动态申请*/
	u8 tmp[256];
	u32 i;

	switch(node->dev.type)
	{
		case LCD_BUS_SPI:
		{
			mcu_spi_transfer((DevSpiChNode *)node->basenode,  data, NULL, len);
		}
		break;
		
		case LCD_BUS_I2C:
		{
			tmp[0] = 0x40;
			memcpy(&tmp[1], data, len);
			mcu_i2c_transfer((DevI2cNode *)node->basenode, 0x3C, MCU_I2C_MODE_W, tmp, len+1);
		}
		break;
		
		case LCD_BUS_8080:			
		{
			bus_lcd_fsmc_dma_init((u32)data, len);	
			
			while(1)
			{
				//wjq_log(LOG_DEBUG, "W");
				if(SET == DMA_GetFlagStatus(DMA2_Stream0, DMA_FLAG_TCIF0))
				{
					DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);
					break;
				}
			}
		}
		break;
		
		default:
			break;
	}
	
	return 0;
}
/**
 *@brief:  等待flush数据完成   
 *@details:   
 *@param[in]    
 *@param[out]  
 *@retval:     
 */
s32 bus_lcd_flush_wait(DevLcdBusNode *node)
{
	switch(node->dev.type)
	{
		case LCD_BUS_SPI:
		{

		}
		break;
		case LCD_BUS_I2C:
		{

		}
		break;
		case LCD_BUS_8080:			
		{
			if(DMA_GetCmdStatus(DMA2_Stream0) == DISABLE)
			{
				DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);
				return 0;
			}
			while(1)
			{
				//wjq_log(LOG_DEBUG, "W");
				if(SET == DMA_GetFlagStatus(DMA2_Stream0, DMA_FLAG_TCIF0))
				{
					DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);
					break;
				}
			}			
		}
		break;
		default:
			break;
	}
	
	return 0;
}
#endif
/**
 *@brief:     
 *@details:   
 *@param[in]    
 *@param[out]  
 *@retval:     
 */
s32 bus_lcd_read_data(DevLcdBusNode *node, u8 *data, u32 len)
{
	u32 i;
	
	switch(node->dev.pnode.type)
	{
		case BUS_LCD_SPI:
		{	
			bus_spich_transfer((DevSpiChNode *)node->basenode,  NULL, data, len);
		}
		break;
		
		case BUS_LCD_I2C:
		{	

		}
		break;
		
		case BUS_LCD_8080:
		{
			u16 *p;
			p = (u16 *)data;
			
			for(i=0; i<len; i++){
				*(p+i) = *LcdData;	
			}
		}
		break;
		default:
			break;
	}
	return 0;	
}
/**
 *@brief:     
 *@details:   
 *@param[in]    
 *@param[out]  
 *@retval:     
 */
s32 bus_lcd_write_cmd(DevLcdBusNode *node, u16 cmd)
{
	u8 tmp[2];

	switch(node->dev.pnode.type)
	{
		case BUS_LCD_SPI:
		{	
			mcu_io_output_resetbit(node->dev.A0port, node->dev.A0pin);
			tmp[0] = cmd;
			bus_spich_transfer((DevSpiChNode *)node->basenode,  &tmp[0], NULL, 1);
			mcu_io_output_setbit(node->dev.A0port, node->dev.A0pin);
		}
		break;
		
		case BUS_LCD_I2C:
		{	
			tmp[0] = 0x00;
			tmp[1] = cmd;
			
			bus_i2c_transfer((DevI2cNode *)node->basenode, 0x3C, MCU_I2C_MODE_W, tmp, 2);
		}
		break;
		
		case BUS_LCD_8080:
		{
			*LcdReg = cmd;	
		}
		break;
		default:
			break;
	}
	return 0;
}
/**
 *@brief:      dev_lcdbus_register
 *@details:    注册LCD接口
 *@param[in]   void  
 *@param[out]  无
 *@retval:     
 */
s32 dev_lcdbus_register(const DevLcdBus *dev)
{
	struct list_head *listp;
	DevLcdBusNode *p;

	wjq_log(LOG_INFO, "[register] lcd bus :%s, base on:%s!\r\n", dev->pnode.name, dev->basebus);

	/*	先要查询当前，防止重名 	*/
	listp = DevBusLcdRoot.next;
	while(1) {
		if (listp == &DevBusLcdRoot) break;

		p = list_entry(listp, DevLcdBusNode, list);

		if (strcmp(dev->pnode.name, p->dev.pnode.name) == 0) {
			wjq_log(LOG_INFO, "bus lcd dev name err!\r\n");
			return -1;
		}
		
		listp = listp->next;
	}

	/* 申请一个节点空间    	*/
	p = (DevLcdBusNode *)wjq_malloc(sizeof(DevLcdBusNode));
	list_add(&(p->list), &DevBusLcdRoot);
	/*复制设备信息*/
	memcpy((u8 *)&p->dev, (u8 *)dev, sizeof(DevLcdBus));
	p->gd = -1;

	/*初始化*/
	bus_lcd_IO_init(dev);

	if(dev->pnode.type == BUS_LCD_8080){
		//初始FSMC
		mcu_fsmc_lcd_Init();
	}
	return 0;
}





