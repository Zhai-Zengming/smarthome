#ifndef __BEEP_H
#define __BEEP_H	 
#include "sys.h"

#define BEEP_PORT		GPIOA
#define BEEP_RCC		RCC_APB2Periph_GPIOA
#define BEEP_PIN		GPIO_Pin_8
#define BEEP_PIN_MODE	GPIO_Mode_Out_PP

//�������˿ڶ���
#define BEEP PAout(8)	// BEEP,�������ӿ�	

//������У�������������ȡ�͵�ƽ����
#define BEEP_ON		1;
#define BEEP_OFF	0;

//����������״̬ȫ�ֱ���	1 ��  0�ر�
//Ĭ��Ϊ�ر�
extern u8 BEEP_Status;

//��������ʼ��,������Ĭ��Ϊ�ر�
void BEEP_Init(void);

//�رջ�򿪷�����	onoffΪ 1 ��  onoffΪ 0 �ر�
void BEEP_SetOnOff(u8 onoff);
		 				    
#endif

