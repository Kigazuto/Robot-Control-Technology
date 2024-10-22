#ifndef __PID_BSP__
#define __PID_BSP__

#include "math.h"

#define LIMIT(x, limit) (x >= limit ? limit : (x <= -limit ? -limit : x))
#define DEADBAND(x, limit) (x >= limit ? x : (x <= -limit ? x : 0))

typedef struct pid
{
	float kp;
	float ki;
	float kd;

	float DeadBand;//死区
	float IntegralLimit;//积分上限
	float MaxOut;//最大输出

	float err;//目标差
	float err_Last;//上一次目标差
	float err_Sum;//目标差的积分

	float P_Out;//P控制器的输出
	float I_Out;//I控制器的输出
	float D_Out;//D控制器的输出

	float PID_Out;
} PID;

typedef struct fuzzy
  {
      
  PID case_far_away;//err较大情况
  PID case_close_to_goal;//接近goal
  PID case_stay_goal;//保持goal
  PID out;//当前使用的pid
  } FUZZY;  
extern int state_of_result;//显示FUZZY的结果状态
float pidcalulate(PID *pid, float now, float goal);
PID fuzzy_judge(FUZZY fuzzy, PID pid_NOW);
void PID_init(void);
extern PID spd_pid;
extern PID pos_pid;
extern FUZZY fuzzy_spd_pid;

#endif
