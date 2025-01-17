

/**
 * @file    retarget.c
 * @author  meng_yu
 * @brief   'Retarget' layer for target-dependent low level functions realize
 * @version 0.0.1
 * @date    2020-09-09
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
// #include <rt_misc.h>
#include "common.h"
#include "mcu.h"

/*-----------------------------------------------------------------------------------
  Private declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Extern variables declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Global variables definition
-----------------------------------------------------------------------------------*/
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if defined (__CC_ARM)
//'Retarget' layer for target-dependent low level functions
#pragma import(__use_no_semihosting)
struct __FILE {
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};
#endif

/* FILE is typedef’ d in stdio.h. */
FILE __stdout;
FILE __stdin;

/*-----------------------------------------------------------------------------------
  Local functions declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Local functions definition
-----------------------------------------------------------------------------------*/
int fputc(int c, FILE *f)
{
  while (!LL_USART_IsActiveFlag_TC(USART1))
    ;
  LL_USART_TransmitData8(USART1, c);
  return c;
}

int fgetc(FILE *f)
{
  return LL_USART_ReceiveData8(USART1);
}

#if defined (__CC_ARM)
int ferror(FILE *f)
{
  return EOF;
}
#endif

void _ttywrch(int c)
{
  LL_USART_TransmitData8(USART1, c);
}

//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int return_code)
{

label:

  goto label; /* endless loop */
}
