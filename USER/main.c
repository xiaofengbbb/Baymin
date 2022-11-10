#include "main.h"

#include "stm32f10x.h"
#include "Lcd_Driver.h"
#include "GUI.h"
#include "delay.h"
#include "TFT_demo.h"

#include "STM32F10x.h"

SendData send_data = {0,0, 0,0, 0.0, 0.0, LED0_OFF, FAN_OFF, RELAY_OFF, BEEP_OFF ,30, 180,30,0,0, 0,0};
uint8_t cmd_rev_flag = 0;
uint8_t video;
uint8_t videoscan=0;
uint8_t key;
u8 buff[30];//参数显示缓存数组
u8 display_contrl=0;
/*****************************************/
int main(void)
{	
	unsigned char *data_ptr = NULL;
	unsigned short time_count = 30;	//发送间隔变量
	
	hardware_init();
	OLED_Clear();
	net_init();
  OLED_Clear();

	//TIM3_Int_Init(9999,7999);
	
	while(1)
	{  
		
		data_get();//获取数据

//		delay_ms(1000);	
//		sprintf((char *)buff,"temp %d.%d   humi %d.%d    light:%d",send_data.humiH,send_data.humiL,send_data.tempH,send_data.tempL,send_data.light);
//		printf("light %d   mq %d  ",send_data.light,send_data.mq);
//		printf("video %d\r\n",video);
//		printf("led:%d fan:%d relay:%d beep:%d",send_data.led ,send_data.fan,send_data.relay,send_data.beep);
//		printf("max_tem:%d max_light:%d max_mq:%d\r\n",send_data.max_temp  ,send_data.max_light,send_data.max_mq);
//		OLED_Clear();
//		OLED_ShowString(0,0,buff,sizeof(buff));
		
		if((time_count >= 20) || (cmd_rev_flag == 1))	//发送间隔或者收到命令
		{	
			cmd_rev_flag = 0;

			OneNet_SendData();	//发送数据
			

			
			time_count = 0;
			ESP8266_Clear();
		}
		
		data_ptr = ESP8266_GetIPD(0);
		if(data_ptr != NULL)
		{
			OneNet_RevPro(data_ptr);
		}	

		data_user_handle();
	

	if(video==0)
	{
		videoscan=1;
		video=99;
	}
 if(videoscan==1&&video!=99)
	{
		if(video==7)send_data.mode=0;         //打开自动控制
		if(video==8)send_data .mode =1;        //打开手动控制
	}
/*****************************************/	
		//如果当前是自动模式
		if(send_data.mode==0)
		{
			//温度
			if(send_data.alarm_temp == 1) 
			{
				FAN =FAN_ON;
			}
		  else{
				FAN =FAN_OFF;
			}
			//灯光
			if(send_data.alarm_light == 1) 
			{
				LED0  =LED0_ON;
			}
			else{
				LED0  =LED0_OFF;
			}
				//报警处理
			if(send_data.alarm_mq == 1) 
			{
				BEEP =0;	
			}else BEEP=1;
		}
/*****************************************/
		//如果是手动模式
		if(send_data.mode ==1)
		{		
			video_handle();		
		}
		
		time_count ++;
	}		 

}


/*****************************************/

void hardware_init(void)
{
	

	char *p = NULL;
	SystemInit();	//System init.
	delay_init();	    	//延时函数初始化	

	OLED_Init();
//	OLED_ColorTurn(0);//0正常显示，1 反色显示w
//  OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
//	OLED_DrawPoint(0,0);
//	OLED_DrawLine(20,0,50,60);
//	OLED_DrawCircle(64,32,20);
	OLED_Clear();
	
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);		//串口1，调试用
	Usart2_Init(115200); 	//串口2，驱动ESP8266用
	usart3_init(9600);   //串口3，语音控制用

	p = BEEP_Init();			//蜂鸣器
	OLED_ShowString(8,0,p,sizeof(p));
	
//	delay_ms(1500);
	p = LED_Init();		  	 	//初始化与LED连接的硬件接口
	OLED_ShowString(8,16,p,sizeof(p));
//	delay_ms(1500);
	p = ADC1_Init();			// ADC 初始化
	OLED_ShowString(8,32,p,sizeof(p));
//	delay_ms(1500);
	p = RELAY_Init();			//继电器
	OLED_ShowString(8,48,p,sizeof(p));
//	delay_ms(1500);
	p = FAN_Init();				//风扇
	OLED_ShowString(8,64,p,sizeof(p));
	// delay_ms(1500);
	p=DHT11_Init();
	OLED_ShowString(8,80,p,sizeof(p)); //温湿度
	
//	delay_ms(1500);
	p = KEY_Init();				//按键
	OLED_ShowString(8,96,p,sizeof(p));	
	delay_ms(1500);

}




/*****************************************/
void net_init(void)
{
	ESP8266_Init();					//初始化ESP8266
	
	while(OneNet_DevLink())			//接入OneNET
		delay_ms(500);
	
	
	RELAY = RELAY_ON;
	LED0 = 0;						//入网成功

	sprintf((char *)buff,"Net Init Success！");
	OLED_ShowString(0,80,buff,12);
//	OLED_Refresh();
	
	delay_ms(100);
	
	RELAY = RELAY_OFF;
	delay_ms(900);
	OLED_Clear();
}



/*****************************************/
void data_get(void)
{
  DHT11_Read_Data(&send_data.humiH,&send_data.humiL,&send_data.tempH,&send_data.tempL);
	send_data.light = ADC1_Light_Process(1);
	send_data.mq = ADC1_mq135_Process(6);
	send_data.fan=FAN ;
	send_data.led=LED0 ;
	send_data.relay=RELAY ;
	send_data.beep=BEEP ;
}


/*****************************************/
void data_user_handle(void)
{
	//温度处理函数
	if(send_data.tempH >= send_data.max_temp)	
	{	
		send_data.alarm_temp = 1;
	}
	else 
	{
		send_data.alarm_temp = 0;

	}
	//mq处理函数
	if(send_data.mq >= send_data.max_mq)
	{
		send_data.alarm_mq = 1;
	}
	else
	{
		send_data.alarm_mq = 0;	
	}
	
	//光照处理
	if(send_data.light >= send_data.max_light)	
	{	
		send_data.alarm_light = 0;
	}
	else
	{
		send_data.alarm_light = 1;
	}
	


}


/*****************************************/
//语音处理
void video_handle(void)
{

		videoscan=0;
		switch(video)
		{
			case 1:LED0 =LED0_ON;break;        //开灯
			case 2:LED0 =LED0_OFF ;break;      //关灯
			case 3:RELAY = RELAY_ON;break;     //打开电视
			case 4:RELAY = RELAY_OFF;break;    //关闭电视
			case 5:FAN=FAN_ON;break;           //打开风扇
			case 6:FAN=FAN_OFF;break;          //关闭风扇
			case 7:send_data.mode=0;break;    //打开自动控制
			case 8:send_data.mode =1;break;   //打开手动控制
			case 9:BEEP =0;break;             //报警
			case 10:BEEP=1;break;             //关闭警报
		}
	}
//	else if(videoscan==0)
//	{
//		//温度
//		if(send_data.alarm_temp == 1) //
//			{
//				FAN =FAN_ON;
//				if(video==6){FAN=FAN_OFF;video =99;}
//			}
//		else{
//				FAN =FAN_OFF;
//				if(video==5)FAN=FAN_ON;
//			}
//			//灯光
//			if(send_data.alarm_light == 1) 
//			{
//				LED0  =LED0_ON;
//			}
//			else{
//				LED0  =LED0_OFF;
//			}
//	}


