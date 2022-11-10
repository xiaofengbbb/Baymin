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
u8 buff[30];//������ʾ��������
u8 display_contrl=0;
/*****************************************/
int main(void)
{	
	unsigned char *data_ptr = NULL;
	unsigned short time_count = 30;	//���ͼ������
	
	hardware_init();
	OLED_Clear();
	net_init();
  OLED_Clear();

	//TIM3_Int_Init(9999,7999);
	
	while(1)
	{  
		
		data_get();//��ȡ����

//		delay_ms(1000);	
//		sprintf((char *)buff,"temp %d.%d   humi %d.%d    light:%d",send_data.humiH,send_data.humiL,send_data.tempH,send_data.tempL,send_data.light);
//		printf("light %d   mq %d  ",send_data.light,send_data.mq);
//		printf("video %d\r\n",video);
//		printf("led:%d fan:%d relay:%d beep:%d",send_data.led ,send_data.fan,send_data.relay,send_data.beep);
//		printf("max_tem:%d max_light:%d max_mq:%d\r\n",send_data.max_temp  ,send_data.max_light,send_data.max_mq);
//		OLED_Clear();
//		OLED_ShowString(0,0,buff,sizeof(buff));
		
		if((time_count >= 20) || (cmd_rev_flag == 1))	//���ͼ�������յ�����
		{	
			cmd_rev_flag = 0;

			OneNet_SendData();	//��������
			

			
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
		if(video==7)send_data.mode=0;         //���Զ�����
		if(video==8)send_data .mode =1;        //���ֶ�����
	}
/*****************************************/	
		//�����ǰ���Զ�ģʽ
		if(send_data.mode==0)
		{
			//�¶�
			if(send_data.alarm_temp == 1) 
			{
				FAN =FAN_ON;
			}
		  else{
				FAN =FAN_OFF;
			}
			//�ƹ�
			if(send_data.alarm_light == 1) 
			{
				LED0  =LED0_ON;
			}
			else{
				LED0  =LED0_OFF;
			}
				//��������
			if(send_data.alarm_mq == 1) 
			{
				BEEP =0;	
			}else BEEP=1;
		}
/*****************************************/
		//������ֶ�ģʽ
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
	delay_init();	    	//��ʱ������ʼ��	

	OLED_Init();
//	OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾw
//  OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ
//	OLED_DrawPoint(0,0);
//	OLED_DrawLine(20,0,50,60);
//	OLED_DrawCircle(64,32,20);
	OLED_Clear();
	
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);		//����1��������
	Usart2_Init(115200); 	//����2������ESP8266��
	usart3_init(9600);   //����3������������

	p = BEEP_Init();			//������
	OLED_ShowString(8,0,p,sizeof(p));
	
//	delay_ms(1500);
	p = LED_Init();		  	 	//��ʼ����LED���ӵ�Ӳ���ӿ�
	OLED_ShowString(8,16,p,sizeof(p));
//	delay_ms(1500);
	p = ADC1_Init();			// ADC ��ʼ��
	OLED_ShowString(8,32,p,sizeof(p));
//	delay_ms(1500);
	p = RELAY_Init();			//�̵���
	OLED_ShowString(8,48,p,sizeof(p));
//	delay_ms(1500);
	p = FAN_Init();				//����
	OLED_ShowString(8,64,p,sizeof(p));
	// delay_ms(1500);
	p=DHT11_Init();
	OLED_ShowString(8,80,p,sizeof(p)); //��ʪ��
	
//	delay_ms(1500);
	p = KEY_Init();				//����
	OLED_ShowString(8,96,p,sizeof(p));	
	delay_ms(1500);

}




/*****************************************/
void net_init(void)
{
	ESP8266_Init();					//��ʼ��ESP8266
	
	while(OneNet_DevLink())			//����OneNET
		delay_ms(500);
	
	
	RELAY = RELAY_ON;
	LED0 = 0;						//�����ɹ�

	sprintf((char *)buff,"Net Init Success��");
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
	//�¶ȴ�����
	if(send_data.tempH >= send_data.max_temp)	
	{	
		send_data.alarm_temp = 1;
	}
	else 
	{
		send_data.alarm_temp = 0;

	}
	//mq������
	if(send_data.mq >= send_data.max_mq)
	{
		send_data.alarm_mq = 1;
	}
	else
	{
		send_data.alarm_mq = 0;	
	}
	
	//���մ���
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
//��������
void video_handle(void)
{

		videoscan=0;
		switch(video)
		{
			case 1:LED0 =LED0_ON;break;        //����
			case 2:LED0 =LED0_OFF ;break;      //�ص�
			case 3:RELAY = RELAY_ON;break;     //�򿪵���
			case 4:RELAY = RELAY_OFF;break;    //�رյ���
			case 5:FAN=FAN_ON;break;           //�򿪷���
			case 6:FAN=FAN_OFF;break;          //�رշ���
			case 7:send_data.mode=0;break;    //���Զ�����
			case 8:send_data.mode =1;break;   //���ֶ�����
			case 9:BEEP =0;break;             //����
			case 10:BEEP=1;break;             //�رվ���
		}
	}
//	else if(videoscan==0)
//	{
//		//�¶�
//		if(send_data.alarm_temp == 1) //
//			{
//				FAN =FAN_ON;
//				if(video==6){FAN=FAN_OFF;video =99;}
//			}
//		else{
//				FAN =FAN_OFF;
//				if(video==5)FAN=FAN_ON;
//			}
//			//�ƹ�
//			if(send_data.alarm_light == 1) 
//			{
//				LED0  =LED0_ON;
//			}
//			else{
//				LED0  =LED0_OFF;
//			}
//	}


