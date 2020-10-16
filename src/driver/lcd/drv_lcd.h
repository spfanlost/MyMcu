

/**
 * @file    drv_lcd.h
 * @author  meng_yu
 * @brief   Graphic LCD function prototypes and defines function header file
 * @version 0.0.1
 * @date    2020-09-14
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#ifndef _DRV_LCD_H_
#define _DRV_LCD_H_

#include "fonts.h"

typedef struct
{
  int32_t ( *DrawBitmap      ) (uint32_t, uint32_t, uint32_t, uint8_t *);
  int32_t ( *FillRGBRect     ) (uint32_t, uint32_t, uint32_t, uint8_t*, uint32_t, uint32_t);
  int32_t ( *DrawHLine       ) (uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
  int32_t ( *DrawVLine       ) (uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
  int32_t ( *FillRect        ) (uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
  int32_t ( *GetPixel        ) (uint32_t, uint32_t, uint32_t, uint32_t*);
  int32_t ( *SetPixel        ) (uint32_t, uint32_t, uint32_t, uint32_t);
  int32_t ( *GetXSize        ) (uint32_t, uint32_t *);
  int32_t ( *GetYSize        ) (uint32_t, uint32_t *);
  int32_t ( *SetLayer        ) (uint32_t, uint32_t);
  int32_t ( *GetFormat       ) (uint32_t, uint32_t *);
} LCD_UTILS_Drv_t;

typedef struct
{
  /* Control functions */
  int32_t (*Init             )(void*, uint32_t, uint32_t);
  int32_t (*DeInit           )(void*);
  int32_t (*ReadID           )(void*, uint32_t*);
  int32_t (*DisplayOn        )(void*);
  int32_t (*DisplayOff       )(void*);
  int32_t (*SetBrightness    )(void*, uint32_t);
  int32_t (*GetBrightness    )(void*, uint32_t*);
  int32_t (*SetOrientation   )(void*, uint32_t);
  int32_t (*GetOrientation   )(void*, uint32_t*);

  /* Drawing functions*/
  int32_t ( *SetCursor       ) (void*, uint32_t, uint32_t);
  int32_t ( *DrawBitmap      ) (void*, uint32_t, uint32_t, uint8_t *);
  int32_t ( *FillRGBRect     ) (void*, uint32_t, uint32_t, uint8_t*, uint32_t, uint32_t);
  int32_t ( *DrawHLine       ) (void*, uint32_t, uint32_t, uint32_t, uint32_t);
  int32_t ( *DrawVLine       ) (void*, uint32_t, uint32_t, uint32_t, uint32_t);
  int32_t ( *FillRect        ) (void*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
  int32_t ( *GetPixel        ) (void*, uint32_t, uint32_t, uint32_t*);
  int32_t ( *SetPixel        ) (void*, uint32_t, uint32_t, uint32_t);
  int32_t ( *GetXSize        ) (void*, uint32_t *);
  int32_t ( *GetYSize        ) (void*, uint32_t *);
}LCD_Drv_t;

struct lcd_drv
{
    dword_t (*bl_init           )(void);
    dword_t (*bl_sts            )(byte_t);
    void    (*bus_init          )(void);
    void    (*bus_wr_cmd        )(byte_t);
    void    (*bus_wr_dat        )(word_t);
    word_t  (*bus_rd_dat        )(void);
    void    (*bus_wr_reg        )(byte_t, word_t);
    word_t  (*bus_rd_reg        )(byte_t);
    void    (*bus_wr_n_reg      )(byte_t, dword_t, word_t);
};

#ifndef GLCD_MIRROR_X
#define GLCD_MIRROR_X   1               /* Mirror X axis = 1:yes, 0:no */
#endif
#ifndef GLCD_MIRROR_Y
#define GLCD_MIRROR_Y   1               /* Mirror Y axis = 1:yes, 0:no */
#endif
#ifndef GLCD_SWAP_XY
#define GLCD_SWAP_XY    0               /* Swap X&Y axis = 1:yes, 0:no */
#endif

/*---------------------- Graphic LCD physical definitions --------------------*/
#define GLCD_SIZE_X     240             /* Screen size X (in pixels) */
#define GLCD_SIZE_Y     320             /* Screen size Y (in pixels) */
#define GLCD_BPP        16              /* Bits per pixel            */
#define BYPP ((GLCD_BPP + 7) / 8)       /* Bytes per pixel                    */

