#include "key.h"
#include "delay.h"
#include  "led.h"

/*******************************************************************************
* �� �� ��         : KEY_Init
* ��������		   : ������ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
char* KEY_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ�����	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
//	GPIO_InitStructure.GPIO_Pin=KEY0_Pin;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//��������
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(KEY_Port,&GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin=KEY1_Pin;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//��������
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(KEY_Port,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=KEY2_Pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//��������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(KEY_Port,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=KEY3_Pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//��������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(KEY_Port,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=KEY4_Pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//��������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(KEY_Port,&GPIO_InitStructure);

	return "KEY success!";

}

/*******************************************************************************
* �� �� ��         : KEY_Scan
* ��������		   : ����ɨ����
* ��    ��         : mode=0:���ΰ��°���
					 mode=1���������°���
* ��    ��         : 0��δ�а�������
					 KEY1_VALUE��KEY1������
					 KEY2_VALUE��KEY2������
*******************************************************************************/
//u8 KEY_Scan(u8 mode)
//{
//	static u8 key=1;
//	if(key==1&&(KEY1==0||KEY2==0)) //����һ����������
//	{
//		delay_ms(10);  //����
//		key=0;
//		if(KEY1==0)
//		{
//	     LED2=!LED2;
//		}
//		else if(KEY2==0)
//		{
//			LED2=!LED2 ;
//		}
//	}
//	else if(KEY1==1&&KEY2==1)    //�ް�������
//	{
//		key=1;
//	}
//	if(mode==1) //������������
//	{
//		key=1;
//	}
//	return 0;
//}
