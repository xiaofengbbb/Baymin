#include "stm32f10x.h"
#include "adc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_tim.h"
#include "usart.h"
#include "delay.h"

/*******************************************
*�������ƣ�void ADC1_Config(void)
*�������ܣ�����ADC1
*�����������
*����ֵ�� ��
*����˵����
********************************************/
char* ADC1_Init()
{
  ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_GPIOA, ENABLE); 	//��ADCʱ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;	  //PA5
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//ģ������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;	  //  PA6
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//ģ������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	ADC_DeInit(ADC1);
  ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;//��������ģʽ
  ADC_InitStructure.ADC_ScanConvMode=DISABLE;//��ͨ��ģʽ
  ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;//����ת��ģʽ
  ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//�ⲿ������ֹ
  ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;//�����Ҷ���
  ADC_InitStructure.ADC_NbrOfChannel=1;//����ת����ͨ����Ϊ1
  ADC_Init(ADC1,&ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1,1,1,ADC_SampleTime_55Cycles5);	//ADC����ͨ������
	ADC_RegularChannelConfig(ADC1,6,1,ADC_SampleTime_55Cycles5);
	ADC_Cmd(ADC1,ENABLE);	   //ʹ��ADC1
	ADC_ResetCalibration(ADC1);  //����ָ��ADC1��У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));   //���У׼�Ƿ���� 
	ADC_StartCalibration(ADC1); //����ADC1У׼
	while(ADC_GetCalibrationStatus(ADC1));	 //���У׼�Ƿ����
	return "ADC success!";
			
}

/*******************************************
*�������ƣ�void ADC1_GetValue(void)
*�������ܣ���ȡADC1ת��ֵ
*�����������
*����ֵ�� ��
*����˵����
********************************************/
uint16_t ADC1_GetValue(u8 ch)
{
	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_239Cycles5);
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);	     //��ʼ���ת��
  while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));    //�ȴ�ת�����
  return ADC_GetConversionValue(ADC1);	 
}


/*******************************************
*�������ƣ�void ADC1_Process(void)
*�������ܣ�ADC1ת��
*�����������
*����ֵ�� ��
*����˵����
********************************************/
uint8_t ADC1_Light_Process(u8 ch)
{
	uint32_t sum = 0;
	uint16_t average = 0;
	uint8_t  value = 0;
	uint8_t i;
	for(i=0;i<10;i++)
	{
		sum+= ADC1_GetValue(ch);
		delay_ms(5);
	}
  average = sum/10;
	if(average>4000)
		 average = 4000;
  value = 100 - average/40;
	return value;
}


uint8_t ADC1_mq135_Process(u8 ch)
{
	uint32_t sum2 = 0;
	uint32_t average2 = 0;
	uint8_t  value2 = 0;
	uint8_t i;
	for(i=0;i<10;i++)
	{
		sum2+= ADC1_GetValue(ch)/10;
		delay_ms(5);
	}
  average2 = sum2/10;
	value2 = average2;
	
	return  value2 ;
}
