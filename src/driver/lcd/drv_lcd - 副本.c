
#include "mcu.h"
#include "petite_config.h"
#include "board_sysconf.h"
#include "log.h"
#include "p_list.h"
#include "alloc.h"
#include "vfs.h"

#include "bus_lcd.h"
#include "drv_lcd.h"

#include "drv_ILI9341.h"
#include "drv_str7565.h"
#include "drv_IL91874.h"
#include "drv_IL3820.h"
#include "drv_ILI9325.h"
#include "drv_st7789.h"
#include "drv_st7735r.h"
#include "drv_nt35510.h"
#include "drv_r61408.h"

#define DEV_LCD_DEBUG

#ifdef DEV_LCD_DEBUG
#define LCD_DEBUG	wjq_log 
#else
#define LCD_DEBUG(a, ...)
#endif

u16 PenColor = BLACK;
u16 BackColor = BLUE;

/*	LCD驱动列表 */
_lcd_drv *LcdDrvList[] = {
				/* tft lcd ILI9341 */
				#if( LCD_DRIVER_9341 == 1 )
					&TftLcdILI9341Drv,   
				#endif
				/* tft lcd ILI9325 */
				#if( LCD_DRIVER_9325 == 1 )	
					&TftLcdILI9325Drv,
				#endif
				/* COG LCD  */
				#if( LCD_DRIVER_ST7565 == 1 )
					&CogLcdST7565Drv,
				#endif
				/* oled  */
				#if( LCD_DRIVER_SSD1615 == 1 )
					&OledLcdSSD1615rv,
				#endif
				/* tft lcd, 不是并口，SPI 或I2C 或8BIT  */
				#if( LCD_DRIVER_9341_8BIT == 1 )
					&TftLcdILI9341_8_Drv,
				#endif
				/* 1.44寸 中景园 tft lcd  */
				#if( LCD_DRIVER_7735 == 1 )
					&TftLcdST7735R_Drv,
				#endif
				/* 1.3寸，IPS，中景园，只有SCL&SDA的SPI接口LCD*/
				#if( LCD_DRIVER_7789 == 1 )
					&TftLcdST7789_Drv,
				#endif
				/*91874 大连佳显 2.7寸电子纸 三色屏*/
				#if( LCD_DRIVER_91874 == 1 )
					&TftLcdIL91874Drv,
				#endif
				/*3820 大连佳显 GDEH154D27*/
				#if( LCD_DRIVER_3820 == 1 )	
					&TftLcdIL3820Drv,
				#endif

				/*全动 NT35510 */
				#if( LCD_DRIVER_NT35510 == 1 )	
					&TftLcdNT35510Drv,
				#endif
				/* 好矩润 R61408*/
				#if( LCD_DRIVER_R61408 == 1 )	
					&TftLcdR61408Drv,
				#endif
					
};
/*	可自动识别ID的驱动*/
_lcd_drv *LcdProbDrvList[] = {
				#if( LCD_DRIVER_9341 == 1 )
					&TftLcdILI9341Drv,
				#endif
				#if( LCD_DRIVER_9325 == 1 )	
					&TftLcdILI9325Drv,
				#endif
				
				/*全动 NT35510 */
				#if( LCD_DRIVER_NT35510 == 1 )	
					&TftLcdNT35510Drv,
				#endif

				#if( LCD_DRIVER_R61408 == 1 )	
					&TftLcdR61408Drv,
				#endif
};
/**
 *@brief:      dev_lcd_finddrv
 *@details:    根据ID查找设备驱动
 *@param[in]   u16 id  
 *@param[out]  无
 *@retval:     _lcd_drv
 */
static _lcd_drv *dev_lcd_finddrv(u16 id)
{
	u8 i =0;
	
	while(1){
		if(LcdDrvList[i]->id == id){
			return LcdDrvList[i];
		}
		i++;
		if(i>= sizeof(LcdDrvList)/sizeof(_lcd_drv *)){
			return NULL;
		}
	}
}

struct list_head DevLcdRoot = {&DevLcdRoot, &DevLcdRoot};	
/**
 *@brief:      dev_lcd_register
 *@details:    注册LCD设备
 *@param[in]   
 *@param[out]  
 *@retval:     
 */
