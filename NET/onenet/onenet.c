#include "stm32f10x.h"
#include <string.h>
#include <stdio.h>
#include "esp8266.h"
#include "onenet.h"
#include "mqttkit.h"
#include "led.h"
#include "main.h"

#define PROID		"554594" //��ƷID

#define AUTH_INFO	"1234"	//��Ȩ��Ϣ

#define DEVID		"1012222074"	//�豸ID
extern u8 buff[30];//������ʾ��������
extern unsigned char esp8266_buf[256];
u8 status_connected = 0;

//==========================================================
//	�������ƣ�	OneNet_DevLink
//
//	�������ܣ�	��onenet��������
//
//	��ڲ�����	��
//
//	���ز�����	1-�ɹ�	0-ʧ��
//
//	˵����		��onenetƽ̨��������
//==========================================================
_Bool OneNet_DevLink(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					//Э���

	unsigned char *dataPtr;
	
	_Bool status = 1;
	
	printf("OneNet_DevLink\r\nPROID: %s,	AUIF: %s,	DEVID:%s\r\n", PROID, AUTH_INFO, DEVID);
	
	if(MQTT_PacketConnect(PROID, AUTH_INFO, DEVID, 256, 0, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0)
	{
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//�ϴ�ƽ̨
		
		dataPtr = ESP8266_GetIPD(250);									//�ȴ�ƽ̨��Ӧ
		if(dataPtr != NULL)
		{
			if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK)
			{
				switch(MQTT_UnPacketConnectAck(dataPtr))
				{
					case 0:printf("Tips:	���ӳɹ�\r\n");status = 0;break;
					
					case 1:printf("WARN:	����ʧ�ܣ�Э�����\r\n");break;
					case 2:printf("WARN:	����ʧ�ܣ��Ƿ���clientid\r\n");break;
					case 3:printf("WARN:	����ʧ�ܣ�������ʧ��\r\n");break;
					case 4:printf("WARN:	����ʧ�ܣ��û������������\r\n");break;
					case 5:printf("WARN:	����ʧ�ܣ��Ƿ�����(����token�Ƿ�)\r\n");break;
					
					default:printf("ERR:	����ʧ�ܣ�δ֪����\r\n");break;
				}
			}
		}
		
		MQTT_DeleteBuffer(&mqttPacket);								//ɾ��
	}
	else
		printf("WARN:	MQTT_PacketConnect Failed\r\n");
	
	return status;
	
}

unsigned char OneNet_FillBuf(char *buf)
{
	char text[32];
	
	memset(text, 0, sizeof(text));
	
	strcpy(buf, ",;");
	
	//�¶�
	memset(text, 0, sizeof(text));
	sprintf(text, "temp,%d.%d;", send_data.tempH,send_data.tempL);
	strcat(buf, text);
	//ʪ��
	memset(text, 0, sizeof(text));
	sprintf(text, "humi,%d.%d;", send_data.humiH,send_data.humiL);
	strcat(buf, text);
	//����
	memset(text, 0, sizeof(text));
	sprintf(text, "light,%d;", send_data.light);
	strcat(buf, text);
	//mq
	memset(text, 0, sizeof(text));
	sprintf(text, "mq,%d;", send_data.mq);
	strcat(buf, text);
	//led
	memset(text, 0, sizeof(text));
	sprintf(text, "led,%d;", send_data.led);
	strcat(buf, text);	
	//����
	memset(text, 0, sizeof(text));
	sprintf(text, "fan,%d;", send_data.fan);
	strcat(buf, text);
	//�̵���
	memset(text, 0, sizeof(text));
	sprintf(text, "relay,%d;", send_data.relay);
	strcat(buf, text);
	//������
	memset(text, 0, sizeof(text));
	sprintf(text, "beep,%d;", send_data.beep);
	strcat(buf, text);
	//MQ��������ֵ
	memset(text, 0, sizeof(text));
	sprintf(text, "max_mq,%d;", send_data.max_mq);
	strcat(buf, text);
	//�¶�����
	memset(text, 0, sizeof(text));
	sprintf(text, "max_temp,%d;", send_data.max_temp);
	strcat(buf, text);
	//��������
	memset(text, 0, sizeof(text));
	sprintf(text, "max_light,%d;", send_data.max_light);
	strcat(buf, text);
	//�¶ȱ���
	memset(text, 0, sizeof(text));
	sprintf(text, "alarm_temp,%d;", send_data.alarm_temp);
	strcat(buf, text);
	//mq����
	memset(text, 0, sizeof(text));
	sprintf(text, "alarm_mq,%d;", send_data.alarm_mq);
	strcat(buf, text);
	//mq����
	memset(text, 0, sizeof(text));
	sprintf(text, "alarm_light,%d;", send_data.alarm_light);
	strcat(buf, text);
	//mode �Զ��ֶ�ģʽ
	memset(text, 0, sizeof(text));
	sprintf(text, "mode,%d;", send_data.mode);
	strcat(buf, text);
	return strlen(buf);

}

