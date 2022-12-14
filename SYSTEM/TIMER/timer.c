#include "timer.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "delay.h"
#include "main.h"
 
extern u8 buff[30];//参数显示缓存数组
extern u8 video;
extern u8 display_contrl;
//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
//	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM3, //TIM2
		TIM_IT_Update ,
		ENABLE  //使能
		);
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
	
	return ;
							 
}

void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
//		display_contrl++;
//		if(display_contrl%2==0)
//		{
			OLED_Clear ();	
			sprintf((char *)buff,"Mode:%d",send_data.mode);
			OLED_ShowString(0,0,buff,12);
			sprintf((char *)buff,"Video:%d",video);
			OLED_ShowString(62,0,buff,12);
			sprintf((char*)buff,"Temp:%d.%d",send_data.tempH,send_data.tempL);
			OLED_ShowString(0,16,buff,12);//温度
			sprintf((char*)buff,"humi:%d.%d",send_data.humiH,send_data.humiL);
			OLED_ShowString(0,32,buff,12);//湿度
			sprintf((char*)buff,"light:%d",send_data.light);
			OLED_ShowString(0,48,buff,12);//光照
			sprintf((char*)buff,"mq:%d",send_data.mq);
			OLED_ShowString(0,64,buff,12);//MQ
			
			sprintf((char*)buff,"max_temp:%d",send_data.max_temp);
			OLED_ShowString(0,80,buff,12);//最大温度
			sprintf((char*)buff,"min_light:%d",send_data.max_light);
			OLED_ShowString(0,96,buff,12);//最低光照
			sprintf((char*)buff,"max_mq:%d",send_data.max_mq);
			OLED_ShowString(0,112,buff,12);//最大mq
			
			
//			OLED_Refresh();
//		}
//		if(display_contrl%2==1)
//		{
//			
//			OLED_Clear ();
//			sprintf((char*)buff,"fan:%d",send_data.fan);
//			OLED_ShowString(0,0,buff,12);//fan
//			sprintf((char*)buff,"relay:%d",send_data.relay);
//			OLED_ShowString(0,10,buff,12);//最大mq
//			sprintf((char*)buff,"beep:%d",send_data.beep);
//			OLED_ShowString(0,20,buff,12);//最大mq
//			OLED_Refresh();
//		
//		}
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //??TIMx?????? (??????)
}












