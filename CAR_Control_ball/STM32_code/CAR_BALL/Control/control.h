#ifndef __CONTROL_H
#define __CONTROL_H
#include "main.h"
#include "stdio.h"
#include "string.h"
#include "stm32f4xx.h"
#include "tim.h"
#include "usart.h"
#include "motor.h"
#include "stm32f4xx_hal_uart.h"
#include <math.h>
#include "Camera.h"
#include "Key.h"
#include "PID.h"

void ALL_Init(void);
void output_data_update(void);
void mode1_stop(void);//对应按键1的指令
//void mode2_number(void);
void mode3_car_run(void);
void mode_handle(void);
#endif