s32 dev_lcd_register(const DevLcd *dev)
{
	struct list_head *listp;
	DevLcdNode *plcdnode;
	s32 ret = -1;
	
	wjq_log(LOG_INFO, "[register] lcd :%s, base on:%s!\r\n", dev->pnode.name, dev->buslcd);

	/*
		先要查询当前，防止重名
	*/
	listp = DevLcdRoot.next;
	while(1) {
		if (listp == &DevLcdRoot) break;

		plcdnode = list_entry(listp, DevLcdNode, list);

		if (strcmp(dev->pnode.name, plcdnode->dev.pnode.name) == 0) {
			wjq_log(LOG_INFO, "lcd dev name err!\r\n");
			return -1;
		}

		if (strcmp(dev->buslcd, plcdnode->dev.buslcd) == 0) {
			wjq_log(LOG_INFO, "one lcd bus just for one lcd!\r\n");
			return -1;
		}
		
		listp = listp->next;
	}

	/* 
		申请一个节点空间 
	*/
	plcdnode = (DevLcdNode *)wjq_malloc(sizeof(DevLcdNode));
	list_add(&(plcdnode->list), &DevLcdRoot);
	
	/*复制设备信息*/
	memcpy((u8 *)&plcdnode->dev, (u8 *)dev, sizeof(DevLcd));
	plcdnode->gd = -1;

	/* 初始化 */
	if (dev->id == NULL) {
		LCD_DEBUG(LOG_DEBUG, "prob LCD id\r\n");

		/*找到驱动跟规格后，初始化*/
		u8 j = 0;

		while(1) {
			ret = LcdProbDrvList[j]->init(plcdnode);
			if (ret == 0) {
				LCD_DEBUG(LOG_DEBUG, "lcd drv prob ok!\r\n");	
				plcdnode->drv = LcdProbDrvList[j];
				break;
			} else {
				j++;
				if (j >= sizeof(LcdProbDrvList)/sizeof(_lcd_drv *)) {
					LCD_DEBUG(LOG_DEBUG, "lcd prob err\r\n");
					break;
				}
			}
		}
	}else {
		ret = -1;
		LCD_DEBUG(LOG_DEBUG, "find lcd drv, id:%04x...", dev->id);
		plcdnode->drv = dev_lcd_finddrv(dev->id);
		if (plcdnode->drv != NULL) {
			/*找到驱动跟规格后，初始化*/
			LCD_DEBUG(LOG_DEBUG, "suc!\r\n");
			ret = plcdnode->drv->init(plcdnode);
		} else {
			LCD_DEBUG(LOG_DEBUG, "fail!\r\n");
		}
	}

	if(ret == 0) {
		plcdnode->gd = -1;
		
		plcdnode->dir = H_LCD;
		
		plcdnode->height = plcdnode->dev.height;
		plcdnode->width = plcdnode->dev.width;
		
		dev_lcd_setdir(plcdnode, W_LCD, L2R_U2D);
		
		plcdnode->drv->onoff((plcdnode),1);//打开显示
		
		plcdnode->drv->color_fill(plcdnode, 0, plcdnode->width, 0, plcdnode->height, BLUE);
		plcdnode->drv->update(plcdnode);
		plcdnode->drv->backlight(plcdnode, 1);

		wjq_log(LOG_INFO, "lcd init OK\r\n");
	} else {
		plcdnode->gd = -2;
		wjq_log(LOG_INFO, "lcd drv init err!\r\n");
	}
	
	return 0;
}


/**
 *@brief:      dev_lcd_open
 *@details:    打开LCD
 *@param[in]   char *name  
 *@param[out]  无
 *@retval:     DevLcd
 */
DevLcdNode *dev_lcd_open(char *name)
{

	DevLcdNode *node;
	struct list_head *listp;
	
	//LCD_DEBUG(LOG_INFO, "lcd open:%s!\r\n", name);

	listp = DevLcdRoot.next;
	node = NULL;
	
	while(1) {
		if (listp == &DevLcdRoot) break;

		node = list_entry(listp, DevLcdNode, list);
		//LCD_DEBUG(LOG_INFO, "lcd name:%s!\r\n", node->dev.name);
		
		if (strcmp(name, node->dev.pnode.name) == 0) {
			//LCD_DEBUG(LOG_INFO, "lcd dev get ok!\r\n");
			break;
		} else {
			node = NULL;
		}
		
		listp = listp->next;
	}

	if (node != NULL) {
		if (node->gd > (-2)) node->gd++;
		else return NULL;
	}
	
	return node;
}

/**
 *@brief:      dev_lcd_close
 *@details:    关闭LCD
 *@param[in]   DevLcd *dev  
 *@param[out]  无
 *@retval:     
 */
s32 dev_lcd_close(DevLcdNode *node)
{
	if(node->gd <0)	return -1;
	else{
		node->gd -= 1;
		return 0;
	}
}
/*
坐标-1 是坐标原点的变化，
在APP层，原点是（1，1），这样更符合平常人。

到驱动就换为(0,0)，无论程序还是控制器显存，都是从（0，0）开始

*/
s32 dev_lcd_drawpoint(DevLcdNode *lcd, u16 x, u16 y, u16 color)
{
	if (lcd == NULL)	return -1;
	
	return lcd->drv->draw_point(lcd, x-1, y-1, color);
}

s32 dev_lcd_prepare_display(DevLcdNode *lcd, u16 sx, u16 ex, u16 sy, u16 ey)
{
	if(lcd == NULL)	return -1;
	
	return lcd->drv->prepare_display(lcd, sx-1, ex-1, sy-1, ey-1);
}


s32 dev_lcd_fill(DevLcdNode *lcd, u16 sx,u16 ex,u16 sy,u16 ey,u16 *color)
{	
	if(lcd == NULL)	return -1;
	
	return lcd->drv->fill(lcd, sx-1,ex-1,sy-1,ey-1,color);
}
s32 dev_lcd_color_fill(DevLcdNode *lcd, u16 sx,u16 ex,u16 sy,u16 ey,u16 color)
{
	if(lcd == NULL)	return -1;
	
	return lcd->drv->color_fill(lcd, sx-1,ex-1,sy-1,ey-1,color);
}
s32 dev_lcd_backlight(DevLcdNode *lcd, u8 sta)
{
	if(lcd == NULL)	return -1;
	
	lcd->drv->backlight(lcd, sta);
	return 0;
}
s32 dev_lcd_display_onoff(DevLcdNode *lcd, u8 sta)
{
	if(lcd == NULL)	return -1;

	return lcd->drv->onoff(lcd, sta);
}

