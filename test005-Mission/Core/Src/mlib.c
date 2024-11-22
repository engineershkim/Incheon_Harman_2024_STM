/*
 * mlib.c
 *
 *  Created on: Nov 15, 2024
 *      Author: user
 */

#include <stdio.h>
#include "main.h"
#include "stm32f4xx_hal_i2c.h"
extern UART_HandleTypeDef huart2;

//I2C_HandleTypeDef *hi2c = NULL;
//void hi2c = NULL;


int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart2,&ch, 1, 10);
	return ch;
}
void StandBy()
{
	//printf()
	while(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin));
}
void ProgramStart(char * str)
{
	//printf("\033[2J\033[0;0H");
	cls();
	Cursor(0,0);
	printf("Program Start - %s", str);
	printf("Press Blue_button(B1) to Start...\r\n");
	StandBy();
}
void cls()
{
	printf("\033[2J");
}
void Cursor(int x, int y)
{
	char buf[20];
	sprintf(buf, "\033[%d;%dH", x, y);
	//printf("%s", buf);
	puts(buf); //string output function
}


