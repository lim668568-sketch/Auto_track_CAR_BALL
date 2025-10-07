#include "PID.h"
/*******************************************************************************/

// 用于初始化PID参数的函数
void PID_Init(PID *pid, float p, float i, float d, float maxI, float maxOut)
{
    pid->kp = p;  // 设置比例系数
    pid->ki = i;  // 设置积分系数
    pid->kd = d;  // 设置微分系数
    pid->maxIntegral = maxI;  // 设置积分限幅
    pid->maxOutput = maxOut;  // 设置输出限幅
    pid->error = 0;
    pid->lastError = 0;
    pid->integral = 0;
    pid->output = 0;
}

// 进行一次PID计算
// 参数为(pid结构体, 目标值, 反馈值)，计算结果放在pid结构体的output成员中
void PID_Calc(PID *pid, float reference, float feedback)
{
    // 更新数据
    pid->lastError = pid->error;  // 将旧error存起来
    pid->error = reference - feedback;  // 计算新error
 
    // 计算微分项
    float dout = (pid->error - pid->lastError) * pid->kd;
 
    // 计算比例项
    float pout = pid->error * pid->kp;
 
    // 计算积分项
    pid->integral += pid->error * pid->ki;
 
    // 积分限幅
    if (pid->integral > pid->maxIntegral) 
        pid->integral = pid->maxIntegral;
    else if (pid->integral < -pid->maxIntegral) 
        pid->integral = -pid->maxIntegral;
 
    // 计算输出
    pid->output = pout + dout + pid->integral;
 
    // 输出限幅
    if (pid->output > pid->maxOutput) 
        pid->output = pid->maxOutput;
    else if (pid->output < -pid->maxOutput) 
        pid->output = -pid->maxOutput;
}