s32 dev_lcd_flush(DevLcdNode *lcd, u16 *color, u32 len)
{
	if(lcd == NULL)	return -1;

	return lcd->drv->flush(lcd, color, len);	
}
s32 dev_lcd_update(DevLcdNode *lcd)
{
	if(lcd == NULL)	return -1;

	return lcd->drv->update(lcd);
}
/**
 *@brief:      dev_lcd_setdir
 *@details:    设置横屏或竖屏，扫描方向
 *@param[in]   u8 dir       0 竖屏1横屏
               u8 scan_dir  参考宏定义L2R_U2D       
 *@param[out]  无
 *@retval:     
 */
s32 dev_lcd_setdir(DevLcdNode *node, u8 dir, u8 scan_dir)
{
	u16 temp;
	u8 scan_dir_tmp;

	if (node == NULL) return -1;

	//切换屏幕方向	
	if (dir != node->dir) {
		
		node->dir = node->dir^0x01;
		temp = node->width;
		node->width = node->height;
		node->height = temp;
		LCD_DEBUG(LOG_DEBUG, "set dir w:%d, h:%d\r\n", node->width, node->height);
	}
	
	//横屏，扫描方向映射转换
	if(node->dir == W_LCD) {
		/*
			横屏	 竖屏
			LR----UD
			RL----DU
			UD----RL
			DU----LR
			UDLR----LRUD
		*/
		scan_dir_tmp = 0;
		if ((scan_dir&LRUD_BIT_MASK) == 0) {
			scan_dir_tmp += LRUD_BIT_MASK;
		}

		if ((scan_dir&LR_BIT_MASK) == LR_BIT_MASK) {
			scan_dir_tmp += UD_BIT_MASK;	
		}

		if ((scan_dir&UD_BIT_MASK) == 0) {
			scan_dir_tmp += LR_BIT_MASK;
		}
	}else {
		scan_dir_tmp = scan_dir;
	}
	
	node->scandir = scan_dir_tmp;
	
	node->drv->set_dir(node, node->scandir);
	
	return 0;
}

/* 

从tslib拷贝一些显示函数到这里
这些函数可以归为GUI
*/
#include "font.h"

/**
 *@brief:      line
 *@details:    画一条线
 *@param[in]   int x1           
               int y1           
               int x2           
               int y2           
               unsigned colidx  
 *@param[out]  无
 *@retval:     
 */
void line (DevLcdNode *lcd, int x1, int y1, int x2, int y2, unsigned colidx)
{
	int tmp;
	int dx = x2 - x1;
	int dy = y2 - y1;

	if (lcd == NULL) return;

	if (abs (dx) < abs (dy)) {
		if (y1 > y2) {
			tmp = x1; x1 = x2; x2 = tmp;
			tmp = y1; y1 = y2; y2 = tmp;
			dx = -dx; dy = -dy;
		}
		x1 <<= 16;
		/* dy is apriori >0 */
		dx = (dx << 16) / dy;
		while (y1 <= y2){
			dev_lcd_drawpoint(lcd, x1 >> 16, y1, colidx);
			x1 += dx;
			y1++;
		}
	} else {
		if (x1 > x2) {
			tmp = x1; x1 = x2; x2 = tmp;
			tmp = y1; y1 = y2; y2 = tmp;
			dx = -dx; dy = -dy;
		}
		
		y1 <<= 16;
		dy = dx ? (dy << 16) / dx : 0;
		while (x1 <= x2) {
			dev_lcd_drawpoint(lcd, x1, y1 >> 16, colidx);
			y1 += dy;
			x1++;
		}
	}
}

/**
 *@brief:     put_cross
 *@details:   画十字
 *@param[in]  int x            
              int y            
              unsigned colidx  
 *@param[out]  无
 *@retval:     
 */
void put_cross(DevLcdNode *lcd, int x, int y, unsigned colidx)
{
	if (lcd == NULL) return;
	
	line (lcd, x - 10, y, x - 2, y, colidx);
	line (lcd, x + 2, y, x + 10, y, colidx);
	line (lcd, x, y - 10, x, y - 2, colidx);
	line (lcd, x, y + 2, x, y + 10, colidx);

	line (lcd, x - 6, y - 9, x - 9, y - 9, colidx + 1);
	line (lcd, x - 9, y - 8, x - 9, y - 6, colidx + 1);
	line (lcd, x - 9, y + 6, x - 9, y + 9, colidx + 1);
	line (lcd, x - 8, y + 9, x - 6, y + 9, colidx + 1);
	line (lcd, x + 6, y + 9, x + 9, y + 9, colidx + 1);
	line (lcd, x + 9, y + 8, x + 9, y + 6, colidx + 1);
	line (lcd, x + 9, y - 6, x + 9, y - 9, colidx + 1);
	line (lcd, x + 8, y - 9, x + 6, y - 9, colidx + 1);

}
/**
 *@brief:      put_char
 *@details:    显示一个英文
 *@param[in]   int x       
               int y       
               int c       
               int colidx  
 *@param[out]  无
 *@retval:     
 */
