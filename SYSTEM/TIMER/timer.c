#include "timer.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "delay.h"
#include "main.h"
 
extern u8 buff[30];//������ʾ��������
extern u8 video;
extern u8 display_contrl;
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
//	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM3, //TIM2
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
	
	return ;
							 
}

void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
//		display_contrl++;
//		if(display_contrl%2==0)
//		{
			OLED_Clear ();	
			sprintf((char *)buff,"Mode:%d",send_data.mode);
			OLED_ShowString(0,0,buff,12);
			sprintf((char *)buff,"Video:%d",video);
			OLED_ShowString(62,0,buff,12);
			sprintf((char*)buff,"Temp:%d.%d",send_data.tempH,send_data.tempL);
			OLED_ShowString(0,16,buff,12);//�¶�
			sprintf((char*)buff,"humi:%d.%d",send_data.humiH,send_data.humiL);
			OLED_ShowString(0,32,buff,12);//ʪ��
			sprintf((char*)buff,"light:%d",send_data.light);
			OLED_ShowString(0,48,buff,12);//����
			sprintf((char*)buff,"mq:%d",send_data.mq);
			OLED_ShowString(0,64,buff,12);//MQ
			
			sprintf((char*)buff,"max_temp:%d",send_data.max_temp);
			OLED_ShowString(0,80,buff,12);//����¶�
			sprintf((char*)buff,"min_light:%d",send_data.max_light);
			OLED_ShowString(0,96,buff,12);//��͹���
			sprintf((char*)buff,"max_mq:%d",send_data.max_mq);
			OLED_ShowString(0,112,buff,12);//���mq
			
			
//			OLED_Refresh();
//		}
//		if(display_contrl%2==1)
//		{
//			
//			OLED_Clear ();
//			sprintf((char*)buff,"fan:%d",send_data.fan);
//			OLED_ShowString(0,0,buff,12);//fan
//			sprintf((char*)buff,"relay:%d",send_data.relay);
//			OLED_ShowString(0,10,buff,12);//���mq
//			sprintf((char*)buff,"beep:%d",send_data.beep);
//			OLED_ShowString(0,20,buff,12);//���mq
//			OLED_Refresh();
//		
//		}
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //??TIMx?????? (??????)
}












