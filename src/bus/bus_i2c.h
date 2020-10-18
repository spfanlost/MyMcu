#ifndef __BUS_I2C_H_
#define __BUS_I2C_H_

#include "p_list.h"
#include "petite_def.h"


/*	i2c设备定义 */
typedef struct
{
	PetiteNode pnode;

	/*设备需要的资源，模拟I2C只需要两根IO口*/
	MCU_PORT sclport;
	u16 sclpin;

	MCU_PORT sdaport;
	u16 sdapin;
}DevI2c;

/*  设备节点 */
typedef struct
{
	s32 gd;
	
	
	DevI2c dev;	

	struct list_head list;
}DevI2cNode;

#define MCU_I2C_MODE_W 0
#define MCU_I2C_MODE_R 1

extern s32 bus_i2c_register(const DevI2c * dev);
extern DevI2cNode *bus_i2c_open(char *name);
extern s32 bus_i2c_close(DevI2cNode *node);
extern s32 bus_i2c_transfer(DevI2cNode *node, u8 addr, u8 rw, u8* data, s32 datalen);

#endif

