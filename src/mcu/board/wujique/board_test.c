/**
 * @file                wujique_stm407.c
 * @brief           �ݼ�ȸSTM32F407������Ӳ�����Գ���
 * @author          wujique
 * @date            2018��1��29�� ����һ
 * @version         ����
 * @par             ��Ȩ���� (C), 2013-2023
 * @par History:
 * 1.��    ��:        2018��1��29�� ����һ
 *   ��    ��:         wujique
 *   �޸�����:   �����ļ�
*/
#include "mcu.h"
#include "board_sysconf.h"

#include "log.h"
#include "font.h"
#include "emenu.h"
#include "tslib.h"

#include "drv_wm8978.h"
#include "drv_lcd.h"
#include "soundplay.h"
#include "drv_touchkey.h"
#include "drv_keypad.h"
#include "drv_key.h"

extern u16 PenColor;
extern u16 BackColor;

#define TEST_FONG "SYsongti_12"
//#define TEST_FONG "SYsongti_16"

DevLcdNode * WJQTestLcd;

s32 wjq_wait_key(u8 key)
{
	while(1) {
		u8 keyvalue;
		s32 res;
		
		res = dev_keypad_read(&keyvalue, 1);
		if(res == 1) {
			if(key == 0) break;
			else if(keyvalue == key) break;	
		}
	}	
	return 0;
}

s32 wjq_test_showstr(char *s)
{
	wjq_log(LOG_DEBUG, "test:%s", s);
	dev_lcd_color_fill(WJQTestLcd, 1, 1000, 1, 1000, BackColor);
	
	/*���о�����ʾ���˵�*/
	dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 32, s, PenColor);
	dev_lcd_update(WJQTestLcd);
	wjq_wait_key(0);
	
	return 0;
}	
/**
 *@brief:      test_tft_display
 *@details:    ����TFT LCD
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     
 */
s32 test_tft_display(void)
{
	DevLcdNode *lcd;
	u8 step = 0;
	u8 dis = 1;
	
	dev_lcd_color_fill(WJQTestLcd, 1, 1000, 1, 1000, WHITE);
	
	/*���о�����ʾ���˵�*/
	dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 32, (char *)__FUNCTION__, BLACK);
	dev_lcd_update(WJQTestLcd);
	
	lcd = dev_lcd_open("tftlcd");
	if (lcd == NULL)	{
		wjq_test_showstr("open lcd err!");	
	} else {
		while(1) {
			if (dis == 1) {
				dis = 0;
				switch(step)
				{
					case 0:
						dev_lcd_color_fill(lcd, 1, 1000, 1, 1000, YELLOW);
						dev_lcd_update(lcd);
						break;
					case 1:
						dev_lcd_color_fill(lcd, 1, 1000, 1, 1000, RED);
						dev_lcd_update(lcd);
						break;
					case 2:
						dev_lcd_color_fill(lcd, 1, 1000, 1, 1000, BLUE);
						dev_lcd_put_string(lcd, TEST_FONG, 1, 120, "abc�ݼ�ȸADC123������12345678901234�ݼ�ȸ������", RED);
						dev_lcd_update(lcd);

						break;
					default:
						break;
				}
				step++;
				if(step >= 3)
					step = 0;
			}
			u8 keyvalue;
			s32 res;
			
			res = dev_keypad_read(&keyvalue, 1);
			if (res == 1) {
				if(keyvalue == 16) {
					dis = 1;
				} else if(keyvalue == 12) {
					break;
				}
			}
		}
	
	}
		return 0;
}

s32 test_cogoled_lcd_display(char *name)
{
	DevLcdNode *lcd;
	u8 step = 0;
	u8 dis = 1;
	
	lcd = dev_lcd_open(name);
	if (lcd == NULL) {
		wjq_test_showstr("open cog lcd err!");	
	} else {
		while(1) {
			if(dis == 1) {
				dis = 0;
				switch(step)
				{
					case 0:
						dev_lcd_color_fill(lcd, 1, 1000, 1, 1000, BLACK);
						dev_lcd_update(lcd);
						break;
					case 1:
						dev_lcd_color_fill(lcd, 1, 1000, 1, 1000, WHITE);
						dev_lcd_update(lcd);
						break;
					case 2:
						dev_lcd_put_string(lcd, TEST_FONG, 1, 56, "abc�ݼ�ȸADC123������", BLACK);
						dev_lcd_update(lcd);
						break;
						
					default:
						break;
				}
				step++;
				if(step >= 3)
					step = 0;
			}
			u8 keyvalue;
			s32 res;
			
			res = dev_keypad_read(&keyvalue, 1);
			if (res == 1) {
				if(keyvalue == 16) {
					dis = 1;
				} else if(keyvalue == 12) {
					break;
				}
			}
		}

	}
	
	return 0;
}
s32 test_i2c_oled_display(void)
{
	dev_lcd_color_fill(WJQTestLcd, 1, 1000, 1, 1000, WHITE);
	/*���о�����ʾ���˵�*/
	dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 32, (char *)__FUNCTION__, BLACK);
	dev_lcd_update(WJQTestLcd);
	return 	test_cogoled_lcd_display("i2coledlcd");
}
s32 test_vspi_oled_display(void)
{
	dev_lcd_color_fill(WJQTestLcd, 1, 1000, 1, 1000, WHITE);
	/*���о�����ʾ���˵�*/
	dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 32, (char *)__FUNCTION__, BLACK);
	dev_lcd_update(WJQTestLcd);
	return 	test_cogoled_lcd_display("vspioledlcd");
}

