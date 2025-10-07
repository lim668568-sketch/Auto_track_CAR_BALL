#include "control.h"

// �ض���printf,����1����λ����ʾ����ͷ���������
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
			output_data_update();//�Ӵ���1������ʱ��λ���鿴���µ����ݣ���Ӵ�������ʾƫ�����ݵ�λΪ����
		__HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);
		}
	if(htim==&htim7)
	{
		//���µ��ת��
		if(h_output<0&&x_output<0)//��ʱ������ΪӦ�ã�����ǰ
		{
			speed_control(35,40);
		}
		else if(h_output<0&&x_output>0)//��ʱ������ΪӦ�ã�����ǰ
		{
			speed_control(40,35);
		}
		else if(h_output>0&&x_output<0)//��ʱ������ΪӦ�ã������
		{
			speed_control(-35,-40);
		}
		else if(h_output>0&&x_output>0)//��ʱ������ΪӦ�ã����Һ�
		{
			speed_control(-40,-35);
		}
		else speed_control(20,20);
		__HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);
	}
}
//�������ļ��ĳ�ʼ������
void ALL_Init(void)
{
	PID pidx={0};

	PID_Init(&pidx, 10, 1, 5, 800, 1000);//��ʼ��PID����
	PWM_on();
	HAL_UART_Receive_IT(&huart3, &received_data, 1);//�������ڽ�������ͷ����
	__HAL_TIM_CLEAR_IT(&htim6, TIM_IT_UPDATE);
	HAL_TIM_Base_Start_IT(&htim6);//��ʱ��6�жϣ����ڸ���x.y��ƫ�����ֵ
	__HAL_TIM_CLEAR_IT(&htim7, TIM_IT_UPDATE);
	HAL_TIM_Base_Start_IT(&htim7);//��ʱ��7�жϣ����ڸ��µ���ת���Լ�����
}

//ִ�а������
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
void output_data_update(void)//С���˶�״̬�µ����ݸ���
{
	printf("X=%d   H=%d\n", x_output,h_output);
}
void mode1_stop(void)//��Ӧ����1��ָ��
{
	//Transmit_to_Camera(0x01);
	speed_control(0,0);//С��ֹͣ
}
//void mode2_number(void)
//{
//	Transmit_to_Camera(0x02);
//	printf("Mode2_number start!\n");
//	target_length=length_data;//���������ݴ浽Ŀ��������������
//	printf("Target_Length=%d\n", target_length);
//}
void mode3_car_run(void)
{
	//Transmit_to_Camera(0x03);
	printf("Now car run!\n");
	printf("Target_Length=%d\n", target_length);
	speed_control(40,40);//��ǰ��
}
	