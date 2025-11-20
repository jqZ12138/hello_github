#ifndef __muc_RS232_H
#define __muc_RS232_H

#include "stm32f10x.h" 
#include <stdint.h>

#define USART_REC_LEN  			200  	//定义最大接收字节数 200


extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目

extern void   RS232_Init (u32 pclk2,u32 bound);

#endif /* __muc_RS232_H */
