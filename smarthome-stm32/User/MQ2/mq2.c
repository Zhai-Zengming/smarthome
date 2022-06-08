#include "mq2.h"
//#include "timer.h"
#include "adc_dma.h"
#include "stdio.h"
#include "math.h"
#include "delay.h"

//�õ�ADC�����ڲ���������ֵ
//ȡ10��,Ȼ��ƽ��
vu16 Get_ADCValue_MQ2(void)
{
	u32 val = 0;
	u8 times = 10;
	u8 count;
	for(count = 0; count < times; count++)
	{
		val += ADC_ConvertedValue;//��ȡDMAͨ��ֵ
		delay_ms(5);
	}
	return val/times;
}

// MQ2���������ݴ���
float MQ2_GetPPM(void)
{	
    float Vrl = 3.3f * Get_ADCValue_MQ2() / 4096.f;
	float ppm = Vrl*100;
    return  ppm;
}
		 







