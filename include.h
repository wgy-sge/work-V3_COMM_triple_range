
#ifndef __INCLUDE_H
#define  __INCLUDE_H


#include <string.h>
#include <stdlib.h>
#include <math.h>



#include    <stdio.h>
#include    <ctype.h>
#include    <stdlib.h>

#include    <setjmp.h>
#include    <stdarg.h>

#include <string.h>

#include "stm32f10x_it.h"
#include "stm32f10x_lib.h"
#include "stm32f10x_type.h"	


#include "init.h"
#include "gpib.h"
//#include "type.h"
#include "uart.h"
#include "variable.h" 
#include "load_register.h"

#include "fmb.h"


//SCPI
#include "cmd_tools.h"
#include "scpi_processor.h"
#include "scpi_interface.h"
#include "IEEE488.2_Errors.h"
#include "scpi_errors.h"



#include "app_timer.h"
#include "net_config.h"
#include "socket.h"
#include "spix.h"
#include "w5500.h"
#include "socket_app.h"
#include "mb_config.h"
#include "uvmb_lan.h"
#include "uvmb_tcp_lan.h"
#include "dma_uart1.h"
#include "uvmb_rs232.h"
#include "uvmb_rs485.h"
#include "trpa_trsm.h"
#include "engine.h"

#include "can.h"
#include "mco.h"
#include "canopenpoll.h"

//系统时间撮使用的定时器
#define RCC_APBPeriph_TIMER_SYS   RCC_APB1Periph_TIM4
#define TIMER_SYS                 TIM4

#define TIMER_SYS_NO              0
#define TIMER_SYS_IRQ             TIM4_IRQChannel

#define TIMER_SYS_PERIOD          5000//50ms为基本周期


#define    SOFT_VERSION          "V1.04-(v3)"
#define    SOFT_DATE             "26-11-16"

#define    SOFT_INFO             SOFT_VERSION\
	","SOFT_DATE  





#endif







