#ifndef __APP_CFG_H
#define __APP_CFG_H

#include "stm32f10x.h"  

extern unsigned char gImage_bmp[];  //
extern volatile int xx,yy; 


extern volatile uint32_t msTicks; 
extern void Delay (uint32_t dlyTicks);
extern void Delay_ms (uint32_t dlyTicks); 


#endif /* __APP_CFG_H */