s32 test_spi_cog_display(void)
{
	dev_lcd_color_fill(WJQTestLcd, 1, 1000, 1, 1000, WHITE);
	/*���о�����ʾ���˵�*/
	dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 32, (char *)__FUNCTION__, BLACK);
	dev_lcd_update(WJQTestLcd);
	return 	test_cogoled_lcd_display("spicoglcd");
}
/*
	240*240 ��ɫTFT lcd 0x7735������
*/
s32 test_lcd_spi_128128(void)
{
	DevLcdNode *lcd;
	u8 step = 0;
	u8 dis = 1;
	
	dev_lcd_color_fill(WJQTestLcd, 1, 1000, 1, 1000, WHITE);
	/*���о�����ʾ���˵�*/
	dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 32, (char *)__FUNCTION__, BLACK);
	dev_lcd_update(WJQTestLcd);
	
	lcd = dev_lcd_open("spitftlcd");
	if (lcd == NULL)	{
		wjq_test_showstr("open lcd err!");	
	} else	{
		dev_lcd_setdir(lcd, W_LCD, L2R_D2U);
		
		while(1) {
			if (dis == 1) {
				dis = 0;
				switch(step)
				{
					case 0:
						dev_lcd_color_fill(lcd, 1, 1000, 1, 1000, YELLOW);
						dev_lcd_update(lcd);
						break;
					case 1:
						dev_lcd_color_fill(lcd, 1, 1000, 1, 1000, RED);
						dev_lcd_update(lcd);
						break;
					case 2:
						dev_lcd_color_fill(lcd, 1, 1000, 1, 1000, BLUE);
						dev_lcd_put_string(lcd, TEST_FONG, 1, 120, "abc�ݼ�ȸADC123������12345678901234�ݼ�ȸ������", RED);
						dev_lcd_update(lcd);
						break;
					case 3:
						dev_lcd_show_bmp(lcd, 1, 1, 240, 240, "mtd0/1:/pic/pic128.bmp");
						break;
					default:
						break;
				}
				step++;
				if(step >= 4)
					step = 0;
			}
			u8 keyvalue;
			s32 res;
			
			res = dev_keypad_read(&keyvalue, 1);
			if (res == 1) {
				if (keyvalue == 16) {
					dis = 1;
				} else if(keyvalue == 12) {
					break;
				}
			}
		}
	
	}
	return 0;
}

s32 test_lcd_pic(void)
{
	DevLcdNode *lcd;
	u8 step = 0;
	u8 dis = 1;
	
	dev_lcd_color_fill(WJQTestLcd, 1, 1000, 1, 1000, WHITE);
	/*���о�����ʾ���˵�*/
	dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 32, (char *)__FUNCTION__, BLACK);
	dev_lcd_update(WJQTestLcd);
	lcd = dev_lcd_open("tftlcd");
	if (lcd == NULL)	{
		wjq_test_showstr("open lcd err!");	
		return -1;
	}
	
	dev_lcd_setdir(lcd, W_LCD, L2R_D2U);
	
	wjq_test_showstr((char *)__FUNCTION__);
	dev_lcd_show_bmp(lcd, 1, 1, 320, 240, "mtd0/1:/pic/Ů�˵�ɫ.bmp");
	wjq_wait_key(16);
	dev_lcd_show_bmp(lcd, 1, 1, 320, 240, "mtd0/1:/pic/Ů��16ɫ.bmp");//��ɫ��
	wjq_wait_key(16);
	dev_lcd_show_bmp(lcd, 1, 1, 320, 240, "mtd0/1:/pic/Ů��256ɫ.bmp");//��ɫ��
	wjq_wait_key(16);
	dev_lcd_show_bmp(lcd, 1, 1, 320, 240, "mtd0/1:/pic/Ů��24λ.bmp");//���ɫ
	wjq_wait_key(16);
	dev_lcd_show_bmp(lcd, 1, 1, 320, 240, "mtd0/1:/pic/logo.bmp");//���ɫ
	wjq_wait_key(16);
	/*
	dev_lcd_show_bmp(lcd, 1, 1, 128, 128, "mtd0/1:/pic/pic128.bmp");
	wjq_wait_key(16);
	dev_lcd_show_bmp(lcd, 1, 1, 128, 64, "mtd0/1:/pic/PIC12864.bmp");
	wjq_wait_key(16);
	dev_lcd_show_bmp(lcd, 1, 1, 240, 240, "mtd0/1:/pic/pic240240.bmp");
	wjq_wait_key(16);
	*/
	dev_lcd_setdir(lcd, W_LCD, L2R_U2D);
	
	return 0;
}

