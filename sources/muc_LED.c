#include "stm32f10x.h"   
#include "muc_LED.h"

void LED_Initialize (void) {

  RCC->APB2ENR |= 0x24;	               /* Enable GPIOA GPIOD clock            */

	/* 设置GPIO D2为推挽输出模式，速率为50MHz */
		GPIOA->CRH &= 0xFFFFFFF0;
		GPIOA->CRH |= 0x00000003;
		GPIOD->CRL &= 0xFFFFF0FF;
		GPIOD->CRL |= 0x00000300;
}

void LED_On (void) {

    GPIOA->ODR &= 0XFEFF;  
		GPIOD->ODR &= 0XFFFB;    
}


void LED_Off (void) {
    GPIOA->ODR |= 1<<8;  
		GPIOD->ODR |= 1<<2;    
   
}
