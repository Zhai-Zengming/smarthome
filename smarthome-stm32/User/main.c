/**
  ******************************************************************************
  * @file    main.c
  * @date    2022-06-01
  * @brief   ���ܼҾ�stm32����
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
uint32_t t = 0;            //��ʱ2S

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main ( void )
{
	uint32_t t = 0;
	/* ��ʼ�� */
	delay_init();
    USART_Config ();                                                      //��ʼ������1
	GENERAL_TIM_Init();
	USART2_Config();
    CPU_TS_TmrInit();                                                     //��ʼ��DWT��������������ʱ����
    LED_GPIO_Config();                                                    //��ʼ��RGB�ʵ�
	ESP8266_Init ();                                                      //��ʼ��WiFiģ��ʹ�õĽӿں�����
	ADCx_Init();
	BEEP_Init();

	printf ( "\r\nҰ�𿪷��壺WF-ESP8266 WiFiģ��ͨ�ţ�������ƿյ����к������⣻����\r\n" );

    ESP8266_StaTcpClient_Unvarnish_ConfigTest();                          //��ESP8266��������

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
		
    	ESP8266_CheckRecvDataTest(); // ESP8266 ���һ���Ƿ���յ�������
    }
  
}