s32 test_lcd_font(void)
{
	wjq_test_showstr((char *)__FUNCTION__);
	return 0;
}


s32 test_sound_buzzer(void)
{
	dev_lcd_color_fill(WJQTestLcd, 1, 1000, 1, 1000, WHITE);
	/*���о�����ʾ���˵�*/
	dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 32, (char *)__FUNCTION__, BLACK);
	dev_lcd_update(WJQTestLcd);
	while(1) {
		u8 keyvalue;
		s32 res;
		
		res = dev_keypad_read(&keyvalue, 1);
		if (res == 1) {
			if(keyvalue == 16) {
				dev_buzzer_open();	
			} else if(keyvalue == (0x80+16)) {
				dev_buzzer_close();
			} else if(keyvalue == 12) {
				break;
			}
		}
			
	}
	return 0;
}

s32 test_sound_fm(void)
{
	dev_lcd_color_fill(WJQTestLcd, 1, 1000, 1, 1000, WHITE);
	dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 32, (char *)__FUNCTION__, BLACK);
	dev_lcd_update(WJQTestLcd);
	dev_wm8978_inout(WM8978_INPUT_DAC|WM8978_INPUT_AUX|WM8978_INPUT_ADC,
					WM8978_OUTPUT_PHONE|WM8978_OUTPUT_SPK);
	
	dev_tea5767_open();
	dev_tea5767_setfre(105700);
	wjq_wait_key(12);
	//dev_tea5767_close();
	return 0;
}

s32 test_sound_wm8978(void)
{
	dev_lcd_color_fill(WJQTestLcd, 1, 1000, 1, 1000, WHITE);
	dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 32, (char *)__FUNCTION__, BLACK);
	dev_lcd_update(WJQTestLcd);
	fun_sound_play("mtd1/2:/stereo_16bit_32k.wav", "wm8978");
	wjq_wait_key(16);
	fun_sound_stop();
	wjq_log(LOG_DEBUG,"wm8978 test out\r\n");
	return 0;
}

s32 test_sound_dac(void)
{
	dev_lcd_color_fill(WJQTestLcd, 1, 1000, 1, 1000, WHITE);
	dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 32, (char *)__FUNCTION__, BLACK);
	dev_lcd_update(WJQTestLcd);
	fun_sound_play("mtd0/1:/mono_16bit_8k.wav", "dacsound");
	wjq_wait_key(16);
	fun_sound_stop();
	wjq_log(LOG_DEBUG,"dac test out\r\n");
	return 0;
}
s32 test_sound_rec(void)
{
	dev_lcd_color_fill(WJQTestLcd, 1, 1000, 1, 1000, WHITE);
	dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 32, (char *)__FUNCTION__, BLACK);
	dev_lcd_update(WJQTestLcd);
	fun_sound_rec("mtd0/1:/rec8.wav");
	wjq_wait_key(16);
	fun_rec_stop();
	fun_sound_play("mtd0/1:/rec8.wav", "wm8978");	
	while(1) {
		if(SOUND_IDLE == fun_sound_get_sta())
			break;

		u8 keyvalue;
		s32 res;
		
		res = dev_keypad_read(&keyvalue, 1);
		if (res == 1) {
			if(keyvalue == 12) {
				break;
			}
		}
	}
	wjq_log(LOG_DEBUG, "test_sound_rec OUT!\r\n");
	return 0;
}

s32 test_tp_calibrate(void)
{
	DevLcdNode *lcd;

	dev_lcd_color_fill(WJQTestLcd, 1, 1000, 1, 1000, WHITE);
	/*���о�����ʾ���˵�*/
	dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 32, (char *)__FUNCTION__, BLACK);
	dev_lcd_update(WJQTestLcd);
	lcd = dev_lcd_open("tftlcd");
	if(lcd == NULL) {
		wjq_test_showstr("open lcd err!");	
	} else	{
		dev_lcd_setdir(lcd, H_LCD, L2R_U2D);
		dev_touchscreen_open();
		ts_calibrate(lcd);
		dev_touchscreen_close();
	}
	
	dev_lcd_color_fill(lcd, 1, 1000, 1, 1000, BLUE);
	dev_lcd_update(lcd);
	dev_lcd_close(lcd);
	
	return 0;
}


