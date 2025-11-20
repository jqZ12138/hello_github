#include "stm32f10x.h"                  // Device header
#include "appcfg.h"

volatile int xx,yy;   //全局变量，定义图像的顶点位置 xx yy


volatile uint32_t msTicks;                                 // counts 1ms timeTicks
void SysTick_Handler(void) {
  msTicks++;
}

// * Delay: delays a number of Systicks
void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
}
void Delay_ms (uint32_t dlyTicks) {//ms
  while (dlyTicks--)
	{Delay(1000);}
}
