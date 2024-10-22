#include "PID_bsp.h"
int state_of_result;

// PID��ʼ��


PID spd_pid={.kp=8,.ki=0.5,.kd=1,.MaxOut = 16000, .IntegralLimit = 1000, .DeadBand = 10};
PID pos_pid={.kp=0.1,.ki=0.02,.kd=0.001,.MaxOut = 14000, .IntegralLimit = 20000, .DeadBand = 100};

FUZZY fuzzy_spd_pid={

 .case_far_away={.kp=8,.ki=0,.kd=1,.MaxOut = 16000, .IntegralLimit = 1000, .DeadBand = 10},
 .case_close_to_goal={.kp=4,.ki=0.5,.kd=1,.MaxOut = 16000, .IntegralLimit = 1000, .DeadBand = 10},
 .case_stay_goal={.kp=2,.ki=1,.kd=1,.MaxOut = 16000, .IntegralLimit = 1000, .DeadBand = 10},
                    };


/**************************************************************************
  * @brief              : ����ʽPID����
  * @param[in]     : pid:PID��������λ�ã�now����ǰֵ��goal��Ŀ��ֵ
  * @param[out]   : PID�����ͼ�����
* @return            : PID�����������
  * @others           : ����
***************************************************************************/
float pidcalulate(PID *pid, float now, float goal)
{
	pid->err_Last = pid->err;
	pid->err = goal - now;

	pid->err = DEADBAND(pid->err, pid->DeadBand);

	if (pid->err != 0)
		pid->err_Sum += pid->err;
	else
		pid->err_Sum = 0;
	pid->err_Sum = LIMIT(pid->err_Sum, pid->IntegralLimit);

	pid->P_Out = pid->kp * pid->err;
	pid->I_Out = pid->ki * pid->err_Sum;
	pid->D_Out = pid->kd * (pid->err - pid->err_Last);

	pid->PID_Out = LIMIT(pid->P_Out + pid->I_Out + pid->D_Out, pid->MaxOut);

	return pid->PID_Out;
}
/**************************************************************************
  * @brief              : ģ���ж�
  * @param[in]     : fuzzy:ģ����������λ�ã�PID�����ͼ�����
  * @param[out]   : 
* @return            : ģ��������PID�������
  * @others           : ����
***************************************************************************/

PID fuzzy_judge(FUZZY fuzzy, PID pid_NOW)
{
   
        if(pid_NOW.err/8192.f/19.f*360>100||pid_NOW.err/8192.f/19.f*360<-100)
    {
        fuzzy.out=fuzzy.case_far_away;
         state_of_result=0;
    }
    else 
        if(pid_NOW.err/8192.f/19.f*360>30||pid_NOW.err/8192.f/19.f*360<-30)
    {
        fuzzy.out=fuzzy.case_close_to_goal;
         state_of_result=1;
    }
    else 
        
    {
        fuzzy.out=fuzzy.case_stay_goal;
         state_of_result=2;
    }
    
    return fuzzy.out;
   
}

