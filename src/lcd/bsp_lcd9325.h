#ifndef __BSP_LCD9325_H
#define __BSP_LCD9325_H

#include "stm32f4xx.h"
#include "stdlib.h"
      
#define Use_Horizontal  0//�����Ƿ�ʹ�ú���         0,��ʹ��.1,ʹ��.
#if Use_Horizontal==1
    //uCGUI
    #ifndef LCD_XSIZE
    #define LCD_XSIZE (320) 
    #endif
    #ifndef LCD_YSIZE
    #define LCD_YSIZE (240) 
    #endif
    //emWin
    #ifndef XSIZE_PHYS
    #define XSIZE_PHYS (320)    
    #endif
    #ifndef YSIZE_PHYS
    #define YSIZE_PHYS (240)    
    #endif

    #define MAX_CHAR_POSX 312
    #define MAX_CHAR_POSY 232 

    #define L2R_U2D  (0<<5)|(1<<4)|(1<<3);  //������,���ϵ���
    #define L2R_D2U  (0<<5)|(0<<4)|(1<<3); //������,���µ���
    #define R2L_U2D  (0<<5)|(0<<4)|(0<<3);//���ҵ���,���ϵ���
    #define R2L_D2U  (1<<5)|(0<<4)|(1<<3); //���ҵ���,���µ���
    #define U2D_L2R  (0<<5)|(1<<4)|(0<<3); //���ϵ���,������
    #define U2D_R2L  (1<<5)|(1<<4)|(0<<3);//���ϵ���,���ҵ���
    #define D2U_L2R  (0<<5)|(0<<4)|(0<<3); //���µ���,������
    #define D2U_R2L  (1<<5)|(0<<4)|(0<<3); //���µ���,���ҵ���
#else     
    //uCGUI
    #ifndef LCD_XSIZE
    #define LCD_XSIZE (240) 
    #endif
    #ifndef LCD_YSIZE
    #define LCD_YSIZE (320) 
    #endif
    //emWin
    #ifndef XSIZE_PHYS
    #define XSIZE_PHYS (240)    
    #endif
    #ifndef YSIZE_PHYS
    #define YSIZE_PHYS (320)    
    #endif
    
    #define MAX_CHAR_POSX 232
    #define MAX_CHAR_POSY 312
    
    #define L2R_U2D  (1<<5)|(1<<4)|(0<<3);  //������,���ϵ���
    #define L2R_D2U  (0<<5)|(1<<4)|(0<<3); //������,���µ���
    #define R2L_U2D  (1<<5)|(0<<4)|(0<<3);//���ҵ���,���ϵ���
    #define R2L_D2U  (0<<5)|(0<<4)|(0<<3); //���ҵ���,���µ���
    #define U2D_L2R  (0<<5)|(0<<4)|(0<<3); //���ϵ���,������
    #define U2D_R2L  (1<<5)|(0<<4)|(1<<3);//���ϵ���,���ҵ���
    #define D2U_L2R  (0<<5)|(1<<4)|(1<<3); //���µ���,������
    #define D2U_R2L  (0<<5)|(0<<4)|(1<<3); //���µ���,���ҵ���
#endif

//LCD��ַ�ṹ��
typedef struct
{
    __IO uint16_t LCD_REG;
    __IO uint16_t LCD_RAM;
} LCD_TypeDef;


//ʹ��NOR/SRAM�� Bank1.sector4,��ַλHADDR[27,26]=11 A6��Ϊ�������������� 
//ע������ʱSTM32�ڲ�������һλ����! 111 1110=0X7E                
#define LCD_BASE        ((uint32_t)(0x6C000000 | 0x0000007E))
#define LCDYM             ((LCD_TypeDef *) LCD_BASE)

#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40 //��ɫ
#define BRRED 0XFC07 //�غ�ɫ
#define GRAY 0X8430 //��ɫ
#define DARKBLUE 0X01CF //����ɫ
#define LIGHTBLUE 0X7D7C    //ǳ��ɫ  
#define GRAYBLUE 0X5458 //����ɫ
#define LIGHTGREEN 0X841F //ǳ��ɫ
#define LGRAY 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE 0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE 0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

void LCD_WR_REG(uint16_t regval);
void LCD_WR_DATA(uint16_t data);
uint16_t LCD_RD_DATA(void);
void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);
uint16_t LCD_ReadReg(uint16_t LCD_Reg);
void LCD_WriteRAM(uint16_t RGB_Code);
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t Color);
uint16_t LCD_ReadPoint(uint16_t x,uint16_t y);
void LCD_Display(uint8_t off_on);
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_Scan_Dir(void);
void LCDx_Init(void);
void LCD_Clear(uint16_t color);

void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t Color);
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);

void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t chr,uint16_t Back_Color,uint16_t Color);
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *pstr,uint16_t Back_Color,uint16_t Color);

#endif


