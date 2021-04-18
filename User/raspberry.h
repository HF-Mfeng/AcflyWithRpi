
#ifndef __RASPBERRY__H__
#define __RASPBERRY__H__
#include "string"

//using namespace std;

void rpi_setBauds( int bauds = 57600 );
void LMZ_SendMessage(std::string stre);
void LMZ_SendMessage(std::string stre, std::vector<double> &value);
std::string receiveData();

#endif
