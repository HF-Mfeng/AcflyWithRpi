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

	int mode; // ��MODE
	std::vector<std::string> params; // ��������
	std::vector<double> paramsValue; // ����ֵ

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
		speedControl,     // �ٶȿ���	��λ��cm/s,0
		distanceControl, // �������	��λ��cm,1
		takeoff,        // ���			��λ��cm,2
		land,          // ����			��λ����,3
		alarm,        // ����			��λ����,4
		changeAngle, // �ı�ת��		��λ: ��,5
		getHeight,  // ��ȡ�߶�			���ص�λ��cm,6
		Height,    // ��ظ߶�			��λ��cm,7
		Wall_Distance, // ��ǽ����   ��λ��cm,8
		NoMode
	};

	static const char split_char = '|';

	// �����������
	void clearAllData(){
		this->mode = NoMode;
		this->params.clear();
		this->paramsValue.clear();
	}
	
	// �ַ����ָ�
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

	// ���ݽ���
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

	// ���ݴ��
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

