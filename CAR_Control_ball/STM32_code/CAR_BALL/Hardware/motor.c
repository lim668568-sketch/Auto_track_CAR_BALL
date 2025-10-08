#include "motor.h"

#define Speed_MAX 99
#define Speed_MIN 10

void PWM_on(void)
{
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
}

void speed_control(int16_t speed1, int16_t speed2)
{
    // printf("sl:%d\rsr:%d\r\n", speed1, speed2);
    if (speed1 < 0) {
        HAL_GPIO_WritePin(Left1_GPIO_Port, Left1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Left2_GPIO_Port, Left2_Pin, GPIO_PIN_SET);
        speed1 = -speed1;
    } // 左轮反转
    //    else if (speed1 == 0) {
    //        HAL_GPIO_WritePin(Left1_GPIO_Port, Left1_Pin, GPIO_PIN_RESET);
    //        HAL_GPIO_WritePin(Left2_GPIO_Port, Left2_Pin, GPIO_PIN_RESET);
    //    } // 左轮停止
    else {
        HAL_GPIO_WritePin(Left1_GPIO_Port, Left1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(Left2_GPIO_Port, Left2_Pin, GPIO_PIN_RESET);
    } // 左轮正转
    if (speed2 < 0) {
        HAL_GPIO_WritePin(Right1_GPIO_Port, Right1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Right2_GPIO_Port, Right2_Pin, GPIO_PIN_SET);
        speed2 = -speed2;
    } // 右轮反转
    //    else if (speed2 == 0) {
    //        HAL_GPIO_WritePin(Right1_GPIO_Port, Right1_Pin, GPIO_PIN_RESET);
    //        HAL_GPIO_WritePin(Right2_GPIO_Port, Right2_Pin, GPIO_PIN_RESET);
    //    } // 右轮停止
    else {
        HAL_GPIO_WritePin(Right1_GPIO_Port, Right1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(Right2_GPIO_Port, Right2_Pin, GPIO_PIN_RESET);
    } // 右轮正转

    // 设置左轮的转速
    if (speed1 > Speed_MAX) { speed1 = Speed_MAX; } // 超过PWM幅值
    else if (speed1 < Speed_MIN && speed1 != 0) {
        speed1 = Speed_MIN;
    }
    // 设置右轮的转速
    if (speed2 > Speed_MAX) {
        speed2 = Speed_MAX;
    } else if (speed2 < Speed_MIN && speed2 != 0) {
        speed2 = Speed_MIN;
    }

    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, speed1);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, speed2);
    printf("Init.speed_left.val=%d\xff\xff\xff",speed1);
    printf("Init.speed_right.val=%d\xff\xff\xff",speed2);
}