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

	float DeadBand;//����
	float IntegralLimit;//��������
	float MaxOut;//������

	float err;//Ŀ���
	float err_Last;//��һ��Ŀ���
	float err_Sum;//Ŀ���Ļ���

	float P_Out;//P�����������
	float I_Out;//I�����������
	float D_Out;//D�����������

	float PID_Out;
} PID;

typedef struct fuzzy
  {
      
  PID case_far_away;//err�ϴ����
  PID case_close_to_goal;//�ӽ�goal
  PID case_stay_goal;//����goal
  PID out;//��ǰʹ�õ�pid
  } FUZZY;  
extern int state_of_result;//��ʾFUZZY�Ľ��״̬
float pidcalulate(PID *pid, float now, float goal);
PID fuzzy_judge(FUZZY fuzzy, PID pid_NOW);
void PID_init(void);
extern PID spd_pid;
extern PID pos_pid;
extern FUZZY fuzzy_spd_pid;

#endif