s32 test_tp_test(void)
{
	DevLcdNode *lcd;

	dev_lcd_color_fill(WJQTestLcd, 1, 1000, 1, 1000, WHITE);
	/*���о�����ʾ���˵�*/
	dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 32, (char *)__FUNCTION__, BLACK);
	dev_lcd_update(WJQTestLcd);
	lcd = dev_lcd_open("tftlcd");
	if (lcd == NULL)	{
		wjq_test_showstr("open lcd err!");	
	} else {
		dev_lcd_setdir(lcd, H_LCD, L2R_U2D);
		dev_touchscreen_open();	
	
		struct tsdev *ts;
		ts = ts_open_module();

		struct ts_sample samp[10];
		int ret;
		u8 i =0;	
		while(1) {
			ret = ts_read(ts, samp, 10);
			if (ret != 0) {
				//uart_printf("pre:%d, x:%d, y:%d\r\n", samp[0].pressure, samp[0].x, samp[0].y);
						
				i = 0;
				
				while(1) {
					if (i>= ret) break;
					
					if (samp[i].pressure != 0 ) {
						//uart_printf("pre:%d, x:%d, y:%d\r\n", samp.pressure, samp.x, samp.y);
						dev_lcd_drawpoint(lcd, samp[i].x, samp[i].y, RED); 
					}
					i++;
				}
			}

			u8 keyvalue;
			s32 res;
			
			res = dev_keypad_read(&keyvalue, 1);
			if (res == 1) {
				if (keyvalue == 8) {
					dev_lcd_color_fill(lcd, 1, 1000, 1, 1000, BLUE);
					dev_lcd_update(lcd);
				} else if(keyvalue == 12) {
					break;
				}
			}
		}

		dev_touchscreen_close();
	}
	return 0;
}


s32 test_key(void)
{
	u8 tmp;
	s32 res;
	u8 keyvalue;
		
	dev_lcd_color_fill(WJQTestLcd, 1, 1000, 1, 1000, WHITE);
	dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 32, (char *)__FUNCTION__, BLACK);
	dev_lcd_update(WJQTestLcd);
	dev_key_open();

	while(1) {
		res = dev_key_read(&tmp, 1);
		if(1 == res) {
			if (tmp == DEV_KEY_PRESS) {
				dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 10, "	   press	", BLACK);
				dev_lcd_update(WJQTestLcd);
			} else if(tmp == DEV_KEY_REL) {
				dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 10, "	 release	  ", BLACK);
				dev_lcd_update(WJQTestLcd);
			}
		}

		res = dev_keypad_read(&keyvalue, 1);
		if (res == 1) {
			if (keyvalue == 12) {
				break;
			}
		}
	}


	dev_key_close();
	return 0;
}


s32 test_camera(void)
{
	DevLcdNode *lcd;

	//dev_lcd_color_fill(emenulcd, 1, 1000, 1, 1000, WHITE);
	/*���о�����ʾ���˵�*/
	//dev_lcd_put_string(emenulcd, FONT_SONGTI_1212, 1, 32, __FUNCTION__, BLACK);
	//dev_lcd_update(emenulcd);
	lcd = dev_lcd_open("tftlcd");
	if(lcd == NULL)
	{
		wjq_test_showstr("open lcd err!");
		wjq_wait_key(16);
	}
	else
	{

		if(-1 == dev_camera_open())
		{
			wjq_log(LOG_DEBUG, "open camera err\r\n");
			return -1;
		}
		dev_camera_show(lcd);
		
		wjq_wait_key(16);
		dev_camera_close();
			
	}
	
	dev_lcd_color_fill(lcd, 1, 1000, 1, 1000, BLUE);
	dev_lcd_update(lcd);
	return 0;
}
s32 test_rs485_rec(void)
{
	u8 keyvalue;
	u8 buf[20];
	u8 len;
	s32 res;
	
	dev_lcd_color_fill(WJQTestLcd, 1, 1000, 1, 1000, WHITE);
	dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 32, (char *)__FUNCTION__, BLACK);
	dev_lcd_update(WJQTestLcd);
	dev_rs485_open();

	while(1)
	{
		res = dev_keypad_read(&keyvalue, 1);
		if(res == 1)
		{
			if(keyvalue == 12)
			{
				break;
			}
		}

		len = dev_rs485_read(buf, sizeof(buf));
		if(len > 0)
		{
			buf[len] = 0;
			wjq_log(LOG_DEBUG,"%s", buf);
			memset(buf, 0, sizeof(buf));
		}

	}

	return 0;
}

