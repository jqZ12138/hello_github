#include "stm32f10x.h"  
#include "appcfg.h"
#include "muc_LED.h"
#include "muc_EXIT.h"
#include "rtc.h"
#include "muc_lcd93xx.h"
extern volatile  u8 mode;
extern _calendar_obj alarm_time;


void muc_EXIT_Init(void) 
{
  NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	//RCC->APB2ENR |= 0x15;	               /* Enable GPIOA GPIOD afio clock            */
	
	RCC->APB2ENR|=1<<2;     //使能PORTA时钟
	RCC->APB2ENR|=1<<4;     //使能PORTC时钟
	RCC->APB2ENR |= 1<<0;     // 使能AFIO时钟（重要！）
	GPIOA->CRL&=0XFFFFFFF0;	//PA0设置成输入	  
	GPIOA->CRL|=0X00000008;   
	GPIOA->CRH&=0X0FFFFFFF;	//PA15设置成输入	  
	GPIOA->CRH|=0X80000000; 			 
	GPIOA->ODR|=1<<15;	   	//PA15上拉,PA0默认下拉
	GPIOC->CRL&=0XFF0FFFFF;	//PC5设置成输入	  
	GPIOC->CRL|=0X00400000;  
	GPIOC->ODR|=1<<5;	   	//PC5上拉
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);/* 设置NVIC为优先级组2 */  
	//SCB->AIRCR=0XFA050500;  //设置分组	  

  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;  /* 配置中断源：按键1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  /* 抢占式优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  /* 使能中断通道 */
  NVIC_Init(&NVIC_InitStructure);      //a0
	//NVIC->ISER0=0x0000 0040;    NVIC->IPR1=0x00A0 0000;
   
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; /* 配置中断源：按键2，同时使能多个中断 */  
  NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;  /* 子优先级 */
  NVIC_Init(&NVIC_InitStructure);	
	//NVIC->ISER1=0x0000 0100;			NVIC->IPR10=0x0000 0080;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; /* 配置中断源：按键3，同时使能多个中断 */  
  NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;  /* 子优先级 */
  NVIC_Init(&NVIC_InitStructure);


  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); /* 选择EXTI信号源 */
	//AFIO->EXTICR1=0;
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;	
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	/* 上升沿中断 */  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;/* 使能中断 */	
  EXTI_Init(&EXTI_InitStructure);
  
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line5;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	/* 下降沿中断 */  
  EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line15;	
  EXTI_Init(&EXTI_InitStructure);
	//EXTI->IMR=0x0000 A001;
}


// 外部中断处理函数实现keyup
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    { 
        LED_Off();
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line5) != RESET)
  {
				volatile uint32_t i;
        for(i = 0; i < 5000; i++); // 短暂延时
        
        if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == 0) // 确认按键状态
        {
            LED_Off();
						if(mode!=2&&mode!=3)
						{
							if(mode ==1)
							{mode=0;}
							else mode=1;
						}
        }       
    EXTI_ClearITPendingBit(EXTI_Line5);
  }
}

void EXTI15_10_IRQHandler(void) // key1 - 设置报警时间
{   
    // 检查EXTI Line15中断
    if(EXTI_GetITStatus(EXTI_Line15) != RESET)
    {
				volatile uint32_t i;
        for(i = 0; i < 5000; i++); // 短暂延时
        
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15) == 0) // 确认按键状态
        {
            LED_On();
						if(mode!=3)
						{
						if(mode ==2)
						{mode=0;}
						else mode=2; 
					}
						EXTI_ClearITPendingBit(EXTI_Line15);
    }}
}
