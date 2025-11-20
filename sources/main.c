#include "stm32f10x.h" 
#include "appcfg.h"
#include <stdio.h>
#include "muc_RS232.h"     
#include "queue.h"
 #include "muc_EXIT.h"  
#include "muc_lcd93xx.h"
#include "rtc.h"
#include "muc_LED.h"
#include <stdlib.h>
#define BUFF_SIZE 128
	
volatile u8 mode=0;
volatile  u8 old_mode=0;
Queue rx_queue;

char* weekdays[]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saterday"};
uint8_t time_buffer[BUFF_SIZE]; 
uint8_t buffer_index = 0;
uint8_t data_ready = 0;   

int parse_time_string(char* time_str, _calendar_obj* time_struct);
int main(void)
{	u8 i;			
	uint8_t tbuf[40];
	uint8_t t = 0;
	uint8_t rx_char = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	SysTick_Config(72);     
	rtc_init();
	lcd_Init();
	LED_Initialize();
	RS232_Init(72,115200);
	queue_init(&rx_queue, 128);
	muc_EXIT_Init();
	while(1)
	{	if(mode!=old_mode)
		{LCD_Clear(WHITE);
			old_mode=mode;
		}
		if(mode==3)//正在报警
		{	
			for(i=5;i>0;i--)
			{LCD_ShowString(30, 160, 210, 16, 16, "alarm is ringing......");
			sprintf((char *)tbuf, "Mode:%d", mode);			
      LCD_ShowString(30, 180, 100, 16, 16, (char*)tbuf);
			sprintf((char *)tbuf, "countdown lasting:%ds", i);
      LCD_ShowString(30, 140, 210, 16, 16, (char*)tbuf);			
			LCD_ShowString(30, 200, 210, 16, 16, "                    ");			
			if(i==1){
			LCD_ShowString(30, 200, 210, 16, 16, "alarm end");}
			Delay_ms(1000);				
			}		
			mode=0;
		}
		else if(mode==2)//报警设置时间模式
		{
			//LCD_Clear(WHITE);			
			rtc_get_time();
			rtc_set_alarm(2025,11,19,14,20,30);
			LCD_ShowString(30, 160, 210, 16, 16, "alarm is seting......");
			sprintf((char *)tbuf, "Mode:%d", mode);  
			LCD_ShowString(30, 180, 100, 16, 16, (char*)tbuf);			
		}			
		else if(mode==1)
		{
			//LCD_Clear(WHITE);
			sprintf((char *)tbuf, "Mode:%d", mode);
			LCD_ShowString(30, 120, 210, 16, 16, "time is setting......");
			LCD_ShowString(30, 140, 210, 16, 16, "The input format is:");
			LCD_ShowString(30, 160, 210, 16, 16, "year-mon-dat week hour:min:sec");
      LCD_ShowString(30, 180, 100, 16, 16, (char*)tbuf);
			if(queue_dequeue(&rx_queue, &rx_char))
			{
					if(rx_char == '\n' || rx_char == '\r')
					{
							if(buffer_index > 0)
							{
									time_buffer[buffer_index] = '\0';  
									data_ready = 1;  
									buffer_index = 0;
							}
					}
					else
					{
							if(buffer_index < sizeof(time_buffer) - 1)
							{
									time_buffer[buffer_index++] = rx_char;
							}
					}
				}
			if(data_ready)
			{
				
					if(parse_time_string((char*)time_buffer, &calendar))
					{
						data_ready = 0; 
						rtc_set_time(calendar.year,calendar.month, calendar.date, calendar.hour, calendar.min, calendar.sec);
						calendar.week= rtc_get_week(calendar.year,calendar.month, calendar.date); 
						LCD_ShowString(30, 200, 210, 16, 16, "Setup successful");						
					}
			}
			//mode=0;
			//Delay(500);
		}
		else if(mode==0)
        if ((t % 10) == 0)  
        {
						sprintf((char *)tbuf, "Mode:%d", mode);  
            LCD_ShowString(30, 180, 100, 16, 16, (char*)tbuf);
            rtc_get_time();
            sprintf((char *)tbuf, "Time:%02d:%02d:%02d", calendar.hour, calendar.min, calendar.sec);
            LCD_ShowString(30, 120, 210, 16, 16, (char*)tbuf);
            sprintf((char *)tbuf, "Date:%04d-%02d-%02d", calendar.year, calendar.month, calendar.date);
            LCD_ShowString(30, 140, 210, 16, 16, (char*)tbuf);
            sprintf((char *)tbuf, "Week:%s", weekdays[calendar.week]);
            LCD_ShowString(30, 160, 210, 16, 16, (char *)tbuf);
					  
        }				

		      Delay(10);                                            // Wait 500ms  
					t++;				
	}	
			 LED_On();
				Delay_ms(500);
				LED_Off();
				Delay_ms(500);
}

int parse_time_string(char* time_str, _calendar_obj* time_struct)
{
   
    if(strlen(time_str) < 19)  
        return 0;
       
    if(sscanf(time_str, "%hu-%hhu-%hhu %hhu %hhu:%hhu:%hhu",
              &time_struct->year,
              &time_struct->month,
              &time_struct->date,
              &time_struct->week,
              &time_struct->hour,
              &time_struct->min,
              &time_struct->sec) == 7)
    {
       
        if(time_struct->month >= 1 && time_struct->month <= 12 &&
           time_struct->date >= 1 && time_struct->date <= 31 &&
           time_struct->week <= 6 &&
           time_struct->hour <= 23 &&
           time_struct->min <= 59 &&
           time_struct->sec <= 59)
        {
            return 1;  
        }
    }
    
    return 0; 
}                           