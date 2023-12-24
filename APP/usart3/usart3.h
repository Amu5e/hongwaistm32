#ifndef __USART3_H
#define __USART3_H
	 
#include "system.h"  


#define USART3_MAX_RECV_LEN		100				//��󻺴��ֽ���
#define USART3_MAX_SEND_LEN		100				//����ͻ����ֽ���

extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//���ջ���,USART3_MAX_LEN
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�

extern vu16 USART3_RX_STA;   					//��������״̬

void USART3_Init(u32 bound);				//���ڳ�ʼ�� 
void u3_printf(char* fmt,...);
u8 bt11_check(void);

#endif
