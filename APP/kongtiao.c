#include "kongtiao.h"
#include "SysTick.h"

void TIM_PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);		
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* PA1引脚设置 */
	
	TIM_TimeBaseStructure.TIM_Period = 378;  		//72 000khz/378/5 = 38.09khz
	TIM_TimeBaseStructure.TIM_Prescaler = 5-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	//TIM2预分频设置:72kHZ。APB1分频系数2，输入到TIM3时钟为36MHzx2 = 72MHz  

	/* Channel 2 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
	//	TIM_OCInitStructure.TIM_Pulse=500;//设置占空比时间
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);

	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable); //使能预装载寄存器

	//使能TIM2定时计数器
	TIM_Cmd(TIM2, ENABLE);
}

void Lead_Code(void)
{
	TIM_SetCompare2(TIM2,189); //接收器拉低
	delay_us(4360);
	TIM_SetCompare2(TIM2,0); //接收器拉高
	delay_us(4460);
}

void Stop_Code(void)
{
	TIM_SetCompare2(TIM2,189); //接收器拉低
	delay_us(477);
	TIM_SetCompare2(TIM2,0); //接收器拉高
	delay_us(5277);
}

void Send_0_Code(void)
{
	TIM_SetCompare2(TIM2,189); //接收器拉低
	delay_us(478);
	TIM_SetCompare2(TIM2,0); //接收器拉高
	delay_us(600);
}

void Send_1_Code(void)
{
	TIM_SetCompare2(TIM2,189); //接收器拉低
	delay_us(478);
	TIM_SetCompare2(TIM2,0); //接收器拉高
	delay_us(1700);
}

void Send_Byte(u8 data)
{
	int i;
	for(i=7;i>=0;i--)
	{
		if(data & (1<<i))
		{
			Send_1_Code();
		}
		else
		{
			Send_0_Code();
		}
	}
}

void Normal_Code(u8 A, u8 B, u8 C)
{
	Lead_Code();
	Send_Byte(A);
	Send_Byte(~A);
	Send_Byte(B);
	Send_Byte(~B);
	Send_Byte(C);
	Send_Byte(~C);
	Stop_Code();
	Lead_Code();
	Send_Byte(A);
	Send_Byte(~A);
	Send_Byte(B);
	Send_Byte(~B);
	Send_Byte(C);
	Send_Byte(~C);
	Stop_Code();
}

void guangji(u8 A, u8 B, u8 C, u8 Q, u8 Y)
{
	Lead_Code();
	Send_Byte(A);
	Send_Byte(~A);
	Send_Byte(B);
	Send_Byte(~B);
	Send_Byte(C);
	Send_Byte(~C);
	Stop_Code();
	
	Lead_Code();
	Send_Byte(A);
	Send_Byte(~A);
	Send_Byte(B);
	Send_Byte(~B);
	Send_Byte(C);
	Send_Byte(~C);
	Stop_Code();
	
	Lead_Code();
	Send_Byte(A);
	Send_Byte(~A);
	Send_Byte(Q);
	Send_Byte(~Q);
	Send_Byte(Y);
	Send_Byte(~Y);
	Stop_Code();
}


