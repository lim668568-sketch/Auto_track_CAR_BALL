#include "delay.h"

uint16_t g_fac_us;
 
void Delay_Init(uint16_t systclk)
{
	SysTick->CTRL=0;
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);
	g_fac_us = systclk/8;//9M速度计时
}

void delay_us(uint32_t nus)
{
	uint32_t temp;
	SysTick->LOAD=nus*g_fac_us;
	SysTick->VAL=0x00;
	SysTick->CTRL|=1<<0;
	do{
		temp = SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));
	SysTick->CTRL&=~(1<<0);
	SysTick->VAL=0x00;
}

void delay_ms(uint16_t nms)
{
	uint32_t repeat = nms/1000;
	uint32_t remain = nms%1000;
	while(repeat)
	{
		delay_us(1000*1000);
		repeat--;
	}
	if(remain)
	{
		delay_us(remain*1000);
	}
}

