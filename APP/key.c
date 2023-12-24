#include "key.h"
#include "SysTick.h"

/*******************************************************************************
* 函 数 名         : KEY_Init
* 函数功能		   : 按键初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体变量	
	RCC_APB2PeriphClockCmd(KEY1_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY2_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY3_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY4_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=KEY1_PIN;//选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//上拉输入  
	GPIO_Init(KEY1_PORT,&GPIO_InitStructure);/* 初始化GPIO */
	
//	GPIO_InitStructure.GPIO_Pin=KEY2_PIN;
//	GPIO_Init(KEY2_PORT,&GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin=KEY3_PIN;
//	GPIO_Init(KEY3_PORT,&GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin=KEY4_PIN;
//	GPIO_Init(KEY4_PORT,&GPIO_InitStructure);
}

/*******************************************************************************
* 函 数 名         : KEY_Scan
* 函数功能		   : 按键扫描检测
* 输    入         : mode=0:单次按下按键
					 mode=1：连续按下按键
* 输    出         : 0：未有按键按下
					 KEY1_PRESS：KEY1键按下
					 KEY2_PRESS：KEY2键按下
					 KEY3_PRESS：KEY3键按下
					 KEY4_PRESS：KEY4键按下
*******************************************************************************/
u8 KEY_Scan(u8 mode)
{
	static u8 key=1;
	
	if(mode==1) //连续按键按下
		key=1;
	if(key==1&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0)) //任意一个按键按下
	{
		delay_ms(10);  //消抖
		key=0;
		if(KEY1==0)
			return KEY1_PRESS; 
		else if(KEY2==0)
			return KEY2_PRESS; 
		else if(KEY3==0)
			return KEY3_PRESS; 
		else if(KEY4==0)
			return KEY4_PRESS; 
	}
	else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)    //无按键按下
		key=1;
	return 0;
}


