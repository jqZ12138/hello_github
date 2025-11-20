#include "stm32f10x.h"   
#include "muc_RS232.h"
#include "queue.h"

#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"

extern Queue rx_queue;
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
  
		//bit15，	接收完成标志    
		//bit14，	接收到0x0d  
		//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	 

void RS232_Init (u32 pclk2,u32 bound) 
{  //配置波特率参数 72M，115200
//	float temp;
//	u16 mantissa;
//	u16 fraction;	   
//	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
//	mantissa=temp;				 //得到整数部分
//	fraction=(temp-mantissa)*16; //得到小数部分	 
//    mantissa<<=4;
//	mantissa+=fraction;  mantissa=0X0271;	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB2ENR|=1<<14;  //使能串口时钟 
	
	GPIOA->CRH&=0XFFFFF00F;//IO状态设置 配置A9A10
	GPIOA->CRH|=0X000008B0;//IO状态设置 
	
	RCC->APB2RSTR|=1<<14;   //复位串口1
	RCC->APB2RSTR&=~(1<<14);//停止复位	   	   
	//波特率设置
 	USART1->BRR=0X0271; // 波特率设置	 
	USART1->CR1|=0X202C;  //1位停止,无校验位. 接收缓冲区非空中断使能	
	
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;  /* 配置中断源*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  /* 配置抢占优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  /* 配置子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  /* 使能中断通道 */
  NVIC_Init(&NVIC_InitStructure);      //F0
   
}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
	if(USART1->SR&(1<<5))	//接收到数据
	{	 
		Res=USART1->DR; 
		queue_enqueue(&rx_queue, Res);
		//USART1->DR=Res;  		 									     
	}
} 


