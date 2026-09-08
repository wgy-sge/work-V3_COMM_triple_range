#ifndef __PORT__H
#define __PORT__H

/*------------------------ Platform includes --------------------------------*/
#include "stm32f10x_lib.h"



/* ----------------------- Defines ------------------------------------------*/
#define ENTER_CRITICAL_SECTION( )   EnterCriticalSection( )
#define EXIT_CRITICAL_SECTION( )    ExitCriticalSection( )

//事件定义
#define EVENT_PORT_1_RECEIVE       1
#define EVENT_PORT_2_RECEIVE       2
#define EVENT_TIMEOUT              88


typedef char    BOOL;

typedef unsigned char UCHAR;

typedef char    CHAR;

typedef unsigned short USHORT;
typedef short   SHORT;

typedef unsigned long ULONG;
typedef long    LONG;

typedef s16 smbError;

/*
#ifndef TRUE
#define TRUE            1
#endif

#ifndef FALSE
#define FALSE           0
#endif
*/
void            EnterCriticalSection( void );
void            ExitCriticalSection( void );




#endif