//矩阵按键初始化
void KEY_Matrix_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
	
	RCC_APB2PeriphClockCmd(KEY_MATRIX_H1_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY_MATRIX_H2_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY_MATRIX_H3_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY_MATRIX_H4_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY_MATRIX_L1_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY_MATRIX_L2_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY_MATRIX_L3_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY_MATRIX_L4_PORT_RCC,ENABLE);
	
	//行管脚输出配置
	GPIO_InitStructure.GPIO_Pin=KEY_MATRIX_H1_PIN;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(KEY_MATRIX_H1_PORT,&GPIO_InitStructure); 	   /* 初始化GPIO */
	
	GPIO_InitStructure.GPIO_Pin=KEY_MATRIX_H2_PIN; 
	GPIO_Init(KEY_MATRIX_H2_PORT,&GPIO_InitStructure); 	 
	
	GPIO_InitStructure.GPIO_Pin=KEY_MATRIX_H3_PIN; 
	GPIO_Init(KEY_MATRIX_H3_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=KEY_MATRIX_H4_PIN; 
	GPIO_Init(KEY_MATRIX_H4_PORT,&GPIO_InitStructure);
	
	//列管脚输入配置
	GPIO_InitStructure.GPIO_Pin=KEY_MATRIX_L1_PIN;  
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;	
	GPIO_Init(KEY_MATRIX_L1_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=KEY_MATRIX_L2_PIN;  
	GPIO_Init(KEY_MATRIX_L2_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=KEY_MATRIX_L3_PIN;  
	GPIO_Init(KEY_MATRIX_L3_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=KEY_MATRIX_L4_PIN;  
	GPIO_Init(KEY_MATRIX_L4_PORT,&GPIO_InitStructure);
}

/*******************************************************************************
* 函 数 名       : KEY_Matrix_Scan
* 函数功能		 : 检测矩阵按键是否按下，按下则返回对应键值
* 输    入       : 无
* 输    出    	 : key_value：1-16，对应S1-S16键，
				   0：按键未按下
*******************************************************************************/
u8 KEY_Matrix_Scan(void)
{
	u8 col1,col2,col3,col4;
	u8 key_value=0;
	
	GPIO_SetBits(KEY_MATRIX_H1_PORT,KEY_MATRIX_H1_PIN);  //先让L1输出高
	GPIO_SetBits(KEY_MATRIX_H2_PORT,KEY_MATRIX_H2_PIN);  //先让L2输出高
	GPIO_SetBits(KEY_MATRIX_H3_PORT,KEY_MATRIX_H3_PIN);  //先让L3输出高
	GPIO_SetBits(KEY_MATRIX_H4_PORT,KEY_MATRIX_H4_PIN);  //先让L4输出高
	
	if((GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN)|
		GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN)|
		GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN)|
		GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN))==0)  
        return 0; //如果X1到X4全为零则没有按键按下  
	else
	{	
		delay_ms(5);    //延时5ms去抖动
		if((GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN)|
			GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN)|
			GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN)|
			GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN))==0)
			return 0;
	}
	
	//第1行检测
	GPIO_SetBits(KEY_MATRIX_H1_PORT,KEY_MATRIX_H1_PIN);
	GPIO_ResetBits(KEY_MATRIX_H2_PORT,KEY_MATRIX_H2_PIN);
	GPIO_ResetBits(KEY_MATRIX_H3_PORT,KEY_MATRIX_H3_PIN);
	GPIO_ResetBits(KEY_MATRIX_H4_PORT,KEY_MATRIX_H4_PIN); 
	
	col1=GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN);
	col2=GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN);
  col3=GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN);
	col4=GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN);
	
	if(col1==1&&col2==0&&col3==0&&col4==0)
    key_value=1;
	if(col1==0&&col2==1&&col3==0&&col4==0)
		key_value=2;
	if(col1==0&&col2==0&&col3==1&&col4==0)
		key_value=3;
	if(col1==0&&col2==0&&col3==0&&col4==1)
		key_value=4;
	//等待按键松开
	while(((GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN))|
			(GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN))|
			(GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN))|
			(GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN))) > 0);

	
	//第2行检测
	GPIO_ResetBits(KEY_MATRIX_H1_PORT,KEY_MATRIX_H1_PIN);
	GPIO_SetBits(KEY_MATRIX_H2_PORT,KEY_MATRIX_H2_PIN);
	GPIO_ResetBits(KEY_MATRIX_H3_PORT,KEY_MATRIX_H3_PIN);
	GPIO_ResetBits(KEY_MATRIX_H4_PORT,KEY_MATRIX_H4_PIN); 
	
	col1=GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN);
	col2=GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN);
  col3=GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN);
	col4=GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN);
	
	if(col1==1&&col2==0&&col3==0&&col4==0)
    key_value=5;
	if(col1==0&&col2==1&&col3==0&&col4==0)
		key_value=6;
	if(col1==0&&col2==0&&col3==1&&col4==0)
		key_value=7;
	if(col1==0&&col2==0&&col3==0&&col4==1)
		key_value=8;
	//等待按键松开
	while(((GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN))|
			(GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN))|
			(GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN))|
			(GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN))) > 0);
	
	//第3行检测
	GPIO_ResetBits(KEY_MATRIX_H1_PORT,KEY_MATRIX_H1_PIN);
	GPIO_ResetBits(KEY_MATRIX_H2_PORT,KEY_MATRIX_H2_PIN);
	GPIO_SetBits(KEY_MATRIX_H3_PORT,KEY_MATRIX_H3_PIN);
	GPIO_ResetBits(KEY_MATRIX_H4_PORT,KEY_MATRIX_H4_PIN); 
	
	col1=GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN);
	col2=GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN);
  col3=GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN);
	col4=GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN);
	
	if(col1==1&&col2==0&&col3==0&&col4==0)
    key_value=9;
	if(col1==0&&col2==1&&col3==0&&col4==0)
		key_value=10;
	if(col1==0&&col2==0&&col3==1&&col4==0)
		key_value=11;
	if(col1==0&&col2==0&&col3==0&&col4==1)
		key_value=12;
	//等待按键松开
	while(((GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN))|
			(GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN))|
			(GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN))|
			(GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN))) > 0);
	
	//第4行检测
	GPIO_ResetBits(KEY_MATRIX_H1_PORT,KEY_MATRIX_H1_PIN);
	GPIO_ResetBits(KEY_MATRIX_H2_PORT,KEY_MATRIX_H2_PIN);
	GPIO_ResetBits(KEY_MATRIX_H3_PORT,KEY_MATRIX_H3_PIN);
	GPIO_SetBits(KEY_MATRIX_H4_PORT,KEY_MATRIX_H4_PIN); 
	
	col1=GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN);
	col2=GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN);
  col3=GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN);
	col4=GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN);
	
	if(col1==1&&col2==0&&col3==0&&col4==0)
    key_value=13;
	if(col1==0&&col2==1&&col3==0&&col4==0)
		key_value=14;
	if(col1==0&&col2==0&&col3==1&&col4==0)
		key_value=15;
	if(col1==0&&col2==0&&col3==0&&col4==1)
		key_value=16;
	//等待按键松开
	while(((GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN))|
			(GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN))|
			(GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN))|
			(GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN))) > 0);
	
	return key_value;
}



