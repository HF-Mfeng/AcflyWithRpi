#ifndef LMZLINK__HPP
#define LMZLINK__HPP

#include <string>
#include <vector>

//#define DEBUG
#ifdef DEBUG
#include <iostream>
#endif // DEBUG


class LMZLink
{
public:

	int mode; // 见MODE
	std::vector<std::string> params; // 参数名称
	std::vector<double> paramsValue; // 参数值

	LMZLink() {
		this->mode = -1;
	}


#ifdef DEBUG
	void show() {
		std::cout << this->mode << std::endl;
		for (int i = 0; i < this->params.size(); i++)
		{
			std::cout << "mode: " << this->params[i] << ":" << this->paramsValue[i] << std::endl;
		}

	}
#endif // DEBUG

 enum MODE
	{
		speedControl,     // 速度控制	单位：cm/s,0
		distanceControl, // 距离控制	单位：cm,1
		takeoff,        // 起飞			单位：cm,2
		land,          // 降落			单位：无,3
		alarm,        // 报警			单位：无,4
		changeAngle, // 改变转角		单位: °,5
		getHeight,  // 获取高度			返回单位：cm,6
		Height,    // 离地高度			单位：cm,7
		Wall_Distance, // 离墙距离   单位：cm,8
		NoMode
	};

	static const char split_char = '|';

	// 清楚所有数据
	void clearAllData(){
		this->mode = NoMode;
		this->params.clear();
		this->paramsValue.clear();
	}
	
	// 字符串分割
	static std::vector<std::string> split_str(std::string& str, char split_char = '|') {
		std::vector<std::string> stringlist;
		std::string temp;
		for (int i = 0, j = str.length(); i < j; i++)
		{
			if (str[i] == split_char)
			{
				if (!(temp.empty())) {
					stringlist.push_back(temp);
				}
				temp.clear();
			}
			else {
				temp.push_back(str[i]);
			}
		}
		return stringlist;
	}

	// 数据解析
	static LMZLink data_analyze(std::string& str) {
		LMZLink dst;
		std::vector<std::string> stringList = split_str(str, LMZLink::split_char);
		dst.mode = std::stoi(stringList[0]);
		for (int i = 1, j = stringList.size(); i < j; i++)
		{
			if (i % 2 == 0) {
				dst.paramsValue.push_back(std::stod(stringList[i]));
			}
			else {
				dst.params.push_back(stringList[i]);
			}
		}
		return dst;
	}

	// 数据打包
	static std::string data_unpack(LMZLink& mess) {
		std::string sendMess;
		sendMess += LMZLink::split_char;
		sendMess += std::to_string(mess.mode);
		sendMess += LMZLink::split_char;
		for (int i = 0, j = mess.params.size(); i < j; i++)
		{
			sendMess += mess.params[i];
			sendMess += LMZLink::split_char;
			sendMess += std::to_string(mess.paramsValue[i]);
			sendMess += LMZLink::split_char;
		}
		return sendMess;
	}

	static std::string addHeadTail(std::string &str){
		std::string out ;
		out = "QQQ" + str + "W";
		return out;
	}
};


#endif // ! LMZLINK__HPP

