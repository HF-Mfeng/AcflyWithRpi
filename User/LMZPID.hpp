
#ifndef LPID__HPP
#define LPID__HPP

class LPID
{
public:
	LPID() {
		this->Kp = 0;
		this->Ki = 0;
		this->Kd = 0;
		this->setPoint = 0;
		this->all_error = 0;
		this->last_error = 0;
		this->last_last_error = 0;
		this->sameple_time_ms = -1;
	}
	LPID(double Kp, double Ki, double Kd, double time_ms = -1, double setPoint = 0) {
		this->Kp = Kp;
		this->Ki = Ki;
		this->Kd = Kd;
		this->setPoint = setPoint;
		this->all_error = 0;
		this->last_error = 0;
		this->last_last_error = 0;
		this->sameple_time_ms = time_ms;
	}

// 差距：目标-本身
// difference = target - value
	double PIDControl_byDiff(double difference, int way = Incremental) { // 根据差值来算的
		double control;
	// P调节，比例
		double Pitem = this->Kp * difference;

		// I调节，积分
		double Iitem = 0;
		switch (way)
		{
			case Position: // 位置式，总误差
				Iitem = this->Ki * this->all_error;
				break;
			case Incremental: // 此次误差，上次误差，上上次误差
				Iitem = this->Ki * (difference + this->last_error + this->last_last_error);
				break;
			default:
				break;
		}

		// D调节，微分
		double Ditem = this->Kd * (difference - this->last_error);
		control = Pitem + Iitem + Ditem;

		// 结果累积
		this->all_error += difference;
		this->last_last_error = this->last_error;
		this->last_error = difference;

		return control;
	}

	double PIDControl_byValu(double value, int way = Incremental) {
		return this->PIDControl_byDiff(this->setPoint - value, way);
	}
	double sameple_time_ms; // 每次调节的时间,单位 ms
 enum ways
	{
		Position,
		Incremental
	};
	void set_p(double p) {
		this->Kp = p;
	}
	void set_i(double i) {
		this->Ki = i;
	}
	void set_d(double d) {
		this->Kd = d;
	}
private:
	double Kp, Ki, Kd, setPoint;

	// 位置式PID， 与整个状态有关
	double all_error;

	// 增量式PID， 与此次和前两次状态有关
	double last_error, last_last_error;
};

#endif // !1


