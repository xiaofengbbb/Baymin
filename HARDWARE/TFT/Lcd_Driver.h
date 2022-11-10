#ifndef __Lcd_Driver_H
#define __Lcd_Driver_H


#include "STM32F10x.h"

//STM32F103���İ�����
/************** Ƕ��ʽ������  **************/
/********** mcudev.taobao.com ��Ʒ  ********/


#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//��ɫ0 3165 00110 001011 00101
#define GRAY1   0x8410      	//��ɫ1      00000 000000 00000
#define GRAY2   0x4208      	//��ɫ2  1111111111011111




#define LCD_CTRL   	  	  GPIOB		//����TFT ���ݶ˿�

#define LCD_CTRL2   	  	GPIOA		//����BLK ���ݶ˿�

#define LCD_BLK        	GPIO_Pin_8   //PA8--BLK
#define LCD_RS         	GPIO_Pin_1	 //PB11---RS/DC
#define LCD_CS        	GPIO_Pin_12  //PB11---CS/CE

#define LCD_SCL        	GPIO_Pin_13	 //PB13--SCL/SCK
#define LCD_SDA        	GPIO_Pin_15	 //PB15 --SDA/DIN

#define LCD_SDO         GPIO_Pin_14  //PB14----SDO

//Һ�����ƿ���1�������궨��
#define	LCD_CS_SET  	LCD_CTRL->BSRR=LCD_CS    
#define	LCD_RS_SET  	LCD_CTRL->BSRR=LCD_RS    
#define	LCD_SDA_SET  	LCD_CTRL->BSRR=LCD_SDA    
#define	LCD_SCL_SET  	LCD_CTRL->BSRR=LCD_SCL    
#define	LCD_RST_SET  	LCD_CTRL->BSRR=LCD_RST

#define	LCD_SDO_SET  	LCD_CTRL->BSRR=LCD_SDO

#define	LCD_BLK_SET  	LCD_CTRL2->BSRR=LCD_BLK   

//Һ�����ƿ���0�������궨��
#define	LCD_CS_CLR  	LCD_CTRL->BRR=LCD_CS    
#define	LCD_RS_CLR  	LCD_CTRL->BRR=LCD_RS    
#define	LCD_SDA_CLR  	LCD_CTRL->BRR=LCD_SDA    
#define	LCD_SCL_CLR  	LCD_CTRL->BRR=LCD_SCL    

#define	LCD_SDO_CLR  	LCD_CTRL->BRR=LCD_SDO

#define	LCD_BLK_CLR  	LCD_CTRL2->BRR=LCD_BLK 



#define LCD_DATAOUT(x) LCD_DATA->ODR=x; //�������
#define LCD_DATAIN     LCD_DATA->IDR;   //��������

#define LCD_WR_DATA(data){\
LCD_RS_SET;\
LCD_CS_CLR;\
LCD_DATAOUT(data);\
LCD_WR_CLR;\
LCD_WR_SET;\
LCD_CS_SET;\
} 



void LCD_GPIO_Init(void);
void Lcd_WriteIndex(u8 Index);
void Lcd_WriteData(u8 Data);
void Lcd_WriteReg(u8 Index,u8 Data);
u16 Lcd_ReadReg(u8 LCD_Reg);
void Lcd_Reset(void);
void Lcd_Init(void);
void Lcd_Clear(u16 Color);
void Lcd_SetXY(u16 x,u16 y);
void Gui_DrawPoint(u16 x,u16 y,u16 Data);
unsigned int Lcd_ReadPoint(u16 x,u16 y);
void Lcd_SetRegion(u16 x_start,u16 y_start,u16 x_end,u16 y_end);
void LCD_WriteData_16Bit(u16 Data);

#endif
