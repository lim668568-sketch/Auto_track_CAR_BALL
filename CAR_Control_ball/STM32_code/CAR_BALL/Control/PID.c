#include "PID.h"
/*******************************************************************************/

// ���ڳ�ʼ��PID�����ĺ���
void PID_Init(PID *pid, float p, float i, float d, float maxI, float maxOut)
{
    pid->kp = p;  // ���ñ���ϵ��
    pid->ki = i;  // ���û���ϵ��
    pid->kd = d;  // ����΢��ϵ��
    pid->maxIntegral = maxI;  // ���û����޷�
    pid->maxOutput = maxOut;  // ��������޷�
    pid->error = 0;
    pid->lastError = 0;
    pid->integral = 0;
    pid->output = 0;
}

// ����һ��PID����
// ����Ϊ(pid�ṹ��, Ŀ��ֵ, ����ֵ)������������pid�ṹ���output��Ա��
void PID_Calc(PID *pid, float reference, float feedback)
{
    // ��������
    pid->lastError = pid->error;  // ����error������
    pid->error = reference - feedback;  // ������error
 
    // ����΢����
    float dout = (pid->error - pid->lastError) * pid->kd;
 
    // ���������
    float pout = pid->error * pid->kp;
 
    // ���������
    pid->integral += pid->error * pid->ki;
 
    // �����޷�
    if (pid->integral > pid->maxIntegral) 
        pid->integral = pid->maxIntegral;
    else if (pid->integral < -pid->maxIntegral) 
        pid->integral = -pid->maxIntegral;
 
    // �������
    pid->output = pout + dout + pid->integral;
 
    // ����޷�
    if (pid->output > pid->maxOutput) 
        pid->output = pid->maxOutput;
    else if (pid->output < -pid->maxOutput) 
        pid->output = -pid->maxOutput;
}







