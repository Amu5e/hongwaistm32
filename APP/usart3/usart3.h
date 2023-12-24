#ifndef __USART3_H
#define __USART3_H
	 
#include "system.h"  


#define USART3_MAX_RECV_LEN		100				//最大缓存字节数
#define USART3_MAX_SEND_LEN		100				//最大发送缓存字节数

extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//接收缓冲,USART3_MAX_LEN
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//发送缓冲,最大USART3_MAX_SEND_LEN字节

extern vu16 USART3_RX_STA;   					//接收数据状态

void USART3_Init(u32 bound);				//串口初始化 
void u3_printf(char* fmt,...);
u8 bt11_check(void);

#endif
