#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "oled.h"
#include "key.h"
#include "24cxx.h"
#include "kongtiao.h"
#include "rtc.h" 
#include "usart3.h"

u8 Bma[]={0x1f,0x9f,0x5f,0x3f};
u8 Cmode[]={0X08,0X00,0X04,0XE4,0X0C};
u8 Ctem[]={0X00,0X10,0X30,0X20,0X60,0X70,0X50,0X40,0XC0,0XD0,0X90,0X80,0XA0,0XB0};
u8 A=0xB2,B,C;
TypeTime MyTime;

int main()
{
	u8 t,b,m,key,kts;
	u8 i,k,h,j,l,uflag,t1,t2,u1flag;
	u8 tflag=0,reclen=0;
	u16 senddat=0;
	u8 fama[238];
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组分2组
	USART1_Init(9600);
	USART3_Init(9600);
//	KEY_Init();
	KEY_Matrix_Init();
	AT24CXX_Init();
	TIM_PWM_Init();
	RTC_Init();
	OLED_Init();
	
	OLED_Clear();

	while(bt11_check())
	{
		printf("BT11 Check Ernor!\r\n");
		delay_ms(500);
		OLED_ShowString(0,0,"BT11 Check Ernor!",12);
		OLED_Refresh_Gram();
	}
	OLED_Clear();
	printf("BT11 Check OK\r\n");
	OLED_ShowString(0,0,"BT11 Check OK   ",12);
	OLED_Refresh_Gram();

	while(AT24CXX_Check())  //检测AT24C02是否正常
	{
		printf("AT24C02检测不正常!\r\n");
		delay_ms(500);
		OLED_ShowString(0,1*16,"AT24C02 Check Ernor!",12);
		OLED_Refresh_Gram();

	}

//	AT24CXX_WriteOneByte(32000,0);
//	AT24CXX_WriteOneByte(32001,0);
//	AT24CXX_WriteOneByte(32002,0);
//	AT24CXX_WriteOneByte(32003,0);
//	AT24CXX_WriteOneByte(32004,0);
	
	printf("AT24C02检测正常!\r\n");
	OLED_ShowString(0,1*16,"AT24C02 Check OK!",12);
	OLED_Refresh_Gram();
	delay_ms(500);
	t=AT24CXX_ReadOneByte(32000);
	b=AT24CXX_ReadOneByte(32001);
	kts=AT24CXX_ReadOneByte(32002);
	m=AT24CXX_ReadOneByte(32003);
	h=AT24CXX_ReadOneByte(32004);

	OLED_Clear();
	
//	AT24CXX_WriteOneByte(0,0xFD);
//	AT24CXX_WriteOneByte(1,0xFD);

	C=Ctem[t]| Cmode[m];
	B=Bma[b];
	
	for(j=0;j<3;j++)
	{
		for(i=0;i<3;i++)
		{
			OLED_ShowFontHZ((i+3)*16-7,j*16,11+(j*3)+i,16,1);
		}
	}
	OLED_Refresh_Gram();
	
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1);
	t1=AT24CXX_ReadOneByte(h*236);
	delay_ms(1);
	t2=AT24CXX_ReadOneByte((h*236)+1);
	
	OLED_Clear();
	
	switch(b)
	{
		case 0:
		{
			OLED_ShowString(0,0,"Auto",16);break; 
		}
		case 1:
		{
			OLED_ShowString(0,0," Low",16); break; 
		}
		case 2:
		{
			OLED_ShowString(0,0," Mid",16); break; 
		}
		case 3:
		{
			OLED_ShowString(0,0,"High",16); break; 
		}
	}
	OLED_ShowNum(6*8,0*16,t+17,2,16);
	OLED_ShowFontHZ(4*16,0,10,16,1);
	for(i=0;i<2;i++)
	{
		OLED_ShowFontHZ((6+i)*16,0,m*2+i,16,1);
	}
	OLED_ShowNum(0*8,1*16,h,3,16);
	OLED_ShowString(4*7,1*16,":",16);
	OLED_Refresh_Gram();
	
	Normal_Code(0x00, 0x00, 0x00);
	
	while(1)
	{	
		key=KEY_Matrix_Scan();
		
		if(key==2||l==2)
		{
			l=0;
			if(t++== 13) t=0;
			C=Ctem[t]| Cmode[m];
			Normal_Code(A, B, C);
//			USART_SendData(USART1,A);	
//			delay_ms(5);
//			USART_SendData(USART1,B);	
//			delay_ms(5);
//			USART_SendData(USART1,C);	
			OLED_ShowNum(6*8,0,t+17,2,16);
			OLED_Refresh_Gram();
			uflag = 1;
			AT24CXX_WriteOneByte(32000,t);
		}
		
		else if(key==1||l==1)
		{    
			l=0;
			if(t--==0) t=13;
			C=Ctem[t]| Cmode[m];;			
			Normal_Code(A, B, C); 
//			USART_SendData(USART1,A);	
//			delay_ms(5);
//			USART_SendData(USART1,B);	
//			delay_ms(5);
//			USART_SendData(USART1,C);	
			OLED_ShowNum(6*8,0,t+17,2,16);
			OLED_Refresh_Gram();	
			uflag = 1;
			AT24CXX_WriteOneByte(32000,t);
		}
		
		else if(key==3||l==3)
		{	
						l=0;
			if(b++>2) b=0;
			B=Bma[b];
			Normal_Code(A, B, C); 
//			USART_SendData(USART1,A);	
//			delay_ms(5);
//			USART_SendData(USART1,B);	
//			delay_ms(5);
//			USART_SendData(USART1,C);	
			switch(b)
			{
				case 0:
				{
					OLED_ShowString(0,0,"Auto",16);break; 
				}
				case 1:
				{
					OLED_ShowString(0,0," Low",16); break; 
				}
				case 2:
				{
					OLED_ShowString(0,0," Mid",16); break; 
				}
				case 3:
				{
					OLED_ShowString(0,0,"High",16); break; 
				}
			}
			OLED_Refresh_Gram();
			uflag = 1;
			AT24CXX_WriteOneByte(32001,b);
		}
		
		else if(key==4||l==4)
		{
						l=0;
			if(m++==4) m=0;
			C=Ctem[t]|Cmode[m];
			Normal_Code(A, B, C); 
//			USART_SendData(USART1,A);	
//			delay_ms(5);
// 			USART_SendData(USART1,B);	
//			delay_ms(5);
//			USART_SendData(USART1,C);	
			for(i=0;i<2;i++)
			{
				OLED_ShowFontHZ((6+i)*16,0,m*2+i,16,1);
			}
			OLED_Refresh_Gram();
			uflag = 1;			
			AT24CXX_WriteOneByte(32003,m);
		}
		
		else if(key==6|l==6)
		{
			l=0;
			if(h++==100) h=0;
			OLED_ShowNum(0*8,1*16,h,3,16);
			OLED_Refresh_Gram();
			AT24CXX_WriteOneByte(32004,h);
			delay_ms(1);
			t1=AT24CXX_ReadOneByte(h*236);
			delay_ms(1);
			t2=AT24CXX_ReadOneByte((h*236)+1);
		}
		
		else if(key==5|l==5)
		{
			l=0;
			if(h--==0) h=100;
			OLED_ShowNum(0*8,1*16,h,3,16);
			OLED_Refresh_Gram();
			AT24CXX_WriteOneByte(32004,h);
			delay_ms(1);
			t1=AT24CXX_ReadOneByte(h*236);
			delay_ms(1);
			t2=AT24CXX_ReadOneByte((h*236)+1);
		}

		else if(key==9|l==9)
		{
			u1flag=1;
			l=0;
			USART_SendData(USART1,0xFD);			delay_ms(1);
			USART_SendData(USART1,0xFD);			delay_ms(1);
			USART_SendData(USART1,0xF1);			delay_ms(1);
			USART_SendData(USART1,0xF2);			delay_ms(1);
			for(i=0;i<228;i++)
			{
				USART_SendData(USART1,0x00);		delay_ms(2);
			}
			USART_SendData(USART1,0xF1);			delay_ms(1);
			USART_SendData(USART1,0xF2);			delay_ms(1);
			USART_SendData(USART1,0xDF);			delay_ms(1);
			USART_SendData(USART1,0xDF);			delay_ms(2);
			OLED_ShowString(0*16,2*16,"Studying ",16);
			OLED_Refresh_Gram();
			u3_printf("Studying\r\n");
			printf("\r\n");printf("\r\n");printf("\r\n");
			printf("Studying\r\n");
			printf("\r\n");printf("\r\n");printf("\r\n");
			while(u1flag)
			{
				if(KEY_Matrix_Scan()==9||l==9) 
				{
					u1flag=0;
					l=0;
				}
				if(USART1_RX_STA&0X8000)	//接收到一次数据了
				{
//					USART1_RX_BUF[236]='\0';
//					Normal_Code(0xB2, 0x9F, 0x00); 		//制冷 低风 17
//					printf("USART1_RX_BUF=%s\r\n",USART1_RX_BUF);
					for(i=0;i<234;i++)
					{
//						USART_SendData(USART1,USART1_RX_BUF[i]);		
						delay_ms(2);
						fama[i]=USART1_RX_BUF[i];
					}
					fama[234]=0xDF;
					fama[235]=0xDF;
//					USART_SendData(USART1,0xDF);		delay_ms(2);
//					USART_SendData(USART1,0xDF);		delay_ms(2);
					USART1_RX_STA=0;
					u1flag=0;
				}
				OLED_Refresh_Gram();
				if(USART3_RX_STA&0X8000)	//接收到一次数据了
				{
					switch(USART3_RX_BUF[0])
					{
						case 8:
						{
							l=9;break; 
						}
					}
					USART3_RX_STA=0;
				}
				if(USART3_RX_STA&0X8000)	//接收到一次数据了
				{
					switch(USART1_RX_BUF[0])
					{
						case 8:
						{
							l=9;break; 
						}
					}
					USART1_RX_STA=0;
				}
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15))
				{
					OLED_ShowFontHZ(7*16,1*16,20,16,1); 					OLED_Refresh_Gram();
				}
				else
				{
					OLED_ShowString(7*16,1*16,"B",16); 					OLED_Refresh_Gram();
				}
			}
			USART1_RX_STA=0;
			OLED_ShowString(0*16,2*16,"        ",16);
			OLED_Refresh_Gram();
			u3_printf("Studied\r\n");
			printf("Studied\r\n");
			printf("\r\n");printf("\r\n");printf("\r\n");
		}
	
		else if(key==7|l==7)
		{
			l=0;
			AT24CXX_WriteOneByte(h*236,0xDF);
			delay_ms(1);
			AT24CXX_WriteOneByte((h*236)+1,0xDF);
			delay_ms(1);
			t1=AT24CXX_ReadOneByte(h*236);
			delay_ms(1);
			t2=AT24CXX_ReadOneByte((h*236)+1);
			u3_printf("Deleted\r\n");
			printf("Deleted\r\n");
		}
		
		else if(key==10|l==10)
		{
			l=0;
			if(fama[0]!=0||fama[1]!=0)
			{
				for(i=0;i<236;i++)
				{
					USART_SendData(USART1,fama[i]);  delay_ms(2);
				}
				delay_ms(10);
				if(USART1_RX_BUF[0]==0xFD||USART1_RX_BUF[0]==0xF1||USART1_RX_BUF[0]==0x09)
				{
					USART1_RX_BUF[0]=0xFD;
					OLED_ShowString(5*16,2*16,"Sended",16);
					OLED_Refresh_Gram();
					printf("\r\n");printf("\r\n");printf("\r\n");
					u3_printf("Sended\r\n");
					printf("Sended\r\n");
					printf("\r\n");printf("\r\n");printf("\r\n");
				}
				USART1_RX_STA=0;
				USART1_RX_BUF[0]=0xFD;
				USART1_RX_STA=0;
				delay_ms(10);
				OLED_ShowString(5*16,2*16,"         ",16);
				OLED_Refresh_Gram();
			}
		}
		
		else if(key==12|l==12)
		{
			l=0;
			if(t1==0xFD && t2==0xFD)
			{
				for(i=0;i<236;i++)
				{
					USART_SendData(USART1,AT24CXX_ReadOneByte((h*236)+i));  delay_ms(2);
				}
				delay_ms(10);
				if(USART1_RX_BUF[0]==0xFD||USART1_RX_BUF[0]==0xF1||USART1_RX_BUF[0]==0x0B)
				{
					USART1_RX_BUF[0]=0xFD;
					OLED_ShowString(5*16,2*16,"Sended",16);
					OLED_Refresh_Gram();
					printf("\r\n");printf("\r\n");printf("\r\n");
					u3_printf("Sended\r\n");
					printf("Sended\r\n");
					printf("\r\n");printf("\r\n");printf("\r\n");
				}
			}
			USART1_RX_STA=0;
			USART1_RX_BUF[0]=0xFD;
			delay_ms(10);
			OLED_ShowString(5*16,2*16,"         ",16);
			OLED_Refresh_Gram();
		}
		
		else if(key==11||l==11)
		{
			l=0;
			for(i=0;i<236;i++)
			{
				AT24CXX_WriteOneByte((h*236)+i,fama[i]);  delay_ms(2);
			}
			t1=AT24CXX_ReadOneByte(h*236);
			delay_ms(1);
			t2=AT24CXX_ReadOneByte((h*236)+1);
			u3_printf("Deposited\r\n");
			printf("Deposited\r\n");
		}
		
		else if(key==13||l==13)
		{
			l=0;
			tflag=1;
			OLED_ShowString(0,3*16,"Setting",16); 					OLED_Refresh_Gram();
			RCC_RTCCLKCmd(DISABLE);	//使能RTC时钟  
			MyTime.hour=calendar.hour;
			MyTime.min=calendar.min;
			while(tflag)
			{
				k=KEY_Matrix_Scan();
				if(k==2||l==2)
				{
					l=0;
					if(MyTime.hour++ == 23) MyTime.hour=0;
					OLED_ShowNum(5*16-16,3*16,MyTime.hour,2,16);				OLED_Refresh_Gram();
				}
				else if(k==1||l==1)
				{   
					l=0;					
					if(MyTime.hour-- == 0) MyTime.hour=23;
					OLED_ShowNum(5*16-16,3*16,MyTime.hour,2,16);				OLED_Refresh_Gram();
				}
				else if(k==6||l==6)
				{    
					l=0;					
					if(MyTime.min++ == 59)  MyTime.min=0;
					OLED_ShowNum(6*16-8,3*16,MyTime.min,2,16);					OLED_Refresh_Gram();
				}
				else if(k==5||l==5)
				{    
					l=0;					
					if(MyTime.min-- == 0)  MyTime.min=59;
					OLED_ShowNum(6*16-8,3*16,MyTime.min,2,16);					OLED_Refresh_Gram();
				}
				else if(k==13||l==13)
				{    
					l=0;					
					tflag=0; 
					OLED_ShowString(0,3*16,"        ",16); 
					RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟  
				}
				
				if(USART3_RX_STA&0X8000)	//接收到一次数据了
				{
//			reclen=USART3_RX_STA&0X7FFF;	//得到数据长度
//		  USART3_RX_BUF[reclen]='\0';	 	//加入结束符
//			printf("reclen=%d\r\n",reclen);
//			printf("USART3_RX_BUF=%s\r\n",USART3_RX_BUF);
					switch(USART3_RX_BUF[0])
					{
						case 0:
						{
							l=1;break; 
						}
						case 1:
						{
							l=2;break; 
						}
						case 5:
						{
							l=6;break; 
						}	
						case 4:
						{
							l=5;break; 
						}	
						case 0X0C:
						{
							l=13;break;  
						}
					}
					USART3_RX_STA=0;
//			reclen=0;
				}
				
				if(USART1_RX_STA&0X8000)	//接收到一次数据了
				{
					switch(USART1_RX_BUF[0])
					{
						case 0:
						{
							l=1;break; 
						}
						case 1:
						{
							l=2;break; 
						}
						case 5:
						{
							l=6;break; 
						}	
						case 4:
						{
							l=5;break; 
						}	
						case 0X0C:
						{
							l=13;break;  
						}
					}
					USART1_RX_STA=0;
				}
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15))
				{
					OLED_ShowFontHZ(7*16,1*16,20,16,1); 					OLED_Refresh_Gram();
				}
				else
				{
					OLED_ShowString(7*16,1*16,"B",16); 					OLED_Refresh_Gram();
				}
			}
			RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date, MyTime.hour,MyTime.min,calendar.sec);  //设置时间	
		}
		
		else if(key==16||l==16)
		{
			l=0;
			kts=!kts;
			if(kts)
			{
				guangji(0xB2, 0x7B, 0xE0,0x00,0x00);
				printf ("关机\r\n");
				u3_printf("Close\r\n");
			}
			else
			{
				Normal_Code(A, B, C); 
				printf ("开机\r\n");
				u3_printf("Open\r\n");
			}
			AT24CXX_WriteOneByte(32002,kts);
		}
		
		else if(key==8||l==8)
		{
			l=0;
			if(t1==0xFD && t2==0xFD)
			{
				for(i=0;i<236;i++)
				{
					USART_SendData(USART3,AT24CXX_ReadOneByte((h*236)+i));  delay_ms(2);
				}
				delay_ms(10);
				OLED_ShowString(5*16,2*16,"Sended",16);
				OLED_Refresh_Gram();
			}
			USART3_RX_STA=0;
			delay_ms(10);
			OLED_ShowString(5*16,2*16,"         ",16);
			OLED_Refresh_Gram();		
		}
		
		else if(key==14||l==14)
		{
			l=0;
			senddat++;
			u3_printf("Send Date:%d\r\n",senddat);
			if(senddat>10)senddat=0;
		}
		
		else if(key==15||l==15)
		{
			l=0;
//			USART1_RX_BUF[236]='\0';
			Normal_Code(0xB2, 0x9F, 0x00); //制冷 低风 17
//			Normal_Code(0x00, 0x00, 0x00);
//			printf("USART1_RX_BUF=%s\r\n",USART1_RX_BUF);
		}
		
		if(uflag==1)
		{
			uflag=0;
			printf ("温度:%d\t",t+17);
			u3_printf ("Temperature:%d\t",t+17);
			switch(b)
			{
				case 0:
				{
				  printf ("风速:自动\t");
					u3_printf("Auto\t");break; 
				}
				case 1:
				{
					printf ("风速:低速\t");
					u3_printf("Low\t");break; 
				}
				case 2:
				{
				  printf ("风速:中速\t");
					u3_printf("Mid\t");break; 
				}
				case 3:
				{
				  printf ("风速:高速\t");
					u3_printf("High\t");break; 
				}	
			}
			switch(m)
			{
				case 0:
				{
				  printf ("模式:自动\r\n");
					u3_printf("Auto\r\n");break; 
				}
				case 1:
				{
					printf ("模式:制冷\r\n");
					u3_printf("Cold\r\n");break; 

				}
				case 2:
				{
				  printf ("模式:除湿\r\n");
					u3_printf("Arefaction\r\n");break; 
				}
				case 3:
				{
				  printf ("模式:送风\r\n");
					u3_printf("Blowing\r\n");break; 
				}	
				case 4:
				{
				  printf ("模式:制热\r\n");
					u3_printf("Hot\r\n");break; 
				}	
			}
		}
		
		if(USART3_RX_STA&0X8000)	//接收到一次数据了
		{
//			reclen=USART3_RX_STA&0X7FFF;	//得到数据长度
//		  USART3_RX_BUF[reclen]='\0';	 	//加入结束符
//			printf("reclen=%d\r\n",reclen);
//			printf("USART3_RX_BUF=%s\r\n",USART3_RX_BUF);
			switch(USART3_RX_BUF[0])
			{
				case 0:
				{
				  l=1;break; 
				}
				case 1:
				{
					l=2;break; 
				}
				case 2:
				{
				  l=3;break; 
				}
				case 3:
				{
				  l=4;break; 
				}	
				case 4:
				{
				  l=5;break; 
				}	
				case 5:
				{
				  l=6;break; 
				}	
				case 6:
				{
				  l=7;break; 
				}	
				case 7:
				{
				  l=8;break; 
				}	
				case 8:
				{
				  l=9;break; 
				}	
				case 9:
				{
				  l=10;break; 
				}	
				case 0X0A:
				{
				  l=11;break; 
				}	
				case 0X0B:
				{
				  l=12;break; 
				}	
				case 0X0C:
				{
				  l=13;break;  
				}
				case 0X0D:
				{
					l=14;break; 
				}
				case 0X0E:
				{
				  l=15;break; 
				}
				case 0X0F:
				{
				  l=16;break; 
				}	
			}
//			switch(USART3_RX_BUF[3])
//			{
//				case 0x43:
//				{
//					OLED_ShowString(0,3*16,"B",16); 					OLED_Refresh_Gram();break; 
//				}
//			}
//			switch(USART3_RX_BUF[1])
//			{
//				case 0x43:
//				{
//					OLED_ShowString(0,3*16,"C",16); 					OLED_Refresh_Gram();break; 
//				}
//			}
			USART3_RX_STA=0;
//			reclen=0;
		}
		
		if(USART1_RX_STA&0x8000)	//接收到一次数据了
		{
			switch(USART1_RX_BUF[0])
			{
				case 0:
				{
				  l=1;break; 
				}
				case 1:
				{
					l=2;break; 
				}
				case 2:
				{
				  l=3;break; 
				}
				case 3:
				{
				  l=4;break; 
				}	
				case 4:
				{
				  l=5;break; 
				}	
				case 5:
				{
				  l=6;break; 
				}	
				case 6:
				{
				  l=7;break; 
				}	
				case 7:
				{
				  l=8;break; 
				}	
				case 8:
				{
				  l=9;break; 
				}	
				case 9:
				{
				  l=10;break; 
				}	
				case 0X0A:
				{
				  l=11;break; 
				}	
				case 0X0B:
				{
				  l=12;break; 
				}	
				case 0X0C:
				{
				  l=13;break;  
				}
				case 0X0D:
				{
					l=14;break; 
				}
				case 0X0E:
				{
				  l=15;break; 
				}
				case 0X0F:
				{
				  l=16;break; 
				}	
			}
			USART1_RX_STA=0;
		}
		
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15))
		{
			OLED_ShowFontHZ(7*16,1*16,20,16,1); 					OLED_Refresh_Gram();
		}
		else
		{
			OLED_ShowString(7*16,1*16,"B",16); 					OLED_Refresh_Gram();
		}
		
		if(t1==0xFD && t2==0xFD)
		{
			OLED_ShowString(3*16,1*16,"Exist ",16);
			OLED_Refresh_Gram();
		}
		else
		{
			OLED_ShowString(3*16,1*16,"None  ",16);
			OLED_Refresh_Gram();
		}
		
		OLED_Refresh_Gram();
		
	}
}

