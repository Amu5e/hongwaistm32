#ifndef _kongtiao_H
#define _kongtiao_H

#include "system.h" 
#include "stdio.h" 

void TIM_PWM_Init(void);
void Lead_Code(void);
void Stop_Code(void);
void Send_0_Code(void);
void Send_1_Code(void);
void Send_Byte(u8 data);
void Normal_Code(u8 A, u8 B, u8 C);
void guangji(u8 A, u8 B, u8 C, u8 Q, u8 Y);

#endif