#if    (GLCD_SWAP_XY)
#define GLCD_WIDTH      GLCD_SIZE_Y     /* Screen Width  (in pixels) */
#define GLCD_HEIGHT     GLCD_SIZE_X     /* Screen Height (in pixels) */
#else
#define GLCD_WIDTH      GLCD_SIZE_X     /* Screen Width  (in pixels) */
#define GLCD_HEIGHT     GLCD_SIZE_Y     /* Screen Height (in pixels) */
#endif

/*---------------------- Graphic LCD color definitions -----------------------*/
/* Color coding (16-bit):
     15..11 = R4..0 (Red)
     10..5  = G5..0 (Green)
      4..0  = B4..0 (Blue)
*/

/* GLCD RGB color definitions                            */
#define GLCD_COLOR_BLACK        0x0000  /*   0,   0,   0 */
#define GLCD_COLOR_NAVY         0x000F  /*   0,   0, 128 */
#define GLCD_COLOR_DARK_GREEN   0x03E0  /*   0, 128,   0 */
#define GLCD_COLOR_DARK_CYAN    0x03EF  /*   0, 128, 128 */
#define GLCD_COLOR_MAROON       0x7800  /* 128,   0,   0 */
#define GLCD_COLOR_PURPLE       0x780F  /* 128,   0, 128 */
#define GLCD_COLOR_OLIVE        0x7BE0  /* 128, 128,   0 */
#define GLCD_COLOR_LIGHT_GREY   0xC618  /* 192, 192, 192 */
#define GLCD_COLOR_DARK_GREY    0x7BEF  /* 128, 128, 128 */
#define GLCD_COLOR_BLUE         0x001F  /*   0,   0, 255 */
#define GLCD_COLOR_GREEN        0x07E0  /*   0, 255,   0 */
#define GLCD_COLOR_CYAN         0x07FF  /*   0, 255, 255 */
#define GLCD_COLOR_RED          0xF800  /* 255,   0,   0 */
#define GLCD_COLOR_MAGENTA      0xF81F  /* 255,   0, 255 */
#define GLCD_COLOR_YELLOW       0xFFE0  /* 255, 255, 0   */
#define GLCD_COLOR_WHITE        0xFFFF  /* 255, 255, 255 */

/* LCD /CS is NE4 - Bank 4 of NOR/SRAM Bank 1~4                               */
//#define LCD_BASE (0x60000000UL | 0x0C000000UL)
//use NOR/SRAM Bank1.sector4,address bit HADDR[27,26]=11 A6 use data/cmd identify line
//STM32 HW will move right 1 bit 111 1110=0X7E
#define LCD_BASE ((uint32_t)(0x6C000000 | 0x0000007E))
#define LCD_REG16  (*((volatile uint16_t *)(LCD_BASE  ))) // LCD register address
#define LCD_DAT16  (*((volatile uint16_t *)(LCD_BASE+2))) // LCD data address

#define BG_COLOR 0  /* Background color                   */
#define TXT_COLOR 1 /* Text color                         */

/*-----------------------------------------------------------------------------------
  Exported variables
-----------------------------------------------------------------------------------*/
//extern const LCD_UTILS_Drv_t LCD_Driver;

/*-----------------------------------------------------------------------------------
  Exported functions
-----------------------------------------------------------------------------------*/
extern void GLCD_Init(void);
extern int32_t  GLCD_SetForegroundColor  (uint32_t color);
extern int32_t  GLCD_SetBackgroundColor  (uint32_t color);
extern int32_t  GLCD_ClearScreen         (void);
extern int32_t  GLCD_SetFont             (GLCD_FONT *font);
extern int32_t  GLCD_DrawPixel           (uint32_t x, uint32_t y);
extern int32_t  GLCD_DrawHLine           (uint32_t x, uint32_t y, uint32_t length);
extern int32_t  GLCD_DrawVLine           (uint32_t x, uint32_t y, uint32_t length);
extern int32_t  GLCD_DrawRectangle       (uint32_t x, uint32_t y, uint32_t width, uint32_t height);
extern int32_t  GLCD_DrawChar            (uint32_t x, uint32_t y, int32_t  ch);
extern int32_t  GLCD_DrawString          (uint32_t x, uint32_t y, const char *str);
extern int32_t  GLCD_DrawBargraph        (uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t val);
extern int32_t  GLCD_DrawBitmap          (uint32_t x, uint32_t y, uint32_t width, uint32_t height, const uint8_t *bitmap);
extern int32_t  GLCD_VScroll             (uint32_t dy);
extern int32_t  GLCD_FrameBufferAccess   (bool enable);

#endif /* _LCD_H_ */


