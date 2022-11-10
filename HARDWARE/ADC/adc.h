#ifndef __ADC_H
#define __ADC_H 			   
#include <stm32f10x.h>

void ADC_Config(void);
char* ADC1_Init();
uint16_t ADC1_GetValue(u8 ch);
uint8_t ADC1_Light_Process(u8 ch);
uint8_t ADC1_mq135_Process(u8 ch);

#endif
