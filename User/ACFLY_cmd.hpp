#ifndef ACFLY_COMMAND__HPP
#define ACFLY_COMMAND__HPP
#include "LMZLink.hpp"
#include "string"
#include "ControlSystem.hpp"
#include "TOF.hpp"
#include "raspberry.h"
//#include "common_ACFLY.h"
#include "NavCmdProcess.hpp"

using namespace std;

class ACFLYcmd{

private:
				int sum ;
	
public:
	ACFLYcmd(){
		LMZ_TOF::TOF_setBauds(921600);
		freeStatus = true ;
		pos_judge = 0 ;
		Curcmd.clearAllData();
		sum = 0 ;
		this->freq = 50;
		init_NavCmdInf(&(this->navInf));
	}
//		ACFLYcmd(NavCmdInf t){
//			this->navInf = t;
//		LMZ_TOF::TOF_setBauds(921600);
//		freeStatus = true ;
//		pos_judge = 0 ;
//		Curcmd.clearAllData();
//		sum = 0 ;
//		this->freq = 50;
//	}
	
	// ����
	bool freeStatus;
	LMZLink Curcmd;
	LMZ_TOF myTOF ;
	int pos_judge ;
	double freq ;
	NavCmdInf navInf;
	
	// ����
	bool JudgeMode(string &strcmd){
		if(strcmd.length() != 0)
				this->Curcmd = LMZLink::data_analyze(strcmd);
		freeStatus = false ;
		switch(this->Curcmd.mode)
		{
			case LMZLink::MODE::speedControl:
					this->SpeedControl();
				break;
			
			case LMZLink::MODE::distanceControl:	
					this->DistanceControl();
				break;
			
			case LMZLink::MODE::takeoff:	
					this->Takeoff();
				break;
			
			case LMZLink::MODE::land:	
					this->LandMode();
				break;
			
			case LMZLink::MODE::Wall_Distance:	
					this->Distance_wall();
				break;
			
			case LMZLink::MODE::NoMode:
					return true ;
			
			default:
				return false;
		}
		return true ;
	}	
	
	// �ٶȿ���
	void SpeedControl(){
		double vx,vy,vz;
		for( int i = 0; i < Curcmd.params.size(); i++)
		{
			if(Curcmd.params[i] == std::string("vx"))
				vx = Curcmd.paramsValue[i];
			if(Curcmd.params[i] == std::string("vy"))
				vy = Curcmd.paramsValue[i];
			if(Curcmd.params[i] == std::string("vz"))
				vz = Curcmd.paramsValue[i];
		}
			Position_Control_set_TargetVelocityBodyHeadingXY_AngleLimit(vx,vy);
			Position_Control_set_TargetVelocityZ(vz);
			freeStatus = true ;
	} // speedControl end
	
	
	//�������
	void DistanceControl(){
		double posx,posy,posz;
		for (int i = 0; i < Curcmd.params.size(); i++)
		{
			if (Curcmd.params[i] == std::string("posx"))
			{
				posx = Curcmd.paramsValue[i]/100;  
			}	
			else if (Curcmd.params[i] == std::string("posy"))
			{
				posy = Curcmd.paramsValue[i]/100;  
			} 
			else if (Curcmd.params[i] == std::string("posz"))
			{
				posz = Curcmd.paramsValue[i]/100;  
			} 
		}
/*-------------------------ִ������------------------------------------------*/
			double params[7];
			params[0] = 0;
			params[1] = 0;
			params[2] = 0;
			params[3] = 360;
			params[4] = posx;	params[5] = posy;
			params[6] = posz;
			int16_t res = Process_NavCmd( MAV_CMD_NAV_WAYPOINT, freq, MAV_FRAME_BODY_FLU, params, &navInf );
			if( NavCmdRs_SuccessOrFault(res) )
			{	//��ֱ�����
				init_NavCmdInf(&navInf);  // ���³�ʼ��
				Curcmd.clearAllData();
				freeStatus = true ;
				LMZ_SendMessage(string("DistanceControlOK"));
			}
	}
	
	//���
	void Takeoff(){
		double alt = 0;
		for (int i = 0; i < Curcmd.params.size(); i++)
		{
			if (Curcmd.params[i] == std::string("alt"))
			{
				alt = Curcmd.paramsValue[i]; 					
			}  
		}
/*----------------------ִ������------------------------------------*/
		double params[7];
						params[0] = 0;
						params[3] = nan("");
						params[4] = 0;	params[5] = 0;
						params[6] = alt / 100;
		int16_t res = Process_NavCmd( MAV_CMD_NAV_TAKEOFF, freq, MAV_FRAME_BODY_FLU, params, &navInf );
		if( NavCmdRs_SuccessOrFault(res) )
		{	//������
					init_NavCmdInf(&navInf);  // ���³�ʼ��
					Curcmd.clearAllData();
					freeStatus = true ;
					LMZ_SendMessage(string("takeoffOK"));
		}
	}//��ɽ���
  
	//������ǽ����
	void Distance_wall(){
		double pidvx,maxvx = 10;

		double distance_wall,wall_dis = 0;
		for (int i = 0; i < Curcmd.params.size(); i++)
		{
			if (Curcmd.params[i] == std::string("wall_dis"))
			{
				wall_dis = Curcmd.paramsValue[i];  
			}else if(Curcmd.params[i] == std::string("vmax")){
				maxvx = Curcmd.paramsValue[i];  
			}
		}
/*---------------------ִ������------------------------*/
		distance_wall = myTOF.getTOFdistance();  //��ȡ����
//									HHK_SendDistance(distance_wall);
		pidvx = distance_wall - wall_dis; //��ȡ��ֵ
		
		if(abs(pidvx) <= 5)         //���뾫ȷ��Χ����С����
		{
			maxvx = 5 ;
			this->sum++;
		}
		else
		{
			// maxvx = 10;
			this->sum = 0;
		}
		if(abs(pidvx) > maxvx)
		{	
			pidvx = maxvx*(pidvx/abs(pidvx));  //�޷�
		}
//									HHK_SendDistance(pidvx); 
		if(distance_wall <  0){
			pidvx = 0;
			Position_Control_set_ZLock();
			Position_Control_set_XYLock();
		//	LMZ_SendMessage("TOFSense error! Status:" + myTOF.errorInformation + " \n");
		}else{
			Position_Control_set_TargetVelocityBodyHeadingXY_AngleLimit(pidvx,0);
			Position_Control_set_ZLock();			
		//	LMZ_SendMessage("distance_wall = " + to_string(distance_wall) + " \v");
		}
		LMZ_SendMessage("Tdistance = " + to_string(distance_wall) + " --> " + myTOF.errorInformation + " \n");
	//	LMZ_SendMessage("distance_wall = " + to_string(distance_wall) + " , pidvx = " + to_string(pidvx) + " , sum = " + to_string(this->sum));

		if(this->sum == 5)   //ȷ��λ���ȶ�
		{
			this->sum = 0;
			Curcmd.clearAllData();
			freeStatus = true ;
			Position_Control_set_XYLock();
			Position_Control_set_ZLock();
			LMZ_SendMessage(string("Wall_DistanceOK"));
		}
	
	}
	
	
	void LandMode(){
	for (int i = 0; i < Curcmd.params.size(); i++)
	{
		if (Curcmd.params[i] == std::string("land"))
		{
			Position_Control_set_XYLock();
			Position_Control_set_TargetVelocityZ(-50);
			LMZ_SendMessage(string("landOK"));
		}
	}
}
	
};
#endif

