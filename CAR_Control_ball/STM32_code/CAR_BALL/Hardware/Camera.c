#include "Camera.h"
uint8_t received_data=0;
uint8_t received_datapack[12]={0x35,0x20,0x2A,0x3B,0x00,0x00,0x5A,0x6B,0x00,0x00,0x00,0x55};//单片机接受的数据包,BBIIBB
uint8_t x[4]={0x00};
uint8_t h[4]={0x00};
int x_output=0;
int h_output=0;
uint16_t length_data=0;
const int target_length=20;//小车运动状态下的目标距离,死值

//发送按键值到摄像头
void Transmit_to_Camera(uint8_t key)
{
	uint8_t transmit_datapack[3]={0xAA,key,0x55};//数据包
	
	HAL_UART_Transmit(&huart3, (uint8_t *)transmit_datapack, sizeof(transmit_datapack),1);

}
//协议中低位在前
int handle_XorH(uint8_t *data)
{
	int final=0;
	final=(int32_t)(data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24));
	return final;
}

//接受摄像头所传的值
void Received_by_Camera(uint8_t received_data)
{

	static uint8_t state = 0;
	static uint8_t i = 0;
	if(state==0&&received_data==0x35)
	{
		state=1;
		i=0;
		received_datapack[i++]=received_data;
	}
	else if(state==1&&received_data==0x20)
	{
		state=2;
		received_datapack[i++]=received_data;
	}
	else if(state==2)
	{
		received_datapack[i++]=received_data;
		if(received_data==0x55)//接收到包围
		{
			state = 0;
			//解析数据
			for(int j=0;j<4;j++)
			{
				x[j]=received_datapack[2+j];
				h[j]=received_datapack[6+j];//对应数据依次放入对应数组
			}
			x_output=handle_XorH(x)-10000;
			h_output=handle_XorH(h)-10000;//低位在前解析数据
			// if(x_output<-150||x_output>130)
			// {
			// 	x_output=x_output;
			// }
			// else
			// {
			// 	x_output=50;
			// }
		}
//		else 
//		{
//			state = 0;
//            for(int i=0;i<12;i++)
//            {
//                received_datapack[i]=0x00;
//            }           
//		}	
	}
	else
		{
			state = 0;
            received_data=0;
			for(int i=0;i<12;i++)
            { 
                received_datapack[i]=0x00;
            }
		}	
}

void Camera_data_handle(void)
{
	
	
	Received_by_Camera(received_data);
	
	HAL_UART_Receive_IT(&huart3, &received_data, 1);//开启串口接收摄像头数据
}
//串口中断回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart3)//接收摄像头
	{
		Camera_data_handle();
	}	
}