void put_char(DevLcdNode *lcd, int x, int y, int c, int colidx)
{
	int i,j,bits;
	u8* p;
	
	if (lcd == NULL) return;	
	
	p = (u8*)font_vga_8x8.path;//need fix
	for (i = 0; i < font_vga_8x8.height; i++) {
		bits =  p[font_vga_8x8.height * c + i];
		for (j = 0; j < font_vga_8x8.width; j++, bits <<= 1) {
			if (bits & 0x80) {
				lcd->drv->draw_point(lcd, x + j, y + i, colidx);
			}
		}
	}
}
/**
 *@brief:      put_string
 *@details:    显示一个字符串
 *@param[in]   int x            
               int y            
               char *s          
               unsigned colidx  
 *@param[out]  无
 *@retval:     
 */
void put_string(DevLcdNode *lcd, int x, int y, char *s, unsigned colidx)
{
	int i;
	
	if (lcd == NULL)	return;	
	
	for (i = 0; *s; i++, x += font_vga_8x8.width, s++)
		put_char(lcd, x, y, *s, colidx);
}
/**
 *@brief:      put_string_center
 *@details:    居中显示一个字符串
 *@param[in]   int x            
               int y            
               char *s          
               unsigned colidx  
 *@param[out]  无
 *@retval:     
 */
void put_string_center(DevLcdNode *lcd, int x, int y, char *s, unsigned colidx)
{
	int sl = strlen (s);
	
	if(lcd == NULL)	return;	
	
    put_string (lcd, x - (sl / 2) * font_vga_8x8.width,
                y - font_vga_8x8.height / 2, s, colidx);
}

/**
 *@brief:      rect
 *@details:    画一个矩形框
 *@param[in]   int x1           
               int y1           
               int x2           
               int y2           
               unsigned colidx  
 *@param[out]  无
 *@retval:     
 */
void rect (DevLcdNode *lcd, int x1, int y1, int x2, int y2, unsigned colidx)
{
	if (lcd == NULL) return;

	line (lcd, x1, y1, x2, y1, colidx);
	line (lcd, x2, y1, x2, y2, colidx);
	line (lcd, x2, y2, x1, y2, colidx);
	line (lcd, x1, y2, x1, y1, colidx);
}

/**
 *@brief:      dev_lcd_put_string
 *@details:    显示字符串，支持中文
 *@param[in]   无
 *@param[out]  无
 *@retval:     	
 */
s32 dev_lcd_put_string(DevLcdNode *lcd, char *font, int x, int y, char *s, unsigned colidx)
{
	u16 slen;
	u16 xlen,ylen;
	
	u16 *framebuff;//样点缓冲，按照L2R_U2D格式填充
	u8 *dotbuf;//字符点阵缓冲
	
	s32 res;
	u16 sidx;
	u16 i,j;
	u32 xbase;//显示在x轴偏移量
	u16 fontw,fonth;
	char tmp;;
	
	if (lcd == NULL) return -1;
	
	/* 通过刷一整块，提高显示速度 */
	slen = strlen(s);
	//uart_printf("str len:%d\r\n", slen);

	font_get_hw(font, &fonth, &fontw);
	/*	根据字符串长度计算刷新区域长宽	*/
	xlen = slen*fontw;
	ylen = fonth;

	framebuff = (u16*)wjq_malloc(xlen*ylen*sizeof(u16));//样点缓冲
	dotbuf = (u8*)wjq_malloc(32);//要改为根据字库类型申请
	sidx = 0;

	/*获取点阵，并转化为LCD像素*/
	while(1) {

		if (sidx >= slen) {
			//uart_printf("finish");
			break;
		}
		
		FontDot dot;
		dot.dot = dotbuf;
		
		res = font_get_dotdata(font, s+sidx, &dot);	
		
		//PrintFormat(dotbuf, 32);
		for (j=0; j<fonth; j++) {
				
			xbase = xlen*j + sidx*fontw;//当前字符X轴偏移量

			for (i=0;i < dot.w ;i++) {
				
				if(dot.dt == FONT_V_H_U_D_L_R) {
					tmp = dotbuf[i*2+j/8]&(0x80>>(j%8));
				} else if(dot.dt == FONT_V_L_L_R_U_D) {
					tmp = dotbuf[i+j/8*dot.w]&(0x01<<(j%8));
				} else if(dot.dt == FONT_H_H_L_R_U_D) {
					tmp = dotbuf[i/8 + j*((dot.w+7)/8)]&(0x80>>(i%8));
				}
				
				if (tmp != 0) {
					//uart_printf("* ");
					framebuff[xbase + i] = colidx;
				} else {
					//uart_printf("- ");
					framebuff[xbase + i] = BackColor;
				}
			}
			//uart_printf("\r\n");
		}	

		sidx+= res;
	}


	if ( y + ylen > lcd->height) {
		/*显示超出屏幕*/
		ylen = lcd->height - y+1;//假设height = 240,y = 240, 也就意味着只显示一行
	}
	
	if (x + xlen >= lcd->width) {
		/*显示超出屏幕宽度*/
		i = lcd->width - x + 1;
		
		/*调整数据*/
		j = 1;
		while(1) {
			if (j >= ylen) break;
			memcpy(framebuff+j*i, framebuff+ j*xlen, 2*i);
			j++;
		}
		xlen = i;
	}

	dev_lcd_fill(lcd, x, x + xlen-1, y, y + ylen-1, framebuff);

	wjq_free(framebuff);
	wjq_free(dotbuf);

	return 0;	
}
extern void Delay(__IO uint32_t nTime);

