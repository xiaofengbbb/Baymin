#include "stm32f10x.h"
#include "adc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_tim.h"
#include "usart.h"
#include "delay.h"

/*******************************************
*函数名称：void ADC1_Config(void)
*函数功能：配置ADC1
*输入参数：无
*返回值： 无
*函数说明：
********************************************/
char* ADC1_Init()
{
  ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_GPIOA, ENABLE); 	//打开ADC时钟
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;	  //PA5
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//模拟输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;	  //  PA6
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//模拟输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	ADC_DeInit(ADC1);
  ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;//独立工作模式
  ADC_InitStructure.ADC_ScanConvMode=DISABLE;//单通道模式
  ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;//单次转换模式
  ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//外部触发禁止
  ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;//数据右对齐
  ADC_InitStructure.ADC_NbrOfChannel=1;//用于转换的通道数为1
  ADC_Init(ADC1,&ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1,1,1,ADC_SampleTime_55Cycles5);	//ADC规则通道设置
	ADC_RegularChannelConfig(ADC1,6,1,ADC_SampleTime_55Cycles5);
	ADC_Cmd(ADC1,ENABLE);	   //使能ADC1
	ADC_ResetCalibration(ADC1);  //重新指定ADC1的校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1));   //检查校准是否结束 
	ADC_StartCalibration(ADC1); //开启ADC1校准
	while(ADC_GetCalibrationStatus(ADC1));	 //检查校准是否结束
	return "ADC success!";
			
}

/*******************************************
*函数名称：void ADC1_GetValue(void)
*函数功能：获取ADC1转换值
*输入参数：无
*返回值： 无
*函数说明：
********************************************/
uint16_t ADC1_GetValue(u8 ch)
{
	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_239Cycles5);
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);	     //开始软件转换
  while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));    //等待转换完成
  return ADC_GetConversionValue(ADC1);	 
}


/*******************************************
*函数名称：void ADC1_Process(void)
*函数功能：ADC1转换
*输入参数：无
*返回值： 无
*函数说明：
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
