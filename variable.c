/********************    ELoader  通讯板嵌入式程序    ********************
Copyright (C), 2010, Zidetech. Co., Ltd.
* File Name          : variable.c
* Author             : lu guangping
* Version            : V0.0.1
* Date               : 2010/9/15
* Description        : 软件用到的变量
**************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "include.h"



AUTO_RESULT_TYPE auto_result;

//获取CTRL状态参数的定时计数器
volatile u16  cnt_get_state;

//远程通信用结构体变量，存储相关信息
REMOTE_INFO_T remote_info;
COMM_CTRL_PORT comm_ctrl;

EVENTFUN Send_event, Recv_event;//发送和接收，待处理事件

bool scpi_proc_ava;//SCPI解析器是否可用

//System_T sys_info;

unReg32 sys_regs[AMOUNT_OF_REGISTER];//寄存器顺序参考load_register.h中宏的定义

u16 (*GetChar)(void);
void (*PrintStr)(char*pstr);


SYST_COMM_INFO_T gSystCommInfo;