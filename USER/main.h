#ifndef __MAIN_H
#define __MAIN_H




#include "delay.h"
#include "oled.h"
#include "sys.h"
#include "usart.h"
#include "usart3.h"
#include "timer.h"

#include "adc.h"

#include "led.h"
#include "beep.h"
#include "dht11.h"

#include "esp8266.h"
#include "onenet.h"
#include "relay.h"
#include "fan.h"



#include "key.h"

typedef struct{
	uint8_t tempH;		//�¶�
	uint8_t tempL;
	uint8_t humiH;
  uint8_t humiL;	//ʪ��
	uint8_t light; 	//���ն�
	uint8_t mq;		//MQ������
	uint8_t led;	//LED
	uint8_t fan;	//����
	uint8_t relay;	//�̵���
	uint8_t beep;
	uint8_t max_temp;//�¶���ֵ
	uint8_t max_mq;//MQ��������ֵ
	uint8_t max_light;//light��ֵ
	uint8_t alarm_temp;  //�¶ȱ�����־  0��1��
	uint8_t alarm_mq;  //mq������־  0��1��
	uint8_t alarm_light;//���ձ�����־  0��1��
	uint8_t mode;
}SendData;

extern SendData send_data;
extern uint8_t cmd_rev_flag;


void hardware_init(void);	
void net_init(void);
void dataview(void);
void data_get(void);
void data_display(void);
void data_user_handle(void);
void video_handle(void);
#endif



