//==========================================================
//	�������ƣ�	OneNet_SendData
//
//	�������ܣ�	�ϴ����ݵ�ƽ̨
//
//	��ڲ�����	type���������ݵĸ�ʽ
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void OneNet_SendData(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};												//Э���
	
	char buf[256];
	
	short body_len = 0, i = 0;
	
	printf("Tips:	OneNet_SendData-MQTT\r\n");
	
	memset(buf, 0, sizeof(buf));
	
	body_len = OneNet_FillBuf(buf);																	//��ȡ��ǰ��Ҫ���͵����������ܳ���
	
	if(body_len)
	{
		if(MQTT_PacketSaveData(DEVID, body_len, NULL, 5, &mqttPacket) == 0)							//���
		{
			for(; i < body_len; i++)
				mqttPacket._data[mqttPacket._len++] = buf[i];
			
			ESP8266_SendData(mqttPacket._data, mqttPacket._len);									//�ϴ����ݵ�ƽ̨
			printf("Send %d Bytes\r\n", mqttPacket._len);
			LED1 = 0;
			MQTT_DeleteBuffer(&mqttPacket);															//ɾ��
		}
		else
		{
			printf("WARN:	EDP_NewBuffer Failed\r\n");
			LED1 = 1;
		}
	}
	
}

//==========================================================
//	�������ƣ�	OneNet_RevPro
//
//	�������ܣ�	ƽ̨�������ݼ��
//
//	��ڲ�����	dataPtr��ƽ̨���ص�����
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void OneNet_RevPro(unsigned char *cmd)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};								//Э���
	
	char *req_payload = NULL;
	char *cmdid_topic = NULL;
	
	unsigned short req_len = 0;
	
	unsigned char type = 0;
	
	short result = 0;

	char *dataPtr = NULL;
	char numBuf[10];
	int num = 0;
	
	type = MQTT_UnPacketRecv(cmd);
	switch(type)
	{
		case MQTT_PKT_CMD:															//�����·�
			
			result = MQTT_UnPacketCmd(cmd, &cmdid_topic, &req_payload, &req_len);	//���topic����Ϣ��
			if(result == 0)
			{
				printf("cmdid: %s, req: %s, req_len: %d\r\n", cmdid_topic, req_payload, req_len);
				
				if(MQTT_PacketCmdResp(cmdid_topic, req_payload, &mqttPacket) == 0)	//����ظ����
				{
					printf("Tips:	Send CmdResp\r\n");
					OLED_Clear();
					sprintf((char*)buff,"Recive message!");
					OLED_ShowString(0,0,buff,12);//fan
					OLED_Refresh();
					
					ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//�ظ�����
					MQTT_DeleteBuffer(&mqttPacket);									//ɾ��
				}
			}
		
		break;
			
		case MQTT_PKT_PUBACK:														//����Publish��Ϣ��ƽ̨�ظ���Ack
		
			if(MQTT_UnPacketPublishAck(cmd) == 0)
				printf("Tips:	MQTT Publish Send OK\r\n");
			
		break;
		
		default:
			result = -1;
		break;
	}
	
	ESP8266_Clear();									//��ջ���
	
	if(result == -1)
		return;
	
	dataPtr = strchr(req_payload, ':');					//����'}'

	if(dataPtr != NULL && result != -1)					//����ҵ���
	{
		dataPtr++;
		
		while(*dataPtr >= '0' && *dataPtr <= '9')		//�ж��Ƿ����·��������������
		{
			numBuf[num++] = *dataPtr++;
		}
		numBuf[num] = 0;
		
		num = atoi((const char *)numBuf);				//תΪ��ֵ��ʽ
		
		if(strstr((char *)req_payload, "led"))		
		{
			send_data.led = num;
			LED0 = num;
			OLED_Clear();
			sprintf((char*)buff,"led:%d",num);
			OLED_ShowString(0,0,buff,12);//fan
			OLED_Refresh();
				
		}
		else if(strstr((char *)req_payload, "relay"))		
		{
			send_data.relay = num;
			RELAY = num;
			OLED_Clear();
			sprintf((char*)buff,"relay:%d",num);
			OLED_ShowString(0,0,buff,12);//fan
			OLED_Refresh();
		}
		else if(strstr((char *)req_payload, "fan"))		
		{
			send_data.fan = num;
			FAN = num;
			OLED_Clear();
			sprintf((char*)buff,"fan:%d",num);
			OLED_ShowString(0,0,buff,12);//fan
			OLED_Refresh();
		}
		else if(strstr((char *)req_payload, "beep"))		
		{
			send_data.beep = num;
			BEEP = num;
			OLED_Clear();
			sprintf((char*)buff,"beep=%d",num);
			OLED_ShowString(0,0,buff,12);//fan
			OLED_Refresh();
		}
		else if(strstr((char *)req_payload, "max_temp"))		
		{
			send_data.max_temp = num;
			OLED_Clear();
			sprintf((char*)buff,"max_temp=%d",num);
			OLED_ShowString(0,0,buff,12);//fan
			OLED_Refresh();
		}
		else if(strstr((char *)req_payload, "max_mq"))		
		{
			send_data.max_mq = num;
			OLED_Clear();
			sprintf((char*)buff,"max_mq=%d",num);
			OLED_ShowString(0,0,buff,12);//fan
			OLED_Refresh();
		}
		else if(strstr((char *)req_payload, "max_light"))		
		{
			send_data.max_light = num;
			OLED_Clear();
			sprintf((char*)buff,"max_light=%d",num);
			OLED_ShowString(0,0,buff,12);//fan
			OLED_Refresh();
		}
		else if(strstr((char *)req_payload, "mode"))		
		{
			send_data.mode = num;
			OLED_Clear();
			sprintf((char*)buff,"mode=%d",num);
			OLED_ShowString(0,0,buff,12);//fan
			OLED_Refresh();
			
		}
		cmd_rev_flag = 1;
	}

	if(type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH)
	{
		MQTT_FreeBuffer(cmdid_topic);
		MQTT_FreeBuffer(req_payload);
	}

}
