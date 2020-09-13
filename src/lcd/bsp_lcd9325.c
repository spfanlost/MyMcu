#include "stdlib.h"
#include "stm32f4xx_hal.h"
#include "bsp_lcd9325.h"
#include "bsp_ascii_1206.h"

void LCD_WR_REG(uint16_t regval) {
    regval = regval;            //ʹ��-O2�Ż���ʱ��,����������ʱ
    LCDYM->LCD_REG = regval;    //д��Ҫд�ļĴ������
}
void LCD_WR_DATA(uint16_t data) {
    data = data;                //ʹ��-O2�Ż���ʱ��,����������ʱ
    LCDYM->LCD_RAM = data;
}
uint16_t LCD_RD_DATA(void) {
    __IO uint16_t ram;          //��ֹ���Ż�
    ram = LCDYM->LCD_RAM;
    return ram;
}
void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue) {
    LCDYM->LCD_REG = LCD_Reg;       //д��Ҫд�ļĴ������
    LCDYM->LCD_RAM = LCD_RegValue;  //д������
}
uint16_t LCD_ReadReg(uint16_t LCD_Reg) {
    LCD_WR_REG(LCD_Reg);            //д��Ҫ���ļĴ������
    return LCD_RD_DATA();           //���ض�����ֵ
}
void LCD_WriteRAM(uint16_t RGB_Code) {
    LCDYM->LCD_RAM = RGB_Code;      //дʮ��λGRAM
}
////��ʼдGRAM
// LCDYM->LCD_REG=0x22;
//mdk -O1ʱ���Ż�ʱ��Ҫ����
void Lcd_Delay(uint16_t i) {
    while(--i);
}

//LCD������ʾ
/*****************************************************************************
** ��������:LCD_DisplayOn
** ��������: ����LCD��ʾ
** ��������: �ر�LCD��ʾ
*****************************************************************************/

void LCD_Display(uint8_t off_on) {
    if(off_on == 1)
        LCD_WriteReg(0x07, 0x0173);             //������ʾ
    else
        LCD_WriteReg(0x07, 0x0);//�ر���ʾ
}
//���ù��λ��
//Xpos:������
//Ypos:������
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos) {
#if Use_Horizontal==1
    LCD_WriteReg(0x20, Ypos);
    LCD_WriteReg(0x21, 319 - Xpos);
#else
    LCD_WriteReg(0x20, Xpos);
    LCD_WriteReg(0x21, Ypos);
#endif
}
//����LCD���Զ�ɨ�跽��
void LCD_Scan_Dir(void) {
    uint16_t regval = 0;
    regval |= L2R_D2U; //������,���ϵ���
    regval |= 1 << 12;
    LCD_WriteReg(0X03, regval);
}
//��ȡ��ĳ�����ɫֵ
//x,y:����
//����ֵ:�˵����ɫ
uint16_t LCD_ReadPoint(uint16_t x, uint16_t y) {
//  uint16_t r=0;
#if Use_Horizontal==1
    LCD_WriteReg(0x20, y);
    LCD_WriteReg(0x21, 319 - x);
#else
    LCD_WriteReg(0x20, x);
    LCD_WriteReg(0x21, y);
#endif
    LCDYM->LCD_REG = 0x22;                      //����IC���Ͷ�GRAMָ��
//  r=LCDYM->LCD_RAM;                       //dummy Read
//  r=r;
    //ʵ��������ɫ
    return LCDYM->LCD_RAM;
}

//����
//x,y:����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t Color) {
//  LCD_SetCursor(x,y);     //���ù��λ��
#if Use_Horizontal==1
    LCD_WriteReg(0x20, y);
    LCD_WriteReg(0x21, 319 - x);
#else
    LCD_WriteReg(0x20, x);
    LCD_WriteReg(0x21, y);
#endif
    LCDYM->LCD_REG = 0x22;  //��ʼд��GRAM
    LCDYM->LCD_RAM = Color;
}

void LCD_Clear(uint16_t color) {
    uint32_t index = 0;
    LCD_SetCursor(0x00, 0x0000);    //���ù��λ��
    LCDYM->LCD_REG = 0x22;          //��ʼд��GRAM
    for(index = 0; index < 76800; index++) {
        LCDYM->LCD_RAM = color;
    }
}

