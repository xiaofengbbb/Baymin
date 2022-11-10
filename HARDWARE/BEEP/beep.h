#ifndef __BEEP_H
#define __BEEP_H	 
#include "sys.h"



//蜂鸣器端口定义
#define BEEP PAout(11)	// BEEP,蜂鸣器接口		   

#define BEEP_ON 0	
#define BEEP_OFF 1


char* BEEP_Init();	//初始化
		 				    
#endif

