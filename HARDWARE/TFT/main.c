//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////	

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "delay.h"
#include "TFT_demo.h"
#include "Lcd_Driver.h"
#include "GUI.h"


//STM32F103���İ�����
/************** Ƕ��ʽ������  **************/
/********** mcudev.taobao.com ��Ʒ  ********/


int main(void)
{

  SystemInit();	//System init.
  delay_init(72);//Delay init.
  while(1)
  {  
	  Test_Demo();	//See the test details in QDTFT_Demo.c		
  }

}





/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