/****************************����һ��****************************/
//�������ܣ�������һ��
//��ڲ���: x1,y1 ֱ�ߵ����;x2,y2 ֱ�ߵ��յ�;color ֱ�ߵ���ɫ
//���ڲ���: ��
//˵����    �ú�������LCD�ϻ�һ��ֱ��
/****************************************************************/
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t Color) {
    int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
            yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
            curpixel = 0;

    deltax = abs(x2 - x1);        /* The difference between the x's */
    deltay = abs(y2 - y1);        /* The difference between the y's */
    x = x1;                       /* Start x off at the first pixel */
    y = y1;                       /* Start y off at the first pixel */
    if(x2 >= x1) {                /* The x-values are increasing */
        xinc1 = 1;
        xinc2 = 1;
    } else {                      /* The x-values are decreasing */
        xinc1 = -1;
        xinc2 = -1;
    }
    if(y2 >= y1) {                /* The y-values are increasing */
        yinc1 = 1;
        yinc2 = 1;
    } else {                      /* The y-values are decreasing */
        yinc1 = -1;
        yinc2 = -1;
    }
    if(deltax >= deltay) {        /* There is at least one x-value for every y-value */
        xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
        yinc2 = 0;                  /* Don't change the y for every iteration */
        den = deltax;
        num = deltax / 2;
        numadd = deltay;
        numpixels = deltax;         /* There are more x-values than y-values */
    } else {                      /* There is at least one y-value for every x-value */
        xinc2 = 0;                  /* Don't change the x for every iteration */
        yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
        den = deltay;
        num = deltay / 2;
        numadd = deltax;
        numpixels = deltay;         /* There are more y-values than x-values */
    }
    for(curpixel = 0; curpixel <= numpixels; curpixel++) {
        LCD_DrawPoint(x, y, Color); //����
        num += numadd;              /* Increase the numerator by the top of the fraction */
        if(num >= den) {            /* Check if numerator >= denominator */
            num -= den;               /* Calculate the new numerator value */
            x += xinc1;               /* Change the x as appropriate */
            y += yinc1;               /* Change the y as appropriate */
        }
        x += xinc2;                 /* Change the x as appropriate */
        y += yinc2;                 /* Change the y as appropriate */
    }
}

/*****************************************************************************
** ��������: LCD_Fill
** ��������: ��ָ��λ�û�һ��ָ����С�ľ������
*****************************************************************************/
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color) {
    uint32_t n;
#if Use_Horizontal==1
    //������ʾ
    LCD_WriteReg(0x50, ysta);
    LCD_WriteReg(0x51, yend);
    LCD_WriteReg(0x52, 319 - xend);
    LCD_WriteReg(0x53, 319 - xsta);

    LCD_WriteReg(0x20, ysta);
    LCD_WriteReg(0x21, 319 - xsta);
    //������ʾ
#else
    LCD_WriteReg(0x50, xsta); //ˮƽ����GRAM��ʼ��ַ
    LCD_WriteReg(0x51, xend); //ˮƽ����GRAM������ַ
    LCD_WriteReg(0x52, ysta); //��ֱ����GRAM��ʼ��ַ
    LCD_WriteReg(0x53, yend); //��ֱ����GRAM������ַ

    LCD_WriteReg(0x20, xsta);
    LCD_WriteReg(0x21, ysta);
#endif
    LCDYM->LCD_REG = 0x22; //��ʼд��GRAM
    n = (uint32_t)(yend - ysta + 1) * (xend - xsta + 1);
    while(n--) {
        LCDYM->LCD_RAM = color;   //��ʾ��������ɫ.
    }
}

/*****************************************************************************
** ��������: LCD_ShowChar
** ��������: ��ָ��λ����ʾһ���ַ�
**��ָ��λ����ʾһ���ַ�
**num:Ҫ��ʾ���ַ�:" "--->"~"
**size:�����С 12
*****************************************************************************/
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t chr, uint16_t Back_Color, uint16_t Color) {
    uint8_t temp;
    uint8_t j, t;
    uint16_t x0 = x;
    if(x > MAX_CHAR_POSX || y > MAX_CHAR_POSY)return;
    //���ô���
    chr = chr - ' '; //�õ�ƫ�ƺ��ֵ
    for(j = 0; j < 12; j++) {
        temp = asc2_1206[chr][j];
        for(t = 0; t < 6; t++) {
            if(temp & 0x01) {
                LCD_DrawPoint(x, y, Color);
            } else {
                LCD_DrawPoint(x, y, Back_Color);
            }
            temp >>= 1;
            x++;
        }
        x = x0;
        y++;
    }
}
void LCD_ShowString(uint16_t x, uint16_t y, const uint8_t *pstr, uint16_t Back_Color, uint16_t Color) {
    while(*pstr != '\0') {
        if(x > MAX_CHAR_POSX) {
            x = 0;
            y += 12;
        }
        if(y > MAX_CHAR_POSY) {
            y = x = 0;
            LCD_Clear(WHITE);
        }
        LCD_ShowChar(x, y, *pstr, Back_Color, Color);
        x += 6;
        pstr++;
    }
}

