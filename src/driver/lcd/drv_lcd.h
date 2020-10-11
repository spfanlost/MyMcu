#ifndef _DEV_LCD_H_
#define _DEV_LCD_H_

#include "bus_lcd.h"
#include "petite_def.h"

#include "font.h"

typedef struct _strDevLcdNode DevLcdNode;
/*
	LCD驱动定义
*/
typedef struct  
{	
	u16 id;
	/*初始化*/
	s32 (*init)(DevLcdNode *lcd);
	/*
		draw_point、color_fill、fill
	*/
	s32 (*draw_point)(DevLcdNode *lcd, u16 x, u16 y, u16 color);
	s32 (*color_fill)(DevLcdNode *lcd, u16 sx,u16 ex,u16 sy,u16 ey, u16 color);
	s32 (*fill)(DevLcdNode *lcd, u16 sx,u16 ex,u16 sy,u16 ey,u16 *color);
	/*prepare_display指定显示区域后，用flush将数据持续更新到LCD*/
	s32 (*prepare_display)(DevLcdNode *lcd, u16 sx, u16 ex, u16 sy, u16 ey);
	s32 (*flush)(DevLcdNode *lcd, u16 *color, u32 len);
	
	s32 (*onoff)(DevLcdNode *lcd, u8 sta);
	void (*set_dir)(DevLcdNode *lcd, u8 scan_dir);
	void (*backlight)(DevLcdNode *lcd, u8 sta);

	/*
		update配合draw_point/color_fill/fill使用，如果是没有gram的，相当于空操作。
	*/
	s32 (*update)(DevLcdNode *lcd);
}_lcd_drv; 


/*
	设备定义
	包含挂载方式定义
	也就是说明有一什么ID的设备挂载什么地方
	例如定义一个COG LCD挂载在SPI3上
	用什么驱动？LCD具体参数是什么？通过ID匹配
	同一个类型的LCD，驱动相同，只是像素大小不一样，如何处理？
	可以重生一个驱动结构体，函数一样，ID不一样。
*/
typedef struct
{
	PetiteNode pnode;
	
	char buslcd[DEV_NAME_SIZE]; //挂在那条LCD总线上
	u16 id;
	u16 width;	//LCD 宽度   竖屏
	u16 height;	//LCD 高度    竖屏
}DevLcd;
/* 设备节点*/
struct _strDevLcdNode
{
	s32 gd;//句柄，控制是否可以打开
	
	DevLcd	dev;

	/* LCD驱动 */
	_lcd_drv *drv;

	/*驱动需要的变量*/
	u8  dir;	//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u8  scandir;//扫描方向
	u16 width;	//LCD 宽度 
	u16 height;	//LCD 高度

	void *pri;//私有数据，黑白屏跟OLED屏在初始化的时候会开辟显存

	DevLcdBusNode *busnode;

	struct list_head list;
};


#define H_LCD 0//竖屏
#define W_LCD 1//横屏

//扫描方向定义
//BIT 0 标识LR，1 R-L，0 L-R
//BIT 1 标识UD，1 D-U，0 U-D
//BIT 2 标识LR/UD，1 DU-LR，0 LR-DU
#define LR_BIT_MASK 0X01
#define UD_BIT_MASK 0X02
#define LRUD_BIT_MASK 0X04

#define L2R_U2D  (0) //从左到右,从上到下
#define L2R_D2U  (0 + UD_BIT_MASK)//从左到右,从下到上
#define R2L_U2D  (0 + LR_BIT_MASK) //从右到左,从上到下
#define R2L_D2U  (0 + UD_BIT_MASK + LR_BIT_MASK) //从右到左,从下到上

#define U2D_L2R  (LRUD_BIT_MASK)//从上到下,从左到右
#define U2D_R2L  (LRUD_BIT_MASK + LR_BIT_MASK) //从上到下,从右到左
#define D2U_L2R  (LRUD_BIT_MASK + UD_BIT_MASK) //从下到上,从左到右
#define D2U_R2L  (LRUD_BIT_MASK + UD_BIT_MASK+ LR_BIT_MASK) //从下到上,从右到左	 

//画笔颜色
/*
	对于黑白屏
	WHITE就是不显示，清空
	BLACK就是显示
*/
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  

#define BLUE         	 0x001F  
#define GREEN         	 0x07E0
#define RED           	 0xF800

#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define MAGENTA       	 0xF81F
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LIGHTGRAY      0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

extern s32 dev_lcd_register(const DevLcd *dev);
extern DevLcdNode *dev_lcd_open(char *name);
extern s32 dev_lcd_close(DevLcdNode *node);
extern s32 dev_lcd_drawpoint(DevLcdNode *lcd, u16 x, u16 y, u16 color);
extern s32 dev_lcd_prepare_display(DevLcdNode *lcd, u16 sx, u16 ex, u16 sy, u16 ey);
extern s32 dev_lcd_fill(DevLcdNode *lcd, u16 sx,u16 ex,u16 sy,u16 ey,u16 *color);
extern s32 dev_lcd_color_fill(DevLcdNode *lcd, u16 sx,u16 ex,u16 sy,u16 ey,u16 color);
extern s32 dev_lcd_backlight(DevLcdNode *lcd, u8 sta);
extern s32 dev_lcd_display_onoff(DevLcdNode *lcd, u8 sta);
extern s32 dev_lcd_setdir(DevLcdNode *node, u8 dir, u8 scan_dir);

extern s32 dev_lcd_put_string(DevLcdNode *lcd, char *font, int x, int y, char *s, unsigned colidx);

extern void put_string_center(DevLcdNode *lcd, int x, int y, char *s, unsigned colidx);
extern s32 dev_lcd_setdir(DevLcdNode *lcd, u8 dir, u8 scan_dir);
extern s32 dev_lcd_update(DevLcdNode *lcd);

#endif