#if 1
typedef struct tagBITMAPFILEHEADER  //文件头  14B  
{ 
    u16  bfType;   //0x424d, "BM"
    u32  bfSize;   //文件大小，包含文件头
    u16  bfReserved1;   //保留字节
    u16  bfReserved2;   //保留字节
    u32  bfOffBits;   	//从文件头到实际位图数据的偏移
} BITMAPFILEHEADER; 

typedef struct tagBITMAPINFOHEADER  //位图信息头
{ 
    u32 biSize;   //本结构长度，也即是40
    s32 biWidth;  //图像宽度   
    s32 biHeight; //图像高度    
    u16 biPlanes; //1  
    u16 biBitCount;//1黑白二色图，4 16位色，8 256色，24 真彩色 
    u32 biCompression;   //是否压缩
    u32 biSizeImage;   //实际位图数据字节数
    s32 biXPelsPerMeter;  //目标设备水平分辨率 
    s32 biYPelsPerMeter;   //目标设备垂直分辨率
    u32 biClrUsed;  //图像实际用到颜色数，如为0，则用到的颜色数为2的biBitCount次方
    u32 biClrImportant;  //指定本图象中重要的颜色数，如果该值为零，则认为所有的颜色都是重要的
} BITMAPINFOHEADER;

/*调色板每个元素*/
typedef struct tagRGBQUAD
{ 
	u8    rgbBlue; //蓝色分量  
	u8    rgbGreen; //绿色分量    
	u8    rgbRed;   //红色分量  
	u8    rgbReserved;    
} RGBQUAD; 


#define WIDTHBYTES(i) ((i+31)/32*4)