s32 test_rs485_snd(void)
{
	u8 keyvalue;
	s32 res;
	
	dev_lcd_color_fill(WJQTestLcd, 1, 1000, 1, 1000, WHITE);
	dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 32, (char *)__FUNCTION__, BLACK);
	dev_lcd_update(WJQTestLcd);
	dev_rs485_open();

	while(1)
	{
		res = dev_keypad_read(&keyvalue, 1);
		if(res == 1)
		{
			if(keyvalue == 12)
			{
				break;
			}
			else if(keyvalue == 16)
			{
				res = dev_rs485_write("rs485 test\r\n", 14);
				wjq_log(LOG_DEBUG, "dev rs485 write:%d\r\n", res);
			}
		}
	}
	return 0;
}

s32 test_can_rec(void)
{
	wjq_test_showstr((char *)__FUNCTION__);
	return 0;
}
s32 test_can_snd(void)
{
	wjq_test_showstr((char *)__FUNCTION__);
	return 0;
}
s32 test_uart(void)
{
	u8 keyvalue;
	u8 buf[12];
    s32 len;
    s32 res;
	
	dev_lcd_color_fill(WJQTestLcd, 1, 1000, 1, 1000, WHITE);
	dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 32, (char *)__FUNCTION__, BLACK);
	dev_lcd_update(WJQTestLcd);
	while(1)
	{
		
	    len =  mcu_uart_read (MCU_UART_3, buf, 10);
		if(len > 0)
		{
	    	res = mcu_uart_write(MCU_UART_3, buf, len);
		}

		res = dev_keypad_read(&keyvalue, 1);
		if(res == 1)
		{
			if(keyvalue == 12)
			{
				break;
			}
		}
	}
	return 0;
}
/*

	�򵥲��ԣ����ڷ���AT����յ�OK���ɡ�

*/
s32 test_esp8266(void)
{
	return 0;
}



s32 wjq_test(void)
{
	wjq_test_showstr((char *)__FUNCTION__);
	return 0;
}

s32 test_spiflash_board(void)
{
	dev_lcd_color_fill(WJQTestLcd, 1, 1000, 1, 1000, WHITE);
	dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 32, (char *)__FUNCTION__, BLACK);
	dev_lcd_update(WJQTestLcd);
	
	dev_spiflash_test_chipcheck("board_spiflash");
	wjq_wait_key(12);
	return 0;
}

s32 test_spiflash_core(void)
{
	dev_lcd_color_fill(WJQTestLcd, 1, 1000, 1, 1000, WHITE);
	dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 32, (char *)__FUNCTION__, BLACK);
	dev_lcd_update(WJQTestLcd);
	
	dev_spiflash_test_chiperase("core_spiflash");
	wjq_wait_key(12);
	return 0;
}

s32 test_touchkey(void)
{
	u8 tmp;
	s32 res;
	u8 keyvalue;
		
	dev_lcd_color_fill(WJQTestLcd, 1, 1000, 1, 1000, WHITE);
	dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 32, (char *)__FUNCTION__, BLACK);
	dev_lcd_update(WJQTestLcd);
	
	dev_touchkey_open();

	while(1)
	{
		res = dev_touchkey_read(&tmp, 1);
		if(1 == res)
		{
			if(tmp == DEV_TOUCHKEY_TOUCH)
			{
				wjq_log(LOG_FUN, "touch key test get a touch event!\r\n");
				dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 10, "      touch    ", BLACK);
				dev_lcd_update(WJQTestLcd);
			}
			else if(tmp == DEV_TOUCHKEY_RELEASE)
			{
				wjq_log(LOG_FUN, "touch key test get a release event!\r\n");
				dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 10, "      release    ", BLACK);
				dev_lcd_update(WJQTestLcd);
			}
		}

		res = dev_keypad_read(&keyvalue, 1);
		if(res == 1)
		{
			if(keyvalue == 12)
			{
				break;
			}
		}
	}


	dev_touchkey_close();
	return 0;
}


