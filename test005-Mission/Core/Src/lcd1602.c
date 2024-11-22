/*
 * lcd1602.c
 *
 *  Created on:  Nov 21, 2024
 *   Author   :  user
 *   Function :  1602 LCD device control
 */
#include "main.h"
#include "stm32f4xx_hal_i2c.h"


#define I2C_ADDR 0x4E	// 0x27 << 1


I2C_HandleTypeDef *hi2c = NULL;
//void lcd_command(char c)	// cmd_bit : abcd efgh
//{

//}

void i2c_data(char ch)
{
	char n1, n2, dd[4];
	n1 = ch & 0xf0;		// n1 :abcd 0000 upper nibble
	n2 = (ch & 0x0f) << 4; // n2 :efgp 0000 lower nibble to upper
	dd[0] = n1 | 0x0c;
	dd[1] = n1 | 0x08;
	dd[2] = n2 | 0x0c;
	dd[3] = n2 | 0x08;
	HAL_I2C_MasterTransmit(hi2c);
}

int i2c_init(I2C_HandleTypeDef *p)
{
	hi2c = p;
}

int i2c_scan()
{
	if(hi2c == NULL) return;
	for(int addr = 0; addr < 128 ; addr++)
	{
		if(HAL_I2C_IsDeviceReady(hi2c, addr, 1, 10) == HAL_OK)
		{
			printf("  %02x ", addr);
		}
		else
		{
			printf("  .  ");
		}
		if((addr % 16) == 0) printf("\r\n");
	}

}

void lcd_command(char cmd) // cmd_bit : abcd efgh
{
	char n1, n2, n3, n4, dd[4];
	n1 = cmd & 0xf0; 	       // n1 : abcd 0000, upper nibble up 4bits remain, low 4bits go away
	n2 = (cmd & 0x0f) << 4;    // n2 : efgh 0000, lower nibble, low 4bits remain, (cmd << 4) is also same
	n3 = (1<<3) | (1<<2) |0|0; // RW|EN_1|NC|RS; 0x0c
	n4 = (1<<3) |  0 	 |0|0; // RW|EN_0|NC|RS; 0x08
	dd[0] = n1 | n3;
	dd[1] = n1 | n4;
	dd[2] = n2 | n3;
	dd[3] = n2 | n4;
	HAL_I2C_Master_Transmit(hi2c, I2C_ADDR, dd, 4, 10);
}

void lcd_data(char ch)
{
	char n1, n2, n3, n4, dd[4];
	n1 = ch & 0xf0;
	n2 = (ch & 0x0f) << 4;
	n3 = (1<<3) | (1<<2) |0|(1<<0);
	n4 = (1<<3) |  0 	 |0|(1<<0);
	dd[0] = n1 | n3;
	dd[1] = n1 | n4;
	dd[2] = n2 | n3;
	dd[3] = n2 | n4;
	HAL_I2C_Master_Transmit(hi2c, I2C_ADDR, dd, 4, 10);
	//printf("%c\r\n",ch);

}

void lcd_init()
{
	lcd_command(0x01); // screen clear
	lcd_command(0x02); // Cursor home
	lcd_command(0x06);
	lcd_command(0x0f);
	HAL_Delay(50);
}


void lcd_test()
{

	lcd_data('H');
	lcd_data('e');
	lcd_data('l');
	lcd_data('l');
	lcd_data('o');
}

void lcd_print(char *str)
{
	while(*str) lcd_data(*str++);
}

void lcd_printEx(char *str, int ln)
{
	if(ln == 0) lcd_command(0x80);
	if(ln == 1) lcd_command(0xc0);
	lcd_print(str);
}
