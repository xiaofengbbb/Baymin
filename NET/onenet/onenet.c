#include "stm32f10x.h"
#include <string.h>
#include <stdio.h>
#include "esp8266.h"
#include "onenet.h"
#include "mqttkit.h"
#include "led.h"
#include "main.h"

#define PROID		"554594" //产品ID

#define AUTH_INFO	"1234"	//鉴权信息

#define DEVID		"1012222074"	//设备ID
extern u8 buff[30];//参数显示缓存数组
extern unsigned char esp8266_buf[256];
u8 status_connected = 0;

//==========================================================
//	函数名称：	OneNet_DevLink
//
//	函数功能：	与onenet创建连接
//
//	入口参数：	无
//
//	返回参数：	1-成功	0-失败
//
//	说明：		与onenet平台建立连接
//==========================================================
_Bool OneNet_DevLink(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					//协议包

	unsigned char *dataPtr;
	
	_Bool status = 1;
	
	printf("OneNet_DevLink\r\nPROID: %s,	AUIF: %s,	DEVID:%s\r\n", PROID, AUTH_INFO, DEVID);
	
	if(MQTT_PacketConnect(PROID, AUTH_INFO, DEVID, 256, 0, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0)
	{
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//上传平台
		
		dataPtr = ESP8266_GetIPD(250);									//等待平台响应
		if(dataPtr != NULL)
		{
			if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK)
			{
				switch(MQTT_UnPacketConnectAck(dataPtr))
				{
					case 0:printf("Tips:	连接成功\r\n");status = 0;break;
					
					case 1:printf("WARN:	连接失败：协议错误\r\n");break;
					case 2:printf("WARN:	连接失败：非法的clientid\r\n");break;
					case 3:printf("WARN:	连接失败：服务器失败\r\n");break;
					case 4:printf("WARN:	连接失败：用户名或密码错误\r\n");break;
					case 5:printf("WARN:	连接失败：非法链接(比如token非法)\r\n");break;
					
					default:printf("ERR:	连接失败：未知错误\r\n");break;
				}
			}
		}
		
		MQTT_DeleteBuffer(&mqttPacket);								//删包
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
	
	//温度
	memset(text, 0, sizeof(text));
	sprintf(text, "temp,%d.%d;", send_data.tempH,send_data.tempL);
	strcat(buf, text);
	//湿度
	memset(text, 0, sizeof(text));
	sprintf(text, "humi,%d.%d;", send_data.humiH,send_data.humiL);
	strcat(buf, text);
	//光照
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
	//风扇
	memset(text, 0, sizeof(text));
	sprintf(text, "fan,%d;", send_data.fan);
	strcat(buf, text);
	//继电器
	memset(text, 0, sizeof(text));
	sprintf(text, "relay,%d;", send_data.relay);
	strcat(buf, text);
	//蜂鸣器
	memset(text, 0, sizeof(text));
	sprintf(text, "beep,%d;", send_data.beep);
	strcat(buf, text);
	//MQ传感器阈值
	memset(text, 0, sizeof(text));
	sprintf(text, "max_mq,%d;", send_data.max_mq);
	strcat(buf, text);
	//温度上限
	memset(text, 0, sizeof(text));
	sprintf(text, "max_temp,%d;", send_data.max_temp);
	strcat(buf, text);
	//光照下限
	memset(text, 0, sizeof(text));
	sprintf(text, "max_light,%d;", send_data.max_light);
	strcat(buf, text);
	//温度报警
	memset(text, 0, sizeof(text));
	sprintf(text, "alarm_temp,%d;", send_data.alarm_temp);
	strcat(buf, text);
	//mq报警
	memset(text, 0, sizeof(text));
	sprintf(text, "alarm_mq,%d;", send_data.alarm_mq);
	strcat(buf, text);
	//mq报警
	memset(text, 0, sizeof(text));
	sprintf(text, "alarm_light,%d;", send_data.alarm_light);
	strcat(buf, text);
	//mode 自动手动模式
	memset(text, 0, sizeof(text));
	sprintf(text, "mode,%d;", send_data.mode);
	strcat(buf, text);
	return strlen(buf);

}

//==========================================================
//	函数名称：	OneNet_SendData
//
//	函数功能：	上传数据到平台
//
//	入口参数：	type：发送数据的格式
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OneNet_SendData(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};												//协议包
	
	char buf[256];
	
	short body_len = 0, i = 0;
	
	printf("Tips:	OneNet_SendData-MQTT\r\n");
	
	memset(buf, 0, sizeof(buf));
	
	body_len = OneNet_FillBuf(buf);																	//获取当前需要发送的数据流的总长度
	
	if(body_len)
	{
		if(MQTT_PacketSaveData(DEVID, body_len, NULL, 5, &mqttPacket) == 0)							//封包
		{
			for(; i < body_len; i++)
				mqttPacket._data[mqttPacket._len++] = buf[i];
			
			ESP8266_SendData(mqttPacket._data, mqttPacket._len);									//上传数据到平台
			printf("Send %d Bytes\r\n", mqttPacket._len);
			LED1 = 0;
			MQTT_DeleteBuffer(&mqttPacket);															//删包
		}
		else
		{
			printf("WARN:	EDP_NewBuffer Failed\r\n");
			LED1 = 1;
		}
	}
	
}

//==========================================================
//	函数名称：	OneNet_RevPro
//
//	函数功能：	平台返回数据检测
//
//	入口参数：	dataPtr：平台返回的数据
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OneNet_RevPro(unsigned char *cmd)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};								//协议包
	
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
		case MQTT_PKT_CMD:															//命令下发
			
			result = MQTT_UnPacketCmd(cmd, &cmdid_topic, &req_payload, &req_len);	//解出topic和消息体
			if(result == 0)
			{
				printf("cmdid: %s, req: %s, req_len: %d\r\n", cmdid_topic, req_payload, req_len);
				
				if(MQTT_PacketCmdResp(cmdid_topic, req_payload, &mqttPacket) == 0)	//命令回复组包
				{
					printf("Tips:	Send CmdResp\r\n");
					OLED_Clear();
					sprintf((char*)buff,"Recive message!");
					OLED_ShowString(0,0,buff,12);//fan
					OLED_Refresh();
					
					ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//回复命令
					MQTT_DeleteBuffer(&mqttPacket);									//删包
				}
			}
		
		break;
			
		case MQTT_PKT_PUBACK:														//发送Publish消息，平台回复的Ack
		
			if(MQTT_UnPacketPublishAck(cmd) == 0)
				printf("Tips:	MQTT Publish Send OK\r\n");
			
		break;
		
		default:
			result = -1;
		break;
	}
	
	ESP8266_Clear();									//清空缓存
	
	if(result == -1)
		return;
	
	dataPtr = strchr(req_payload, ':');					//搜索'}'

	if(dataPtr != NULL && result != -1)					//如果找到了
	{
		dataPtr++;
		
		while(*dataPtr >= '0' && *dataPtr <= '9')		//判断是否是下发的命令控制数据
		{
			numBuf[num++] = *dataPtr++;
		}
		numBuf[num] = 0;
		
		num = atoi((const char *)numBuf);				//转为数值形式
		
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
