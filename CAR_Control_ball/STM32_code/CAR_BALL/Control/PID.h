#ifndef __PID_H
#define __PID_H
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



// 定义PID结构体用于存放一个PID的数据
typedef struct
{
    float kp, ki, kd;        // 三个系数：比例、积分和微分
    float error, lastError;  // 当前误差、上次误差
    float integral, maxIntegral;  // 积分、积分限幅
    float output, maxOutput; // 输出、输出限幅
} PID;

void PID_Init(PID *pid, float p, float i, float d, float maxI, float maxOut);
void PID_Calc(PID *pid, float reference, float feedback);



#endif