/*

	4种图片，刷屏时间，图片保存在SD卡

			FSMC	读数据		SPI（其中，将U16拆为U8，花10ms）
	1bit	53ms 	30  	316
	4bit	74	 	50		340
	8bit	79	 	51		344
	24bit	111	 	91		378


	要提速，还可以有下面方法：
	申请双缓冲，用DMA，启动DMA后，不等传输完成，
	就出来准备下一包数据，发送下一包前，查询上一包是否发送完成。
	这样，原来的时间：
		数据准备时间+数据传输时间
	现在时间变为：
		数据准备时间/数据传输时间，两者较长的为需要时间。
*/
s32 dev_lcd_show_bmp(DevLcdNode *lcd, u16 x, u16 y, u16 xlen, u16 ylen, s8 *BmpFileName)
{
	BITMAPFILEHEADER    bf;
    BITMAPINFOHEADER    bi;
	
	int bmpfd;
	
	u32 rlen;
    u16 LineBytes;
	u16 NumColors;
    u32 ImgSize;
	u16 buf[40];

	u32 i,j;
	u8 *palatte;
	volatile u16 color;
	u32 k, m;
	u16 r,g,b;
	u8 c;
	u16 *pcc;
	u8 *pdata;
	u8 linecnt = 20;//一次读多行，加快速度
	u8 l;
	
	wjq_log(LOG_DEBUG, "bmp open file:%s\r\n", BmpFileName);

	bmpfd = vfs_open(BmpFileName, O_RDONLY);

	if (bmpfd == NULL) {
		wjq_log(LOG_DEBUG, "bmp open file err\r\n");
		return -1;
	}
	rlen = vfs_read(bmpfd, (void *)buf, 14);
    
	bf.bfType      = buf[0];
    bf.bfSize      = buf[2];
    bf.bfSize = (bf.bfSize<<16)+buf[1];
    bf.bfReserved1 = buf[3];
    bf.bfReserved2 = buf[4];
    bf.bfOffBits   = buf[6];
    bf.bfOffBits = (bf.bfOffBits<<16)+buf[5];
	
	wjq_log(LOG_DEBUG, "bf.bfType:%x\r\n", bf.bfType);	
	wjq_log(LOG_DEBUG, "bf.bfSize:%d\r\n", bf.bfSize);
	wjq_log(LOG_DEBUG, "bf.bfOffBits:%d\r\n", bf.bfOffBits);

	rlen = vfs_read(bmpfd, (void *)buf, 40);

	bi.biSize           = (unsigned long) buf[0];
    bi.biWidth          = (long) buf[2];
    bi.biHeight         = (long) buf[4];
    bi.biPlanes         = buf[6];
    bi.biBitCount       = buf[7];
    bi.biCompression    = (unsigned long) buf[8];
    bi.biSizeImage      = (unsigned long) buf[10];
    bi.biXPelsPerMeter  = (long) buf[12];
    bi.biYPelsPerMeter  = (long) buf[14];
    bi.biClrUsed        = (unsigned long) buf[16];
    bi.biClrImportant   = (unsigned long) buf[18];

	wjq_log(LOG_DEBUG, "bi.biSize:%d\r\n", bi.biSize);	
	wjq_log(LOG_DEBUG, "bi.biWidth:%d\r\n", bi.biWidth);
	wjq_log(LOG_DEBUG, "bi.biHeight:%d\r\n", bi.biHeight);
	wjq_log(LOG_DEBUG, "bi.biPlanes:%d\r\n", bi.biPlanes);
	wjq_log(LOG_DEBUG, "bi.biBitCount:%d\r\n", bi.biBitCount);
	wjq_log(LOG_DEBUG, "bi.biCompression:%d\r\n", bi.biCompression);
	wjq_log(LOG_DEBUG, "bi.biSizeImage:%d\r\n", bi.biSizeImage);
	wjq_log(LOG_DEBUG, "bi.biXPelsPerMeter:%d\r\n", bi.biXPelsPerMeter);
	wjq_log(LOG_DEBUG, "bi.biYPelsPerMeter:%d\r\n", bi.biYPelsPerMeter);
	wjq_log(LOG_DEBUG, "bi.biClrUsed:%d\r\n", bi.biClrUsed);
	wjq_log(LOG_DEBUG, "bi.biClrImportant:%d\r\n", bi.biClrImportant);

	/*8个像素占用一个字节，不足一个字节补足一个字节*/
	/*单色图片四字节对齐*/
	LineBytes = WIDTHBYTES(bi.biWidth * bi.biBitCount);
    ImgSize   = (unsigned long) LineBytes  * bi.biHeight;

    wjq_log(LOG_DEBUG, "bmp w:%d,h:%d, bitcount:%d, linebytes:%d\r\n", bi.biWidth, bi.biHeight, bi.biBitCount, LineBytes);
	
	if(bi.biClrUsed!=0)
		NumColors=(unsigned long)bi.biClrUsed;//如果 bi.biClrUsed 不为零，就是本图象实际用到的颜色
	else {
	    switch(bi.biBitCount)
	    {
	    case 1:
	        NumColors=2;//黑白屏用到两个调色板，一个是黑一个是白
	        break;
	        
	    case 4:
	        NumColors=16;
	        break;
	        
	    case 8:
	        NumColors=256;
	        break;
	        
	    case 24:
	        NumColors=0;
	        break;
	        
	    default:
			vfs_close(bmpfd);
	        return 2;
	    }
	}

	/* 读调色板 */
	if (NumColors != 0) {
		palatte = wjq_malloc(4*NumColors);
		rlen = vfs_read(bmpfd, (void *)palatte, 4*NumColors);

	}

	if (xlen > bi.biWidth) xlen = bi.biWidth;

    if (ylen > bi.biHeight) ylen = bi.biHeight;

	pdata = wjq_malloc(LineBytes*linecnt);
	
	dev_lcd_prepare_display(lcd, x, x+xlen-1, y, y+ylen-1);
			
    switch(bi.biBitCount)
    {
    case 1:
		pcc = wjq_malloc(xlen*sizeof(u16));
		//图片取模:横向,左高右低
		for (j=0; j<ylen;) {
        	if(j+linecnt >= ylen)
				linecnt = ylen-j;

			rlen = vfs_read(bmpfd, (void *)pdata, LineBytes*linecnt);
			if (rlen != LineBytes*linecnt) {
				wjq_log(LOG_DEBUG, "bmp read data err!\r\n");
			}
			l = 0;
			while (l < linecnt) {
				k = l*LineBytes;
				#if 0//不用除法， 测试
				i = 0;				
				while(1)
	            {
					/*
            		一个字节8个像素，高位在前
            		调色板有256种颜色
					*/
		            c = pdata[k];
					
					m = 0;
					while(m <8)
					{
						if((c &(0x80>>m)) != 0)
	                		*(pcc+i)  = WHITE;
						else
							*(pcc+i)  = BLACK;	

						i++;
						if(i>= xlen)
							break;
						m++;
					}
					
					if(i>= xlen)
						break;

					k++;
	            }
				#else
				for (i=0; i<xlen; i++) {
		        	/*
		        		一个字节8个像素，高位在前
		        		调色板有256种颜色
					*/
		            c = pdata[k+(i/8)]&(0x80>>(i%8));
		            
		            if(c != 0)
						*(pcc+i) = WHITE;
		            else
						*(pcc+i) = BLACK;
		        }
				#endif
				
				dev_lcd_flush(lcd, pcc, xlen);
				l++;
			}

			j += linecnt;
        }
		wjq_free(pcc);

        break;
        
    case 4:
		pcc = wjq_malloc(xlen*sizeof(u16));
		//图片取模:横向,左高右低s
		for (j=0; j<ylen;) {
        	if (j+linecnt >= ylen)
				linecnt = ylen-j;
			
			rlen = vfs_read(bmpfd, (void *)pdata, LineBytes*linecnt);			
			if (rlen != LineBytes*linecnt) {
				wjq_log(LOG_DEBUG, "bmp read data err!\r\n");
			}
			l = 0;
			while(l < linecnt) {
				k = l*LineBytes;

				#if 0//不用除法,测试
				i = 0;
				while(1)
	            {
					/*4个bit 1个像素，要进行对U16的转换
					rgb565
					#define BLUE         	 0x001F  
					#define GREEN         	 0x07E0
					#define RED           	 0xF800
					*/
					c = *(pdata+k);
					
					m = ((c>>4)&0x0f)*4;
					
					b = (palatte[m++] & 0xF8)>>3;
					g = (palatte[m++] & 0xFC)>>2;
					r = (palatte[m] & 0xF8)>>3;
	                                
	                *(pcc+i) = (r<<11)|(g<<5)|(b<<0);
					
					i++;
					if(i>= xlen)
						break;
					
					m = (c&0x0f)*4;
					
					b = (palatte[m++] & 0xF8)>>3;
					g = (palatte[m++] & 0xFC)>>2;
					r = (palatte[m] & 0xF8)>>3;

	                *(pcc+i) = (r<<11)|(g<<5)|(b<<0);
					
					i++;
					if(i>= xlen)
						break;
					k++;
	            }
				#else
				for (i=0; i < xlen; i++) {
									/*4个bit 1个像素，要进行对U16的转换
					rgb565
	#define BLUE			 0x001F  
	#define GREEN			 0x07E0
	#define RED 			 0xF800
					*/
					m = *(pdata+k+i/2);
					
					if (i%2 == 0)
						m = ((m>>4)&0x0f);
					else
						m = (m&0x0f);
					
					m = m*4;
					
					r = (palatte[m+2] & 0xF8)>>3;
					g = (palatte[m+1] & 0xFC)>>2;
					b = (palatte[m] & 0xF8)>>3;
					
					*(pcc+i) = (r<<11)|(g<<5)|(b<<0);
				}
				#endif
				
				dev_lcd_flush(lcd, pcc, xlen);
				l++;
			}

			j += linecnt;
        }
		wjq_free(pcc);
        break;

    case 8:
		pcc = wjq_malloc(xlen*sizeof(u16));	
		//图片取模:横向,左高右低
		for (j=0; j<ylen;) {
        	if (j+linecnt >= ylen) linecnt = ylen-j;
			
			rlen = vfs_read(bmpfd, (void *)pdata, LineBytes*linecnt);	
			if (rlen != LineBytes*linecnt) {
				wjq_log(LOG_DEBUG, "bmp read data err!\r\n");
			}
			
			l = 0;
			while(l < linecnt) {
				k = l*LineBytes;
				
            	for (i=0; i < xlen; i++) {
					/*1个字节1个像素，要进行对U16的转换
					rgb565
					#define BLUE         	 0x001F  
					#define GREEN         	 0x07E0
					#define RED           	 0xF800
					*/
					m = *(pdata+k);
					k++;
					m = m*4;
					
					r = (palatte[m+2] & 0xF8)>>3;
	                g = (palatte[m+1] & 0xFC)>>2;
	                b = (palatte[m] & 0xF8)>>3;
					
	                *(pcc+i)  = (r<<11)|(g<<5)|(b<<0);
	            }
				
				dev_lcd_flush(lcd, pcc, xlen);
				
				l++;
			}

			j += linecnt;
        }
		wjq_free(pcc);
        break;

	case 16:

		break;
		
    case 24://65K真彩色		
		pcc = (u16 *)pdata;
		//图片取模:横向,左高右低
        for (j=0; j<ylen;) {
			if(j+linecnt >= ylen)
				linecnt = ylen-j;
			
			rlen = vfs_read(bmpfd, (void *)pdata, LineBytes*linecnt);
			if (rlen != LineBytes*linecnt) {
				wjq_log(LOG_DEBUG, "bmp read data err!\r\n");
			}
			
			l = 0;
			while (l < linecnt) {
				k = l*LineBytes;
				
            	for (i=0; i < xlen; i++) {
	            	/*3个字节1个像素，要进行对U16的转换
						rgb565
						#define BLUE         	 0x001F  
						#define GREEN         	 0x07E0
						#define RED           	 0xF800
					*/
					b = pdata[k++];
					g = pdata[k++];
					r = pdata[k++];
					
					r = ((r<<8)&0xf800);
					g = ((g<<3)&0x07e0);
					b = ((b>>3)&0x001f);
					*(pcc+i) = r+g+b;

	            }
				
				dev_lcd_flush(lcd, pcc, xlen);
				l++;
			}

			j += linecnt;
        }			
        break;

	case 32:
		break;
		
    default:
        break;
    } 

	dev_lcd_update(lcd);
	
	wjq_free(pdata);
	if (NumColors != 0) {
		wjq_free(palatte);
	}
	vfs_close(bmpfd);
    return 0;
}
#endif
/**
 *@brief:      dev_lcd_test
 *@details:    LCD测试函数
 *@param[in]   void  
 *@param[out]  无
 *@retval:     
 */
