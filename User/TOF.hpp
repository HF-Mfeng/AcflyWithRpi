
#ifndef TOF__HPP
#define TOF__HPP

#include "Basic.hpp"
#include "vector"
#include "drv_Uart7.hpp"
#include "string"

//Read_UartRpi(&data_temp, 1 , 2, 5)
#define TOF_USE_UART7
#ifdef TOF_USE_UART7
#define Read_UartTOF Read_Uart7
#define SetBaudRate_UartTOF SetBaudRate_Uart7
#endif
using namespace std;



class LMZ_TOF{
public:
		LMZ_TOF( int DataLength = 16 ){
			this->DataLength = DataLength;
			sum = 0;
			distance = -1;
		}
		
	  double distance;
		vector<uint8_t> datas;
		string errorInformation;
		
		
		void receiveAllData(){
					int count = 0 ;
					while (true)
					{
						uint8_t data_temp ;
						if(Read_UartTOF(&data_temp, 1 , 0.5 , 0.5)){
								if(data_temp == this->head[0]){
										datas.push_back(data_temp);
										count++ ;
									  sum += data_temp;
								}else if( count > 0 ){
										datas.push_back(data_temp) ;
										count++ ;
										sum += data_temp;
									if( count > DataLength - 1 )
										break;
								}
						}
						else break;
					}
		}		
		
		void createError(){
			 this->errorInformation = "" ;
			 if(datas[0]  != head[0]) this->errorInformation += "The first head error.|" ;
			 if(datas[1]  != head[1]) this->errorInformation += "The second head error.|" ;
			 if(datas[11] != 0x00   ) this->errorInformation += "The status error.|" ;
		}
		// 获取距离信息
		double getTOFdistance(){
			this->receiveAllData();
			this->distance = -1;
			if(  datas[0] == head[0] 
				&& datas[1] == head[1]
				// && (datas[11] == 0x00 || datas[11] == 0x02)
			){  // 数据可用
						this->distance = (double) (datas[8]<<8|datas[9]<<16|datas[10]<<24)/2560.0f;
				}
			
				// this->distance = (double)(datas[8]<<8|datas[9]<<16|datas[10]<<24)/2560.0f;
//				if(this->distance < 0){
//					createError();
//				}
				this->errorInformation = "status = " + to_string(datas[11]) ;
				datas.clear();
				return this->distance;
		}
		
		
		static void TOF_setBauds( int bauds ){
			if (bauds <= 0)
				return ;
			while(!SetBaudRate_UartTOF(bauds,0.5,2));
		}
		
private:
		uint8_t head[2] = {0x57, 0x00};
	  int DataLength;
		uint8_t sum;


};

#endif
