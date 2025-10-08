#ifndef __DELAY_H
#define __DELAY_H
#include "main.h"
#include "stdio.h"
#include "string.h"
#include "stm32f4xx.h"
#include "tim.h"
#include "usart.h"
#include "motor.h"
#include "stm32f4xx_hal_uart.h"
#include <math.h>
#include "control.h"

void Delay_Init(uint16_t systclk);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);


#endif