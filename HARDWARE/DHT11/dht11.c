#include "dht11.h"
#include "delay.h"


//由于DHT11为单总线通信，即发送、接收都为同一根数据线，STM32的GPIO无法像51的IO同时配置为输入输出模式，
//因此需要将与DHT11数据线相连的GPIO写两套初始化函数，向DHT11发送数据时先调用DHT11_IO_OUT()函数，再
//发送数据，接收DHT11的数据时先调用DHT11_IO_IN()函数，再接收数据
void DHT11_IO_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(DHT11_GPIO_CLK|RCC_APB2Periph_AFIO, ENABLE);	 //使能PG端口时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //由于PA15为JTAG调试接口，需要先禁用JTAG功能才能作为普通的GPIO口
	//使用，若使用的是普通的GPIO，可将   RCC_APB2Periph_AFIO  与  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE)  去掉    //禁用JTAG
	
 	GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;				 //PG11端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);				 //初始化IO口
}

void DHT11_IO_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(DHT11_GPIO_CLK|RCC_APB2Periph_AFIO, ENABLE);	 //使能PG端口时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);     //禁用JTAG
	
 	GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;				 //PG11端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //推挽输出
 	GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);				 //初始化IO口
}

//复位DHT11
void DHT11_Rst(void)	   
{                 
	DHT11_IO_OUT(); 	//SET OUTPUT
  DHT11_DQ_OUT=0; 	//拉低DQ
  delay_ms(20);    	//拉低至少18ms
  DHT11_DQ_OUT=1; 	//DQ=1 
	delay_us(30);     	//主机拉高20~40us
}
//等待DHT11的回应
//返回1:未检测到DHT11的存在
//返回0:存在
u8 DHT11_Check(void) 	   
{   
	u8 retry=0;
	DHT11_IO_IN();//SET INPUT	 
    while (DHT11_DQ_IN&&retry<100)//DHT11会拉低40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11拉低后会再次拉高40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}
//从DHT11读取一个位
//返回值：1/0
u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(DHT11_DQ_IN&&retry<100)//等待变为低电平
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//等待变高电平
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//等待40us
	if(DHT11_DQ_IN)return 1;
	else return 0;		   
}
//从DHT11读取一个字节
//返回值：读到的数据
u8 DHT11_Read_Byte(void)    
{        
	u8 i,dat;
	dat=0;
	for (i=0;i<8;i++) 
	{
		dat<<=1; 
		dat|=DHT11_Read_Bit();
  }						    
  return dat;
}
//从DHT11读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
u8 DHT11_Read_Data(u8 *tempH,u8 *tempL,u8 *humiH,u8 *humiL)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//读取40位数据
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humiH=buf[2];
			*humiL=buf[3];
			*tempH=buf[0];
			*tempL=buf[1];
		}
	}else return 1;
	return 0;	    
}
//初始化DHT11的IO口 DQ 同时检测DHT11的存在
//返回1:不存在
//返回0:存在    	 
char* DHT11_Init(void)
{	 
 	
			    
	DHT11_Rst();  //复位DHT11
	DHT11_Check();//等待DHT11的回应
	return "DHT11 success!";
	
} 







