#include "control.h"

// 重定向printf,串口1接上位机显示摄像头输出的数据
int fputc(int ch, FILE *f)
{
    uint8_t temp[1] = {ch};
    HAL_UART_Transmit(&huart1, temp, 1, 0xFFFF);
    return ch;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim6) 
		{
			output_data_update();//接串口1，调试时上位机查看更新的数据，后接串口屏显示偏差数据单位为厘米
		__HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);
		}
	if(htim==&htim7)
	{
		//更新电机转速
		if(h_output<0&&x_output<0)//此时车的行为应该：向左前
		{
			speed_control(35,40);
		}
		else if(h_output<0&&x_output>0)//此时车的行为应该：向右前
		{
			speed_control(40,35);
		}
		else if(h_output>0&&x_output<0)//此时车的行为应该：向左后
		{
			speed_control(-35,-40);
		}
		else if(h_output>0&&x_output>0)//此时车的行为应该：向右后
		{
			speed_control(-40,-35);
		}
		else speed_control(20,20);
		__HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);
	}
}
//主函数文件的初始化函数
void ALL_Init(void)
{
	PID pidx={0};

	PID_Init(&pidx, 10, 1, 5, 800, 1000);//初始化PID参数
	PWM_on();
	HAL_UART_Receive_IT(&huart3, &received_data, 1);//开启串口接收摄像头数据
	__HAL_TIM_CLEAR_IT(&htim6, TIM_IT_UPDATE);
	HAL_TIM_Base_Start_IT(&htim6);//定时器6中断，用于更新x.y轴偏差输出值
	__HAL_TIM_CLEAR_IT(&htim7, TIM_IT_UPDATE);
	HAL_TIM_Base_Start_IT(&htim7);//定时器7中断，用于更新点电机转速以及方向
}

//执行按键检测
void mode_handle(void)
{
	if(get_mode==1)
	{
		mode1_stop();
		//get_mode=0;
	}
//	else if(get_mode==2)
//	{
//		mode2_number();
//		//get_mode=0;
//	}
	else if(get_mode==3)
	{
		mode3_car_run();
		//get_mode=0;
	}
	
}
void output_data_update(void)//小车运动状态下的数据更新
{
	printf("X=%d   H=%d\n", x_output,h_output);
}
void mode1_stop(void)//对应按键1的指令
{
	//Transmit_to_Camera(0x01);
	speed_control(0,0);//小车停止
}
//void mode2_number(void)
//{
//	Transmit_to_Camera(0x02);
//	printf("Mode2_number start!\n");
//	target_length=length_data;//将串口数据存到目标距离这个变量里
//	printf("Target_Length=%d\n", target_length);
//}
void mode3_car_run(void)
{
	//Transmit_to_Camera(0x03);
	printf("Now car run!\n");
	printf("Target_Length=%d\n", target_length);
	speed_control(40,40);//先前进
}
	