s32 test_keypad(void)
{
	u8 dis_flag = 1;
	u8 keyvalue;
	s32 res;
	s32 esc_flag = 1;
	
	char testkeypad1[]="1 2 3 F1 ";
	char testkeypad2[]="4 5 6 DEL";
	char testkeypad3[]="7 8 9 ESC";
	char testkeypad4[]="* 0 # ENT";
	
	dev_keypad_open();

	while(1)
	{
		if(dis_flag == 1)
		{
			dev_lcd_color_fill(WJQTestLcd, 1, 1000, 1, 1000, WHITE);
			dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 1, (char *)__FUNCTION__, BLACK);
			dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 13, testkeypad1, BLACK);
			dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 26, testkeypad2, BLACK);
			dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 39, testkeypad3, BLACK);
			dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 52, testkeypad4, BLACK);
			dev_lcd_update(WJQTestLcd);
			dis_flag = 0;
		}
		
		res = dev_keypad_read(&keyvalue, 1);
		if(res == 1 && ((keyvalue & KEYPAD_PR_MASK) == 0))
		{
			dis_flag = 1;
			switch(keyvalue)
			{
				case 1:
					testkeypad1[0] = ' ';
					break;
				case 2:
					testkeypad1[2] = ' ';
					break;
				case 3:
					testkeypad1[4] = ' ';
					break;
				case 4:
					testkeypad1[6] = ' ';
					testkeypad1[7] = ' ';
					testkeypad1[8] = ' ';
					break;
				case 5:
					testkeypad2[0] = ' ';
					break;
				case 6:
					testkeypad2[2] = ' ';
					break;
				case 7:
					testkeypad2[4] = ' ';
					break;
				case 8:
					testkeypad2[6] = ' ';
					testkeypad2[7] = ' ';
					testkeypad2[8] = ' ';
					break;
				case 9:
					testkeypad3[0] = ' ';
					break;
				case 10:
					testkeypad3[2] = ' ';
					break;
				case 11:
					testkeypad3[4] = ' ';
					break;
				case 12:
					if(esc_flag == 1)
					{
						esc_flag = 0;
						testkeypad3[6] = ' ';
						testkeypad3[7] = ' ';
						testkeypad3[8] = ' ';
					}
					else
					{
						return 0;
					}
					break;

				case 13:
					testkeypad4[0] = ' ';
					break;
				case 14:
					testkeypad4[2] = ' ';
					break;
				case 15:
					testkeypad4[4] = ' ';
					break;
				case 16:
					testkeypad4[6] = ' ';
					testkeypad4[7] = ' ';
					testkeypad4[8] = ' ';
					break;
			} 
		}
	}

}


