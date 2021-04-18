#include "raspberry.h"

#include "LMZLink.hpp"
#include "string"
#include "vector"

using namespace std;

// 下面是用uart8来使用树莓派
#include "drv_Uart8.hpp"
#define Read_UartRpi Read_Uart8
#define SetBaudRate_UartRpi SetBaudRate_Uart8
#define Write_UartRpi Write_Uart8
#define WaitSent_UartRpi WaitSent_Uart8
#define ResetRx_UartRpi ResetRx_Uart8

// 设置波特率
void rpi_setBauds( int bauds ){
	if (bauds <= 0)
		return ;
	while(!SetBaudRate_UartRpi(bauds,0.5,2));
}


// 发送数据
void LMZ_SendMessage(string stre){
	//rpi_setBauds(57600);
	uint8_t t_data[100];
	for (int i = 0; i < 100 && i < stre.length(); i++)
	{
		t_data[i] = stre[i];
	}
	Write_UartRpi(t_data, stre.length(), 2, 0.5);
	WaitSent_UartRpi(5);
}

// 发送数据
void LMZ_SendMessage(string stre, vector<double> &value){
	string temp = stre + ": |";
	for (int  i = 0; i < value.size(); i++)
	{
		temp += to_string(value[i]);
		temp += '|' ;
	}
	LMZ_SendMessage(temp);
}


// 接收并解析数据（去掉报头与报尾），返回数据字符串string
string receiveData(){
	//rpi_setBauds(57600);
	string rout;
	int rc_step = 0 ;
	char head = 'Q';
	char tail = 'W'; 
	int times = 20;
	while (times >= 0)
	{
		uint8_t data_temp ;
		if(Read_UartRpi(&data_temp, 1 , 2, 5)){
			if(rc_step == 0){ // 没有接收到报头
				if(data_temp == head)
					rc_step = 1;
				else times--;
			}else if(rc_step == 1){ // 已经接收到了报头，准备接收数据
				if(data_temp != head){
					if (data_temp == tail)
						break;
					rout += (char)data_temp;  // 加到字符串里面
				}
			}
		}else break;
	}
	if(rout.length() != 0)
		ResetRx_UartRpi(0.5);
	return rout;
}
