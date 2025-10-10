#include "control.h"
PID pidx={0};//初始化结构体变量
int states=0;//小车运动的状态
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
			PID_Calc(&pidx,0,x_output);//计算PID偏差
		__HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);
		}
	if(htim==&htim7)
	{
		//更新电机转速
		if(h_output<0&&x_output<0)//此时车的行为应该：向左前
		{
			states=1;
			speed_control(20-pidx.output,20+pidx.output);
 			printf("Init.car_state.txt=\"向左前行驶\"\xff\xff\xff");
		
		}
		else if(h_output<0&&x_output>0)//此时车的行为应该：向右前
		{
			states=2;
			speed_control(20+pidx.output,20-pidx.output);
			printf("Init.car_state.txt=\"向右前行驶\"\xff\xff\xff");
			
		}
		else if(h_output>0&&x_output<0)//此时车的行为应该：向左后
		{
			states=3;
			speed_control(-20+pidx.output,-20-pidx.output);
			printf("Init.car_state.txt=\"向左后行驶\"\xff\xff\xff");
		}
		else if(h_output>0&&x_output>0)//此时车的行为应该：向右后
		{
			states=4;
			speed_control(-20-pidx.output,-20+pidx.output);
			printf("Init.car_state.txt=\"向右后行驶\"\xff\xff\xff");
		}
		else //小车原地转圈寻找小球
		{
			states=0;
			speed_control(10,-10);//没找到球时原地转圈
			printf("Init.speed_left.val=10\xff\xff\xff");
			printf("Init.speed_right.val=10\xff\xff\xff");
			printf("Init.car_state.txt=\"原地转圈\"\xff\xff\xff");
		}
		__HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);
	
}
}
//主函数文件的初始化函数
void ALL_Init(void)
{
	PID_Init(&pidx, 0.005, 0.01, 0.005, 1, 5);//初始化PID参数
	PWM_on();
	HAL_UART_Receive_IT(&huart3, &received_data, 1);//开启串口接收摄像头数据
	__HAL_TIM_CLEAR_IT(&htim6, TIM_IT_UPDATE);
	HAL_TIM_Base_Start_IT(&htim6);//定时器6中断，用于更新x.y轴偏差输出值
	__HAL_TIM_CLEAR_IT(&htim7, TIM_IT_UPDATE);
	HAL_TIM_Base_Start_IT(&htim7);//定时器7中断，用于更新点电机转速以及方向
}
void output_data_update(void)//小车运动状态下的数据更新
{
	//printf("X=%d   H=%d\n", x_output,h_output);
	printf("Init.x_output.val=%d\xff\xff\xff",x_output);
	printf("Init.h_output.val=%d\xff\xff\xff",h_output);
}
// void car_control(void)
// {
// 	if(states==1)
// 	{
// 		speed_control(20-pidx.output,20+pidx.output);
// 			printf("Init.car_state.txt=\"向左前行驶\"\xff\xff\xff");
// 			//delay_ms(500);
// 	}
// 	else if(states==2)
// 	{
// 		speed_control(20+pidx.output,20-pidx.output);
// 			printf("Init.car_state.txt=\"向右前行驶\"\xff\xff\xff");
// 			//delay_ms(500);
// 	}
// 	else if(states==3)
// 	{
// 		speed_control(-20+pidx.output,-20-pidx.output);
// 			printf("Init.car_state.txt=\"向左后行驶\"\xff\xff\xff");
// 			//delay_ms(500);
// 	}
// 	else if(states==4)
// 	{
// 		speed_control(-20-pidx.output,-20+pidx.output);
// 			printf("Init.car_state.txt=\"向右后行驶\"\xff\xff\xff");
// 			//delay_ms(500);
// 	}
// 	else 
// 	{
// 		speed_control(15,-15);//没找到球时原地转圈
// 		printf("Init.car_state.txt=\"原地转圈\"\xff\xff\xff");
// 		//delay_ms(500);
// 		speed_control(0,0);
// 		//delay_ms(1000);
// 	}
// }
// //执行按键检测
// void mode_handle(void)
// {
// 	if(get_mode==1)
// 	{
// 		mode1_stop();
// 		//get_mode=0;
// 	}
// //	else if(get_mode==2)
// //	{
// //		mode2_number();
// //		//get_mode=0;
// //	}
// 	else if(get_mode==3)
// 	{
// 		mode3_car_run();
// 		//get_mode=0;
// 	}
// }

// void mode1_stop(void)//对应按键1的指令
// {
// 	//Transmit_to_Camera(0x01);
// 	speed_control(0,0);//小车停止
// }
//void mode2_number(void)
//{
//	Transmit_to_Camera(0x02);
//	printf("Mode2_number start!\n");
//	target_length=length_data;//将串口数据存到目标距离这个变量里
//	printf("Target_Length=%d\n", target_length);
//}
// void mode3_car_run(void)
// {
// 	//Transmit_to_Camera(0x03);
// 	printf("Now car run!\n");
// 	printf("Target_Length=%d\n", target_length);
// 	speed_control(40,40);//先前进
// }
	