const MENU WJQTestList[]=
{
	MENU_L_0,//�˵��ȼ�
	"���Գ���",//����
	"test",	//Ӣ��
	MENU_TYPE_KEY_2COL,//�˵�����
	NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

		MENU_L_1,//�˵��ȼ�
		"LCD",//����
		"LCD",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			/*
			MENU_L_2,//�˵��ȼ�
			"VSPI OLED",//����
			"VSPI OLED",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			test_vspi_oled_display,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			*/
			MENU_L_2,//�˵��ȼ�
			"I2C OLED",//����
			"I2C OLED",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			test_i2c_oled_display,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			/*
			MENU_L_2,//�˵��ȼ�
			"SPI COG",//����
			"SPI COG",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			test_spi_cog_display,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			*/
			MENU_L_2,//�˵��ȼ�
			"SPI tft",//����
			"SPI tft",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			//test_lcd_spi_128128,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			test_spi_cog_display,
			
			MENU_L_2,//�˵��ȼ�
			"tft",//����
			"tft",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			test_tft_display,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
	
			MENU_L_2,//�˵��ȼ�
			"ͼƬ����",//����
			"test BMP",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			test_lcd_pic,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			
			MENU_L_2,//�˵��ȼ�
			"�ֿ����",//����
			"test Font",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			test_lcd_font,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

		MENU_L_1,//�˵��ȼ�
		"����",//����
		"sound",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"������",//����
			"buzzer",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			test_sound_buzzer,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

			MENU_L_2,//�˵��ȼ�
			"DAC����",//����
			"DAC music",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			test_sound_dac,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

			MENU_L_2,//�˵��ȼ�
			"����",//����
			"FM",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			test_sound_fm,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

			MENU_L_2,//�˵��ȼ�
			"I2S����",//����
			"I2S Music",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			test_sound_wm8978,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

			MENU_L_2,//�˵��ȼ�
			"¼��",//����
			"rec",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			test_sound_rec,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

		MENU_L_1,//�˵��ȼ�
		"������",//����
		"tp",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			
			MENU_L_2,//�˵��ȼ�
			"У׼",//����
			"calibrate",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			test_tp_calibrate,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

			MENU_L_2,//�˵��ȼ�
			"����",//����
			"test",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			test_tp_test,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			
		MENU_L_1,//�˵��ȼ�
		"����",//����
		"KEY",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"���İ尴��",//����
			"core KEY",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			test_key,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			
			/*��������*/
			MENU_L_2,//�˵��ȼ�
			"��������",//����
			"touch key",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			test_touchkey,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

			/*���Բ��⣬�������Գ����Ҫʹ�þ��󰴼�*/
			MENU_L_2,//�˵��ȼ�
			"���󰴼�",//����
			"keypad",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			test_keypad,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
		
		MENU_L_1,//�˵��ȼ�
		"����",//����
		"camera",	//Ӣ��
		MENU_TYPE_FUN,//�˵�����
		test_camera,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
		
		MENU_L_1,//�˵��ȼ�
		"SPI FLASH",//����
		"SPI FLASH",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"���İ�FLASH",//����
			"core FLASH",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			test_spiflash_core,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			
			MENU_L_2,//�˵��ȼ�
			"�װ� FLASH",//����
			"board FLASH",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			test_spiflash_board,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			
		MENU_L_1,//�˵��ȼ�
		"ͨ��",//����
		"com",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

			MENU_L_2,//�˵��ȼ�
			"485",//����
			"485",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
				MENU_L_3,//�˵��ȼ�
				"485 ����",//����
				"485 rec",	//Ӣ��
				MENU_TYPE_FUN,//�˵�����
				test_rs485_rec,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
				MENU_L_3,//�˵��ȼ�
				"485 ����",//����
				"485 snd",	//Ӣ��
				MENU_TYPE_FUN,//�˵�����
				test_rs485_snd,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			
			MENU_L_2,//�˵��ȼ�
			"CAN",//����
			"CAN",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
				MENU_L_3,//�˵��ȼ�
				"CAN ����",//����
				"CAN rec",	//Ӣ��
				MENU_TYPE_FUN,//�˵�����
				test_can_rec,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
				
				MENU_L_3,//�˵��ȼ�
				"CAN ����",//����
				"CAN snd",	//Ӣ��
				MENU_TYPE_FUN,//�˵�����
				test_can_snd,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			
		
		
			MENU_L_2,//�˵��ȼ�
			"����",//����
			"uart",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			test_uart,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

			MENU_L_2,//�˵��ȼ�
			"����",//����
			"eth",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

			MENU_L_2,//�˵��ȼ�
			"OTG",//����
			"OTG",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
				MENU_L_3,//�˵��ȼ�
				"HOST",//����
				"HOST",	//Ӣ��
				MENU_TYPE_LIST,//�˵�����
				NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
				MENU_L_3,//�˵��ȼ�
				"Device",//����
				"Device",	//Ӣ��
				MENU_TYPE_LIST,//�˵�����
				NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
		MENU_L_1,//�˵��ȼ�
		"ģ��",//����
		"mod",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
		
			/*�����ӿڲ���*/
			MENU_L_2,//�˵��ȼ�
			"RF24L01",//����
			"RF24L01",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

			MENU_L_2,//�˵��ȼ�
			"MPU6050",//����
			"MPU6050",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

			/*�ô�������8266ģ��*/
			MENU_L_2,//�˵��ȼ�
			"wifi",//����
			"wifi",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			test_esp8266,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
		

		MENU_L_1,//�˵��ȼ�
		"test",//����
		"test",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
		
		MENU_L_1,//�˵��ȼ�
		"test1",//����
		"test1",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

		MENU_L_1,//�˵��ȼ�
		"test2",//����
		"test2",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

		MENU_L_1,//�˵��ȼ�
		"test3",//����
		"test3",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

		MENU_L_1,//�˵��ȼ�
		"test4",//����
		"test4",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

		MENU_L_1,//�˵��ȼ�
		"test5",//����
		"test5",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

		MENU_L_1,//�˵��ȼ�
		"test6",//����
		"test6",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

		MENU_L_1,//�˵��ȼ�
		"test7",//����
		"test7",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

		MENU_L_1,//�˵��ȼ�
		"test8",//����
		"test8",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

		MENU_L_1,//�˵��ȼ�
		"test9",//����
		"test9",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

		MENU_L_1,//�˵��ȼ�
		"test10",//����
		"test10",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
		
			MENU_L_2,//�˵��ȼ�
			"t10-1",//����
			"t10-1",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"t10-2",//����
			"t10-2",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"t10-3",//����
			"t10-3",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"t10-4",//����
			"t10-4",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"t10-5",//����
			"t10-5",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"t10-6",//����
			"t10-6",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"t10-7",//����
			"t10-7",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"t10-8",//����
			"t10-8",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"t10-9",//����
			"t10-9",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"t10-10",//����
			"t10-10",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"t10-11",//����
			"t10-11",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"t10-12",//����
			"t10-12",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
	
	/*���Ĳ˵��ǽ����˵���������*/			
	MENU_L_0,//�˵��ȼ�
	"END",//����
	"END",	//Ӣ��
	MENU_TYPE_NULL,//�˵�����
	NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
};

/*
	���Գ����൱��һ��APP�߳�
	
*/
void wujique_stm407_test(void)
{
	wjq_log(LOG_DEBUG,"\r\n\r\n\r\n---run board test\r\n\r\n\r\n\r\n");


	//WJQTestLcd = dev_lcd_open("tftlcd");
	WJQTestLcd = dev_lcd_open("i2coledlcd");
	if(WJQTestLcd == NULL)
	{
		wjq_log(LOG_DEBUG, "emenu open lcd err\r\n");
	}

	dev_key_open();
	dev_keypad_open();

	emenu_run(WJQTestLcd, (MENU *)&WJQTestList[0], sizeof(WJQTestList)/sizeof(MENU), TEST_FONG, 1, MENU_LANG_CHA);	
	while(1)
	{
	}
}


