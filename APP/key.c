#include "key.h"
#include "SysTick.h"

/*******************************************************************************
* �� �� ��         : KEY_Init
* ��������		   : ������ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ�����	
	RCC_APB2PeriphClockCmd(KEY1_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY2_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY3_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY4_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=KEY1_PIN;//ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//��������  
	GPIO_Init(KEY1_PORT,&GPIO_InitStructure);/* ��ʼ��GPIO */
	
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
* �� �� ��         : KEY_Scan
* ��������		   : ����ɨ����
* ��    ��         : mode=0:���ΰ��°���
					 mode=1���������°���
* ��    ��         : 0��δ�а�������
					 KEY1_PRESS��KEY1������
					 KEY2_PRESS��KEY2������
					 KEY3_PRESS��KEY3������
					 KEY4_PRESS��KEY4������
*******************************************************************************/
u8 KEY_Scan(u8 mode)
{
	static u8 key=1;
	
	if(mode==1) //������������
		key=1;
	if(key==1&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0)) //����һ����������
	{
		delay_ms(10);  //����
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
	else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)    //�ް�������
		key=1;
	return 0;
}


//���󰴼���ʼ��
void KEY_Matrix_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����ṹ�����
	
	RCC_APB2PeriphClockCmd(KEY_MATRIX_H1_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY_MATRIX_H2_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY_MATRIX_H3_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY_MATRIX_H4_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY_MATRIX_L1_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY_MATRIX_L2_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY_MATRIX_L3_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY_MATRIX_L4_PORT_RCC,ENABLE);
	
	//�йܽ��������
	GPIO_InitStructure.GPIO_Pin=KEY_MATRIX_H1_PIN;  //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(KEY_MATRIX_H1_PORT,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
	
	GPIO_InitStructure.GPIO_Pin=KEY_MATRIX_H2_PIN; 
	GPIO_Init(KEY_MATRIX_H2_PORT,&GPIO_InitStructure); 	 
	
	GPIO_InitStructure.GPIO_Pin=KEY_MATRIX_H3_PIN; 
	GPIO_Init(KEY_MATRIX_H3_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=KEY_MATRIX_H4_PIN; 
	GPIO_Init(KEY_MATRIX_H4_PORT,&GPIO_InitStructure);
	
	//�йܽ���������
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
* �� �� ��       : KEY_Matrix_Scan
* ��������		 : �����󰴼��Ƿ��£������򷵻ض�Ӧ��ֵ
* ��    ��       : ��
* ��    ��    	 : key_value��1-16����ӦS1-S16����
				   0������δ����
*******************************************************************************/
u8 KEY_Matrix_Scan(void)
{
	u8 col1,col2,col3,col4;
	u8 key_value=0;
	
	GPIO_SetBits(KEY_MATRIX_H1_PORT,KEY_MATRIX_H1_PIN);  //����L1�����
	GPIO_SetBits(KEY_MATRIX_H2_PORT,KEY_MATRIX_H2_PIN);  //����L2�����
	GPIO_SetBits(KEY_MATRIX_H3_PORT,KEY_MATRIX_H3_PIN);  //����L3�����
	GPIO_SetBits(KEY_MATRIX_H4_PORT,KEY_MATRIX_H4_PIN);  //����L4�����
	
	if((GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN)|
		GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN)|
		GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN)|
		GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN))==0)  
        return 0; //���X1��X4ȫΪ����û�а�������  
	else
	{	
		delay_ms(5);    //��ʱ5msȥ����
		if((GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN)|
			GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN)|
			GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN)|
			GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN))==0)
			return 0;
	}
	
	//��1�м��
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
	//�ȴ������ɿ�
	while(((GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN))|
			(GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN))|
			(GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN))|
			(GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN))) > 0);

	
	//��2�м��
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
	//�ȴ������ɿ�
	while(((GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN))|
			(GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN))|
			(GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN))|
			(GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN))) > 0);
	
	//��3�м��
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
	//�ȴ������ɿ�
	while(((GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN))|
			(GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN))|
			(GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN))|
			(GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN))) > 0);
	
	//��4�м��
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
	//�ȴ������ɿ�
	while(((GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN))|
			(GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN))|
			(GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN))|
			(GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN))) > 0);
	
	return key_value;
}



