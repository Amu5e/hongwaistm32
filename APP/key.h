	#ifndef _key_H
#define _key_H

#include "system.h"

//�ܽŶ���
#define KEY1_PORT 			GPIOA  
#define KEY1_PIN 			GPIO_Pin_15
#define KEY1_PORT_RCC		RCC_APB2Periph_GPIOA

#define KEY2_PORT 			GPIOA  
#define KEY2_PIN 			GPIO_Pin_14
#define KEY2_PORT_RCC		RCC_APB2Periph_GPIOA

#define KEY3_PORT 			GPIOA  
#define KEY3_PIN 			GPIO_Pin_13
#define KEY3_PORT_RCC		RCC_APB2Periph_GPIOA

#define KEY4_PORT 			GPIOA  
#define KEY4_PIN 			GPIO_Pin_12
#define KEY4_PORT_RCC		RCC_APB2Periph_GPIOA 	


//ʹ��λ��������
#define KEY1 	PAin(15)
#define KEY2 	PAin(14)
#define KEY3 	PAin(13)
#define KEY4 	PAin(12)

//�����������ֵ  
#define KEY1_PRESS 		1
#define KEY2_PRESS		2
#define KEY3_PRESS		3
#define KEY4_PRESS		4



//���󰴼��˿ڶ���
#define KEY_MATRIX_H1_PORT			GPIOB
#define KEY_MATRIX_H1_PIN 			GPIO_Pin_1
#define KEY_MATRIX_H1_PORT_RCC		RCC_APB2Periph_GPIOB

#define KEY_MATRIX_H2_PORT			GPIOB
#define KEY_MATRIX_H2_PIN 			GPIO_Pin_0
#define KEY_MATRIX_H2_PORT_RCC		RCC_APB2Periph_GPIOB

#define KEY_MATRIX_H3_PORT			GPIOA
#define KEY_MATRIX_H3_PIN 			GPIO_Pin_7
#define KEY_MATRIX_H3_PORT_RCC		RCC_APB2Periph_GPIOA

#define KEY_MATRIX_H4_PORT			GPIOA
#define KEY_MATRIX_H4_PIN 			GPIO_Pin_6
#define KEY_MATRIX_H4_PORT_RCC		RCC_APB2Periph_GPIOA

#define KEY_MATRIX_L1_PORT			GPIOA
#define KEY_MATRIX_L1_PIN 			GPIO_Pin_5
#define KEY_MATRIX_L1_PORT_RCC		RCC_APB2Periph_GPIOA

#define KEY_MATRIX_L2_PORT			GPIOA
#define KEY_MATRIX_L2_PIN 			GPIO_Pin_4
#define KEY_MATRIX_L2_PORT_RCC		RCC_APB2Periph_GPIOA

#define KEY_MATRIX_L3_PORT			GPIOA
#define KEY_MATRIX_L3_PIN 			GPIO_Pin_3
#define KEY_MATRIX_L3_PORT_RCC		RCC_APB2Periph_GPIOA

#define KEY_MATRIX_L4_PORT			GPIOA
#define KEY_MATRIX_L4_PIN 			GPIO_Pin_2
#define KEY_MATRIX_L4_PORT_RCC		RCC_APB2Periph_GPIOA
	

 
void KEY_Init(void);
u8 KEY_Scan(u8 mode);
void KEY_Matrix_Init(void);
u8 KEY_Matrix_Scan(void);

#endif