/*

��������

*/

s32 test_tft_lcd(void)
{
	DevLcdNode *lcd;
	u8 step = 0;
	u8 dis = 1;
	
	lcd = dev_lcd_open("tftlcd");
	if(lcd == NULL)
	{
		wjq_test_showstr("open tft lcd err!");	
	}
	else
	{
		while(1)
		{
			if(dis == 1)
			{
				dis = 0;
				switch(step)
				{
					case 0:
						dev_lcd_color_fill(lcd, 1, 1000, 1, 1000, YELLOW);
						dev_lcd_update(lcd);
						break;
					
					case 1:
						dev_lcd_color_fill(lcd, 1, 1000, 1, 1000, RED);
						dev_lcd_update(lcd);
						break;
					
					case 2:
						dev_lcd_color_fill(lcd, 1, 1000, 1, 1000, BLUE);
						dev_lcd_put_string(lcd, TEST_FONG, 20, 20, "abc�ݼ�ȸ������ADC", RED);
						dev_lcd_update(lcd);
						break;

					case 3:
						dev_lcd_show_bmp(lcd, 1, 1, 320, 240, "1:/pic/Ů�˵�ɫ.bmp");
						break;
					
					case 4:
						dev_lcd_show_bmp(lcd, 1, 1, 320, 240, "1:/pic/Ů��24λ.bmp");//���ɫ
						break;
					case 5:
						dev_lcd_backlight(lcd, 0);
						break;
					case 6:
						dev_lcd_backlight(lcd, 1);
						break;		
					default:
						break;
				}
				step++;
				if(step >= 7)
					step = 0;
			}
			u8 keyvalue;
			s32 res;
			
			res = dev_keypad_read(&keyvalue, 1);
			if(res == 1)
			{
				if(keyvalue == 16)
				{
					dis = 1;
				}
				else if(keyvalue == 12)
				{
					break;
				}
			}
		}

	}
	dev_lcd_close(lcd);
	return 0;
}

s32 test_cog_lcd(void)
{
	DevLcdNode *lcd;
	u8 step = 0;
	u8 dis = 1;
	
	lcd = dev_lcd_open("spicoglcd");
	if(lcd == NULL)
	{
		wjq_test_showstr("open cog lcd err!");	
		while(1);
	}

	while(1)
	{
		dev_lcd_color_fill(lcd, 1, 1000, 1, 1000, BLACK);
		dev_lcd_update(lcd);
		wjq_wait_key(16);
		dev_lcd_color_fill(lcd, 1, 1000, 1, 1000, WHITE);
		dev_lcd_update(lcd);
		wjq_wait_key(16);
		dev_lcd_put_string(lcd, TEST_FONG, 1, 32, "cog LCD���Գ���", BLACK);
		dev_lcd_update(lcd);
		wjq_wait_key(16);
		dev_lcd_backlight(lcd, 0);
		wjq_wait_key(16);
		dev_lcd_backlight(lcd, 1);
		wjq_wait_key(16);
	}
	
	return 0;
}

s32 test_tft_tp(void)
{
	DevLcdNode *lcd;


	lcd = dev_lcd_open("tftlcd");
	if(lcd == NULL)
	{
		wjq_test_showstr("open lcd err!");	
	}
	else
	{
		dev_lcd_backlight(lcd, 1);
		
		dev_lcd_color_fill(lcd, 1, 1000, 1, 1000, BLUE);
		dev_lcd_update(lcd);
		dev_lcd_setdir(lcd, H_LCD, L2R_U2D);
		dev_touchscreen_open();
		ts_calibrate(lcd);
		dev_touchscreen_close();
	}
	
	dev_lcd_color_fill(lcd, 1, 1000, 1, 1000, BLUE);
	dev_lcd_update(lcd);
	{
		dev_touchscreen_open();	
	
		struct tsdev *ts;
		ts = ts_open_module();

		struct ts_sample samp[10];
		int ret;
		u8 i =0;	
		while(1)
		{
			ret = ts_read(ts, samp, 10);
			if(ret != 0)
			{
				//uart_printf("pre:%d, x:%d, y:%d\r\n", samp[0].pressure, samp[0].x, samp[0].y);
						
				i = 0;
				
				while(1)
				{
					if(i>= ret)
						break;
					
					if(samp[i].pressure != 0 )
					{
						//uart_printf("pre:%d, x:%d, y:%d\r\n", samp.pressure, samp.x, samp.y);
						dev_lcd_drawpoint(lcd, samp[i].x, samp[i].y, 0xF800); 
					}
					i++;
				}
			}

		}

		dev_touchscreen_close();
	}
	return 0;
}


