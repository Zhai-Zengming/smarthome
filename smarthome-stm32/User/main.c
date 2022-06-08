/**
  ******************************************************************************
  * @file    main.c
  * @date    2022-06-01
  * @brief   智能家居stm32部分
  ******************************************************************************
  */ 
 
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include "bsp_esp8266.h"
#include "bsp_esp8266_test.h"
#include "bsp_dht11.h"
#include "bsp_led.h"  
#include "core_delay.h"
#include "usart2.h"
#include "mq2.h"
#include "adc_dma.h"
#include "delay.h"
#include "beep.h"
#include "timer.h"

float MQ2_value;
uint32_t t = 0;            //定时2S

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main ( void )
{
	uint32_t t = 0;
	/* 初始化 */
	delay_init();
    USART_Config ();                                                      //初始化串口1
	GENERAL_TIM_Init();
	USART2_Config();
    CPU_TS_TmrInit();                                                     //初始化DWT计数器，用于延时函数
    LED_GPIO_Config();                                                    //初始化RGB彩灯
	ESP8266_Init ();                                                      //初始化WiFi模块使用的接口和外设
	ADCx_Init();
	BEEP_Init();

	printf ( "\r\n野火开发板：WF-ESP8266 WiFi模块通信；红外控制空调；有害气体检测；开锁\r\n" );

    ESP8266_StaTcpClient_Unvarnish_ConfigTest();                          //对ESP8266进行配置

    while ( 1 )
    {
		t++;
		if(t == 360000)
		{
			t = 0;
			MQ2_value = MQ2_GetPPM();
			printf("MQ2 = %d\r\n", (int)MQ2_value);
		}
		
		if(MQ2_value > 90)
		{
			BEEP_SetOnOff(1);
			LED_RED;
		}else{
			BEEP_SetOnOff(0);
		}
		
    	ESP8266_CheckRecvDataTest(); // ESP8266 检查一次是否接收到了数据
    }
  
}


