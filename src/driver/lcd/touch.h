#ifndef __TOUCH_H__
#define __TOUCH_H__

#include "common.h"
#include "mcu.h"
#include "mcu_io.h"

#define TP_PRES_DOWN 0x80  //����������
#define TP_CATH_PRES 0x40  //�а���������
#define CT_MAX_TOUCH  5    //������֧�ֵĵ���,�̶�Ϊ5��


//����״̬
#define Key_Down 0x80U
#define TP_Key_Down 0x40U
#define Key_Up   0x00U


/* Һ������ṹ�� */
typedef struct POINT
{
   uint16_t x;
   uint16_t y;
}Coordinate;

//�ʸ˽ṹ��
typedef struct
{
    u16 X0;//ԭʼ����
    u16 Y0;
    u16 X; //����/�ݴ�����
    u16 Y;
    u8  Key_Sta;//�ʵ�״̬
}Pen_Parameters;

//������У׼����
struct touch_param
{
    float xfac;
    float yfac;
    short xoff;
    short yoff;
};

extern struct touch_param Touch_date;
extern Pen_Parameters Pen_data;


//�봥����оƬ��������
#define TS_DRAW_RANGE 50
#define PEN mcu_io_pin_val(GPIOB,PIN1)   //PB1  INT
#define DOUT mcu_io_pin_val(GPIOB,PIN2)   //PC2  MISO
#define TDIN_Low mcu_io_reset(GPIOF,PIN11)  //PC3  MOSI
#define TDIN_High mcu_io_set(GPIOF,PIN11)  //PC3  MOSI
#define TCLK_Low mcu_io_reset(GPIOB,PIN0)  //PC0  SCLK
#define TCLK_High mcu_io_set(GPIOB,PIN0)  //PC0  SCLK
#define TCS_Low mcu_io_reset(GPIOC,PIN13) //PC13 CS
#define TCS_High mcu_io_set(GPIOC,PIN13) //PC13 CS

void TP_Write_Byte(u8 num);                     //�����оƬд��һ������
u16 TP_Read_AD(u8 CMD);                         //��ȡADת��ֵ
u16 TP_Read_XOY(u8 xy);                         //���˲��������ȡ(X/Y)
u8 TP_Read_XY(u16 *x,u16 *y);                   //˫�����ȡ(X+Y)
u8 TP_Read_XY2(u16 *x,u16 *y);                  //����ǿ�˲���˫���������ȡ
void TP_Drow_Touch_Point(u16 x,u16 y,u16 color);//��һ������У׼��
void TP_Save_Adjdata(void);                     //����У׼����
u8 TP_Get_Adjdata(void);                        //��ȡУ׼����
u8 Read_TP(u8 tp);                          //ɨ��
void touch_init(void);
void Draw_Big_Circle(u8 x,u16 y,u16 Color);
void touch_load_draw_ui(void);

#endif
