void dev_lcd_test(void)
{
	DevLcdNode *LcdCog = NULL;
	DevLcdNode *LcdOled = NULL;
	DevLcdNode *LcdOledI2C = NULL;
	DevLcdNode *LcdTft = NULL;

	/*  打开三个设备 */
	LcdCog = dev_lcd_open("spicoglcd");
	if (LcdCog==NULL)
		wjq_log(LOG_FUN, "open cog lcd err\r\n");

	LcdOled = dev_lcd_open("vspioledlcd");
	if (LcdOled==NULL)
		wjq_log(LOG_FUN, "open oled lcd err\r\n");
	
	LcdTft = dev_lcd_open("tftlcd");
	if (LcdTft==NULL)
		wjq_log(LOG_FUN, "open tft lcd err\r\n");

	LcdOledI2C = dev_lcd_open("i2coledlcd");
	if (LcdOledI2C==NULL)
		wjq_log(LOG_FUN, "open oled i2c lcd err\r\n");
	
	/*打开背光*/
	dev_lcd_backlight(LcdCog, 1);
	dev_lcd_backlight(LcdOled, 1);
	dev_lcd_backlight(LcdOledI2C, 1);
	dev_lcd_backlight(LcdTft, 1);

	#if 0/*不支持汉字时*/
	put_string(LcdCog, 5, 5, "spi cog lcd", BLACK);
	put_string(LcdOled, 5, 5, "vspi oled lcd", BLACK);
	put_string(LcdOledI2C, 5, 5, "i2c oled lcd", BLACK);
	put_string(LcdTft, 5, 5, "2.8 tft lcd", BLACK);
	#endif

	#if 1
	dev_lcd_put_string(LcdOled, "songti12", 10,1, "ABC-abc，", BLACK);
	dev_lcd_put_string(LcdOled, "siyuan16", 1, 13, "这是oled lcd", BLACK);
	dev_lcd_put_string(LcdOled, "songti12", 10,30, "www.wujique.com", BLACK);
	dev_lcd_put_string(LcdOled, "siyuan16", 1, 47, "屋脊雀工作室", BLACK);
	dev_lcd_update(LcdOled);
	dev_lcd_put_string(LcdCog, "songti12", 10,1, "ABC-abc，", BLACK);
	dev_lcd_put_string(LcdCog, "siyuan16", 1, 13, "这是cog lcd", BLACK);
	dev_lcd_put_string(LcdCog, "songti12", 10,30, "www.wujique.com", BLACK);
	dev_lcd_put_string(LcdCog, "siyuan16", 1, 47, "屋脊雀工作室", BLACK);
	dev_lcd_update(LcdCog);
	dev_lcd_put_string(LcdTft, "songti12", 20,30, "ABC-abc，", RED);
	dev_lcd_put_string(LcdTft, "siyuan16", 20,60, "这是tft lcd", RED);
	dev_lcd_put_string(LcdTft, "songti12", 20,100, "www.wujique.com", RED);
	dev_lcd_put_string(LcdTft, "siyuan16", 20,150, "屋脊雀工作室", RED);
	dev_lcd_update(LcdTft);
	dev_lcd_put_string(LcdOledI2C, "songti12", 10,1, "ABC-abc，", BLACK);
	dev_lcd_put_string(LcdOledI2C, "siyuan16", 1,13, "这是LcdOledI2C", BLACK);
	dev_lcd_put_string(LcdOledI2C, "songti12", 10,30, "www.wujique.com", BLACK);
	dev_lcd_put_string(LcdOledI2C, "siyuan16", 1,47, "屋脊雀工作室", BLACK);
	dev_lcd_update(LcdOledI2C);
	#endif
	
	while(1);
}



