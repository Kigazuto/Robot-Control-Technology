#include "filter.h"
#include <string.h>
#include "stdint.h"
#include "math.h"
#include "RM3508.h"
#define FILTER_SIZE 100


float filterCoeffs[FILTER_SIZE] = {
1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,
 1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,
 1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,
 1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,
 1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,
 1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,
 1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,2/100.f,1/100.f,1/100.f,1/100.f,1/100.f,
 1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,
 1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,
 1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,1/100.f,
    };
float filterState[2][FILTER_SIZE]; // 缓冲区大小为滤波器长度
float fn_out;
float new_fn;
float Fn_out;
    
    
// 在循环中处理刷新频率为1ms的信号
float filter0FnManual(float in)
{

        // 读取新的fn信号数据，保存至new_fn中
        new_fn=in;
        // 更新滤波器状态缓存区
        for (int i = FILTER_SIZE - 1; i > 0; i--)
        {
            filterState[0][i] = filterState[0][i-1];
        }
        filterState[0][0] = new_fn;

        // 卷积计算
        fn_out= 0.f;
        for (int i = 0; i < FILTER_SIZE; i++) {
            fn_out += filterCoeffs[i] * filterState[0][i];
        }

        //  将滤波后的输出数据保存下来
        Fn_out=fn_out;
        return Fn_out;
}
    
float filter1FnManual(float in)
{

        // 读取新的fn信号数据，保存至new_fn中
        new_fn=in;
        // 更新滤波器状态缓存区
        for (int i = FILTER_SIZE - 1; i > 0; i--)
        {
            filterState[1][i] = filterState[1][i-1];
        }
        filterState[1][0] = new_fn;

        // 卷积计算
        fn_out= 0.f;
        for (int i = 0; i < FILTER_SIZE; i++) {
            fn_out += filterCoeffs[i] * filterState[1][i];
        }

        //  将滤波后的输出数据保存下来
        Fn_out=fn_out;
        return Fn_out;
}




