//	tem = ((buff[0]<<8) | buff[1]);//温度拼接
//	hum = ((buff[3]<<8) | buff[4]);//湿度拼接
//	
//	/*转换实际温度*/
//	*Temperature= (175.0*(float)tem/65535.0-45.0) ;// T = -45 + 175 * tem / (2^16-1)
//	*Humidity= (100.0*(float)hum/65535.0);// RH = hum*100 / (2^16-1)
//	