#define LCD_BL_Pin GPIO_PIN_15
#define LCD_BL_GPIO_Port GPIOB
#define LCD_LED HAL_GPIO_WritePin(LCD_BL_GPIO_Port,LCD_BL_Pin,GPIO_PIN_SET) //LCD����          PB0       
void BL_Init() {
    GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = LCD_BL_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LCD_BL_GPIO_Port, &GPIO_InitStruct);
}

void LCDx_Init(void) {
    BL_Init();
    Lcd_Delay(0xffff);
    LCD_WriteReg(0x00E5, 0x78F0);
    LCD_WriteReg(0x0001, 0x0100);
    LCD_WriteReg(0x0002, 0x0700);
    LCD_WriteReg(0x0003, 0x1030);
    LCD_WriteReg(0x0004, 0x0000);
    LCD_WriteReg(0x0008, 0x0202);
    LCD_WriteReg(0x0009, 0x0000);
    LCD_WriteReg(0x000A, 0x0000);
    LCD_WriteReg(0x000C, 0x0000);
    LCD_WriteReg(0x000D, 0x0000);
    LCD_WriteReg(0x000F, 0x0000);
    //power on sequence VGHVGL
    LCD_WriteReg(0x0010, 0x0000);
    LCD_WriteReg(0x0011, 0x0007);
    LCD_WriteReg(0x0012, 0x0000);
    LCD_WriteReg(0x0013, 0x0000);
    LCD_WriteReg(0x0007, 0x0000);
    //vgh
    LCD_WriteReg(0x0010, 0x1690);
    LCD_WriteReg(0x0011, 0x0227);
    HAL_Delay(100);
    //vregiout
    LCD_WriteReg(0x0012, 0x009D); //0x001b
    HAL_Delay(100);
    //vom amplitude
    LCD_WriteReg(0x0013, 0x1900);
    HAL_Delay(100);
    //vom H
    LCD_WriteReg(0x0029, 0x0025);
    LCD_WriteReg(0x002B, 0x000D);
    //gamma
    LCD_WriteReg(0x0030, 0x0007);
    LCD_WriteReg(0x0031, 0x0303);
    LCD_WriteReg(0x0032, 0x0003); // 0006
    LCD_WriteReg(0x0035, 0x0206);
    LCD_WriteReg(0x0036, 0x0008);
    LCD_WriteReg(0x0037, 0x0406);
    LCD_WriteReg(0x0038, 0x0304); //0200
    LCD_WriteReg(0x0039, 0x0007);
    LCD_WriteReg(0x003C, 0x0602); // 0504
    LCD_WriteReg(0x003D, 0x0008);
    //ram
    LCD_WriteReg(0x0050, 0x0000);
    LCD_WriteReg(0x0051, 0x00EF);
    LCD_WriteReg(0x0052, 0x0000);
    LCD_WriteReg(0x0053, 0x013F);
    LCD_WriteReg(0x0060, 0xA700);
    LCD_WriteReg(0x0061, 0x0001);
    LCD_WriteReg(0x006A, 0x0000);
    //
    LCD_WriteReg(0x0080, 0x0000);
    LCD_WriteReg(0x0081, 0x0000);
    LCD_WriteReg(0x0082, 0x0000);
    LCD_WriteReg(0x0083, 0x0000);
    LCD_WriteReg(0x0084, 0x0000);
    LCD_WriteReg(0x0085, 0x0000);
    //
    LCD_WriteReg(0x0090, 0x0010);
    LCD_WriteReg(0x0092, 0x0600);

    LCD_WriteReg(0x0007, 0x0133);
    LCD_WriteReg(0x00, 0x0022); //
    LCD_Scan_Dir();             //Ĭ��ɨ�跽��
    LCD_LED;                    //��������
}