void dev_i2coledlcd_test(void)
{

	DevLcdNode *LcdOledI2C = NULL;

	wjq_log(LOG_FUN, "dev_i2coledlcd_test\r\n");

	LcdOledI2C = dev_lcd_open("i2coledlcd");
	if (LcdOledI2C==NULL)
		wjq_log(LOG_FUN, "open oled i2c lcd err\r\n");
	else
		wjq_log(LOG_FUN, "open oled i2c lcd suc\r\n");
	/*打开背光*/
	dev_lcd_backlight(LcdOledI2C, 1);

	dev_lcd_put_string(LcdOledI2C, "songti12", 10,1, "ABC-abc，", BLACK);
	dev_lcd_put_string(LcdOledI2C, "siyuan16", 1,13, "这是LcdOledI2C", BLACK);
	dev_lcd_put_string(LcdOledI2C, "songti12", 10,30, "www.wujique.com", BLACK);
	dev_lcd_put_string(LcdOledI2C, "siyuan16", 1,47, "屋脊雀工作室", BLACK);
	dev_lcd_update(LcdOledI2C);

	LcdOledI2C = dev_lcd_open("i2coledlcd2");
	if (LcdOledI2C==NULL)
		wjq_log(LOG_FUN, "open oled i2c2 lcd err\r\n");
	
	/*打开背光*/
	dev_lcd_backlight(LcdOledI2C, 1);
	while(1) {
		dev_lcd_put_string(LcdOledI2C, "songti12", 10,1, "ABC-abc，", BLACK);
		dev_lcd_put_string(LcdOledI2C, "siyuan16", 1,13, "这是LcdOledI2C", BLACK);
		dev_lcd_put_string(LcdOledI2C, "songti12", 10,30, "www.wujique.com", BLACK);
		dev_lcd_put_string(LcdOledI2C, "siyuan16", 1,47, "屋脊雀工作室", BLACK);
		dev_lcd_update(LcdOledI2C);
		Delay(1000);
		dev_lcd_color_fill(LcdOledI2C, 1, 1000, 1, 1000, WHITE);
		dev_lcd_update(LcdOledI2C);
		Delay(1000);
	}

}

